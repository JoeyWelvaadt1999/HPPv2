#include <cmath>
#include <complex>
#include <numbers>
#include <ostream>
#include <omp.h>
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
    #pragma omp parallel for collapse(2)
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
    int n_threads = 1;
    if (argc >= 2) {
        n_threads = std::stoi(argv[1]);
    }
    omp_set_num_threads(n_threads);

    int id = -1, nprocs = 1;
    
    cout << "Number of threads: " << n_threads << endl;
    
    animation frames;

    frames.initialise(FRAMES);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    double start_time = omp_get_wtime();
    int max_count = 0;
    

    // TODO - parallellisation
    #pragma omp parallel for
    for (int combination = 0; combination < FRAMES; combination++) {
        // cout << endl << "Rendering frame " << combination << " on process [" << id << "]" << endl;
        renderFrame(frames, combination, 0);
        max_count++;
    }

    double end_time = omp_get_wtime() - start_time;

    cimg_library::CImg<byte> img(WIDTH,HEIGHT,FRAMES,3);
    cimg_forXYZ(img, x, y, z) { 
        img(x,y,z,RED) = (frames)[z].get_channel(x,y,RED);
        img(x,y,z,GREEN) = (frames)[z].get_channel(x,y,GREEN);
        img(x,y,z,BLUE) = (frames)[z].get_channel(x,y,BLUE);
    }

    std::string filename = std::string("animation.avi");
    img.save_video(filename.c_str());

    cout << "\nTotal frames rendered: " << max_count << endl;
    cout << "Max execution time (across all processes): " << end_time << " seconds\n" << endl;
    cout << "Number of processes" << ";" << "Threads" << ";" << "Frames" << ";" << "Time" << ";" << endl;
    cout << 1 << ";" << n_threads << ";" << max_count << ";" << end_time << ";" << endl;

    MPI_Finalize();
    return 0;
}
