#include <cmath>
#include <complex>
#include <numbers>
#include <ostream>
#include <iostream>
#include <mpi.h>

#include "consts.h"
#include "frame.h"
#include "animation.h"

#define cimg_display 0        // No window plz
#include "CImg.h"

using std::cout, std::endl;
using namespace std::literals::complex_literals;

#define MAX_ITER 81

// Colour based on ratio between number of iterations and MAX_ITER
inline constexpr pixel COLOURISE(double iter) { 
    iter = fmod(4 - iter * 5 / MAX_ITER, 6);
    byte x = static_cast<byte>(255 * (1 - std::abs(fmod(iter, 2) - 1)));
    byte r, g, b;

    if      (             iter < 1) { r = 255; g =   x; b =   0; }
    else if (iter >= 1 && iter < 2) { r =   x; g = 255; b =   0; }
    else if (iter >= 2 && iter < 3) { r =   0; g = 255; b =   x; }
    else if (iter >= 3 && iter < 4) { r =   0; g =   x; b = 255; }
    else if (iter >= 4 && iter < 5) { r =   x; g =   0; b = 255; }
    else                            { r = 255; g =   0; b =   x; }
    return { r, g, b };
}


void renderFrame(animation &frames, unsigned int t, unsigned int offset) {
    // TODO - render frame t and store in frames[t-offset]
    int max_iter = MAX_ITER;
    int escape_radius = 2;

    double a = 2 * std::numbers::pi * t / CYCLE_FRAMES;
    double r = 0.7885;
    std::complex<double> c = r * cos(a) + static_cast<std::complex<double>>(1i) * r * sin(a);

    // Loop through all pixels in the frame
    for (unsigned int x = 0; x < WIDTH; x++) {
        for (unsigned int y = 0; y < HEIGHT; y++) {
            // Calculate the number of iterations for the pixel at (x,y)
            int iter = 0;
            double x_y_range = 2;

            //double scale = 1.5 - 1.45 * t / FRAMES;                           // iets simpeler
            double scale = 1.5 - 1.45 * log(1 + 9.0 * t / FRAMES) / log(10);    // iets interessanter om naar te kijken

            std::complex<double> z = 2 * x_y_range * std::complex(static_cast<double>(x)/WIDTH, static_cast<double>(y)/HEIGHT)
                - std::complex(x_y_range*3/4, x_y_range);

            z *= scale;

            while (std::abs(z) < escape_radius && iter < max_iter) {
                z = z*z + c;
                iter++;
            }

            if (iter == max_iter) {
                frames[t - offset].set_colour(x, y, {0, 0, 0});
            } else {
                pixel colour_hue = COLOURISE(static_cast<double>(iter));
                frames[t - offset].set_colour(x, y, colour_hue);
            }
        }
    }
}

int main (int argc, char *argv[]) {

    uint64_t i, combination;
    int id = -1, nprocs = 1;
    uint64_t start = 0, stop = 0;
    int count = 0, max_count = 0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    uint64_t chunk_size = FRAMES / nprocs;
    uint64_t remainder = FRAMES % nprocs;
    start = id * chunk_size + (id < remainder ? id : remainder);
    stop = start + chunk_size + (id < remainder ? 1 : 0);

    if (id == 0) {
        cout << "Number of processes: " << nprocs << endl;
    }
    
    // Needed to send frames over MPI
    MPI_Datatype mpi_img;
    MPI_Type_contiguous(FRAME_SIZE, MPI_BYTE, &mpi_img);
    MPI_Type_commit(&mpi_img);
    
    animation frames;

    if (id == 0) {
        frames.initialise(FRAMES);
    }
    
    std::vector<int> counts_per_proc(nprocs);
    for (int i = 0; i < nprocs; i++) {
        int local_start = i * chunk_size + (i < remainder ? i : remainder);
        int local_stop = local_start + chunk_size + (i < remainder ? 1 : 0);
        counts_per_proc[i] = local_stop - local_start;
    }

    animation local_frames(stop - start);

    cout << endl << "Process " << id << " is rendering frames " << start << " to " << stop << endl;

    double start_time = MPI_Wtime();

    // TODO - parallellisation
    for (combination = start; combination < stop; combination++) {
        // cout << endl << "Rendering frame " << combination << " on process [" << id << "]" << endl;
        renderFrame(local_frames, combination, start);
        count++;
    }

    std::vector<int> counts(nprocs);
    std::vector<int> recv_counts(nprocs);
    std::vector<int> displs(nprocs);
    MPI_Gather(&count, 1, MPI_INT, counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (id == 0) {
        for (int i = 0; i < nprocs; i++) {
            recv_counts[i] = counts[i] * FRAME_SIZE;  // per proces, bytes
        }

        displs[0] = 0;
        for (int i = 1; i < nprocs; i++) {
            displs[i] = displs[i - 1] + recv_counts[i - 1];
        }

    }

    MPI_Gatherv(
        local_frames.data(), 
        (stop - start) * FRAME_SIZE, 
        MPI_BYTE, 
        frames.data(), 
        recv_counts.data(),
        displs.data(),
        MPI_BYTE, 
        0, 
        MPI_COMM_WORLD
    );

    double end_time = MPI_Wtime() - start_time;
    double max_time = 0.0;

    MPI_Reduce(&count, &max_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&end_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (id == 0) {
        cimg_library::CImg<byte> img(WIDTH,HEIGHT,FRAMES,3);
        cimg_forXYZ(img, x, y, z) { 
            img(x,y,z,RED) = (frames)[z].get_channel(x,y,RED);
            img(x,y,z,GREEN) = (frames)[z].get_channel(x,y,GREEN);
            img(x,y,z,BLUE) = (frames)[z].get_channel(x,y,BLUE);
        }

        std::string filename = std::string("animation.avi");
        img.save_video(filename.c_str());

        cout << "\nTotal frames rendered: " << max_count << endl;
        cout << "Max execution time (across all processes): " << max_time << " seconds\n" << endl;
        cout << "Number of processes" << ";" << "Threads" << ";" << "Frames" << ";" << "Time" << ";" << endl;
        cout << nprocs << ";" << 1 << ";" << max_count << ";" << max_time << ";" << endl;
    }

    // Also needed to send frames over MPI
    MPI_Type_free(&mpi_img);
    MPI_Finalize();
    return 0;
}
