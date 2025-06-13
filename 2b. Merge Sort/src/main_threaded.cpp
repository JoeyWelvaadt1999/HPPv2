// #include <iostream>
// #include <vector>
// #include <random>
// #include <algorithm>
// #include <thread>
// #include <future>

// using namespace std;

// // Function to merge two sorted portions of the vector
// void merge(vector<float>& xs, size_t start, size_t mid, size_t end) {
//     vector<float> temp(end - start);
//     size_t i = start;
//     size_t j = mid;
//     size_t k = 0;
    
//     while (i < mid && j < end) {
//         if (xs[i] <= xs[j]) {
//             temp[k++] = xs[i++];
//         } else {
//             temp[k++] = xs[j++];
//         }
//     }
    
//     while (i < mid) {
//         temp[k++] = xs[i++];
//     }
    
//     while (j < end) {
//         temp[k++] = xs[j++];
//     }
    
//     // Copy back to original array
//     for (size_t i = 0; i < k; i++) {
//         xs[start + i] = temp[i];
//     }
// }

// // Sequential merge sort for individual chunks
// void sequential_merge_sort(vector<float>& xs, size_t start, size_t end) {
//     if (end - start <= 1) return;
    
//     size_t mid = start + (end - start) / 2;
//     sequential_merge_sort(xs, start, mid);
//     sequential_merge_sort(xs, mid, end);
//     merge(xs, start, mid, end);
// }

// // Parallel merge sort
// void parallel_merge_sort(vector<float>& xs, int num_threads) {
//     if (num_threads <= 0) num_threads = thread::hardware_concurrency();
//     vector<future<void>> futures;
    
//     // Calculate chunk size for each thread
//     size_t chunk_size = xs.size() / num_threads;
    
//     // Launch threads to sort individual chunks
//     for (int i = 0; i < num_threads; i++) {
//         size_t start = i * chunk_size;
//         size_t end = (i == num_threads - 1) ? xs.size() : (i + 1) * chunk_size;
        
//         futures.push_back(async(launch::async, [&xs, start, end]() {
//             sequential_merge_sort(xs, start, end);
//         }));
//     }
    
//     // Wait for all threads to complete their individual sorts
//     for (auto& f : futures) {
//         f.wait();
//     }
    
//     // Merge sorted chunks
//     size_t curr_size = chunk_size;
//     while (curr_size < xs.size()) {
//         futures.clear();
        
//         for (size_t i = 0; i < xs.size(); i += curr_size * 2) {
//             size_t start = i;
//             size_t mid = min(i + curr_size, xs.size());
//             size_t end = min(i + 2 * curr_size, xs.size());
            
//             futures.push_back(async(launch::async, [&xs, start, mid, end]() {
//                 merge(xs, start, mid, end);
//             }));
//         }
        
//         for (auto& f : futures) {
//             f.wait();
//         }
        
//         curr_size *= 2;
//     }
// }

// int threaded_main() {
//     cout << "Started parallel merge sort" << endl;
    
//     // Initialize random number generation
//     random_device rnd_device;
//     mt19937 mersenne_engine{rnd_device()};
//     uniform_real_distribution<float> dist{-1000, 1000};
    
//     auto gen = [&]() {
//         return dist(mersenne_engine);
//     };
    
//     // Create and fill vector with random numbers
//     const size_t vector_size = 1000000;
//     vector<float> random_vec(vector_size);
//     generate(begin(random_vec), end(random_vec), gen);
    
//     // Store original vector for validation
//     vector<float> sorted_vec = random_vec;
//     sort(sorted_vec.begin(), sorted_vec.end());
    
//     // Get number of available threads
//     int num_threads = thread::hardware_concurrency();
//     cout << "Starting merge sort with " << num_threads << " threads" << endl;
    
//     // Perform parallel merge sort
//     auto start = chrono::high_resolution_clock::now();
//     parallel_merge_sort(random_vec, num_threads);
//     auto end = chrono::high_resolution_clock::now();
//     auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
//     // Validate results
//     bool is_sorted = equal(random_vec.begin(), random_vec.end(), sorted_vec.begin());
//     cout << "Sort completed in " << duration.count() << "ms" << endl;
//     cout << "Sort is " << (is_sorted ? "correct" : "incorrect") << endl;
    
//     return 0;
// } 