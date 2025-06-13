#include <iostream>
#include <vector>
#include <random> 
#include <algorithm>
#include <thread>

using namespace std;

void merge(vector<float>& xs, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<float> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++) {
        L[i] = xs[l + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = xs[m + 1 + j];
    }   

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            xs[k] = L[i];
            i++;
        } else {
            xs[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        xs[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        xs[k] = R[j];
        j++;
        k++;
    }       
    
}

void merge_sort (vector<float>& xs) {
    int unit = 1;

    while (unit <= static_cast<int>(xs.size())) {

        for (size_t head = 0; head < xs.size(); head+=unit * 2)
        {
            int left = head;
            int right = min(xs.size(), head + 2 * unit);

            int mid = head + unit;

            int p = left;
            int q = mid;

            while (p < mid && q < right) {
                if (xs[p] <= xs[q]) {
                    p++;
                } else {
                    int tmp = xs[q];                    
                    copy_backward(xs.begin() + p, xs.begin() + q, xs.begin() + q + 1);
                    xs[p] = tmp;
                    p++;
                    mid++;
                    q++;
                    
                    
                }
            }
        }
        unit *= 2;
    }
}

void parallel_merge_sort (vector<float>& xs, int num_threads) {
    if(num_threads == 1) {
        merge_sort(xs);
        return;
    }
    
    // First phase: Sort chunks in parallel
    size_t chunk_size = xs.size() / 2;
    
    // Create copies of the halves
    vector<float> left_side(xs.begin(), xs.begin() + chunk_size);
    vector<float> right_side(xs.begin() + chunk_size, xs.end());

    // Sort both halves in parallel
    thread left_thread(parallel_merge_sort, ref(left_side), num_threads / 2);
    parallel_merge_sort(right_side, num_threads - num_threads / 2);

    // Wait for left half to complete
    left_thread.join();

    // Copy sorted halves back to original vector
    copy(left_side.begin(), left_side.end(), xs.begin());
    copy(right_side.begin(), right_side.end(), xs.begin() + chunk_size);

    // Merge the sorted halves in the original vector
    merge(xs, 0, chunk_size - 1, xs.size() - 1);
}

int main () {    

    vector<int> threads = {1, 2, 4, 8};
    vector<int> sizes = {10, 25, 50, 75, 100, 250, 500, 750, 1000, 2500, 5000, 7500, 10000 };
        
    random_device rnd_device;
    mt19937 mersenne_engine {rnd_device()}; 
    uniform_real_distribution<float> dist {-1000, 1000};
        
    auto gen = [&](){
        return dist(mersenne_engine);
    };
    cout << "size; threads; time" << endl;
    for (int thread : threads) {
        for (int size : sizes) {
            // cout << "started thread: " << thread << " and size: " << size << endl;
            
            vector<float> random_vec(size);
            generate(begin(random_vec), end(random_vec), gen);

            // if(size == 10) {
            //     for (size_t i = 0; i < random_vec.size(); i++)
            //     {
            //         cout << "test: " << random_vec[i] << endl;
            //     }
            // }
            chrono::steady_clock::time_point start = chrono::steady_clock::now();
            parallel_merge_sort(random_vec, thread);
            chrono::steady_clock::time_point end = chrono::steady_clock::now();

            // if(size == 10) {
            //     for (size_t i = 0; i < random_vec.size(); i++)
            //     {
            //         cout << "validated: " << random_vec[i] << endl;
            //     }
            // }

            cout << size << "; " << thread << "; " << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;
        }
    }   
    return 0;
}