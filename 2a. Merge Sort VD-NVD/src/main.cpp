#include <iostream>
#include <vector>
#include "include/mergesort.hpp"

using namespace std;
using namespace mergesort;

int main() {
    vector<int> vec = {5, 4, 3, 2, 1};
    merge_sort(vec, 0, vec.size() - 1);

    cout << "Unsorted vector: ";
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;


    cout << "Sorted vector: ";
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;

    return 0;
}