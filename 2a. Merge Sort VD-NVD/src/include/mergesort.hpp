#ifndef MERGESORT_HPP
#define MERGESORT_HPP

#include <vector>

using namespace std;
namespace mergesort {
    void merge_sort(std::vector<int>& vec, int left, int right);
    void merge(std::vector<int>& vec, int left, int middle, int right);
}

#endif // MERGESORT_HPP