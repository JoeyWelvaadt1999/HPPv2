#include <iostream>
#include <vector>
#include <random> 
#include <algorithm>
#include <chrono>
#include <valarray>

using namespace std;

using namespace chrono;
/** START UTILS FUNCTIONS */
int get_digit(int number, int place) {
    // If place value is higher than the number itself
    return (number / place) % 10;
}

int get_largest_decimal_amount (vector<float> numbers) {
    int answer = 0;
    for (float number : numbers) {
        string stringNum =  to_string(number);
        int decimalPos = stringNum.find(".");
        string newString = stringNum.substr(decimalPos);
        
        while (!newString.empty() && newString.back() == '0') {
            newString.pop_back();
        }

        int tmp = newString.length() - 1;
        if(tmp > answer) {
            answer = tmp;
        }
    }
    return answer;
}

/** END UTILS FUNCTIONS */

void bucket_sort(vector<int>& list) {
    int place = 1;

    vector<vector<int>> buckets(10);

    int minimum = *min_element(begin(list), end(list));  

    if(minimum < 0) {
        valarray<int> list_val(list.data(), list.size());
        list_val += -minimum;
        list.assign(begin(list_val), end(list_val));
    }

    int places = to_string(*max_element(begin(list), end(list))).length() + 1;
    // 9i + 1
    for (int i = 0; i < places; i++)
    {
        
        // Distribution Pass
        for (size_t j = 0; j < list.size(); j++) 
        {
            int digit = get_digit(list[j], place); // 2 arithmetic operations, 1 lookup
            // cout << list[j] << ", " << digit << endl;

            buckets[digit].push_back(list[j]); // 1 lookup, 1 write
            // cout << "help me " << endl;
        }

       
        // cout << " " << endl;
        int index = 0;
        // Gathering Pass
        for (size_t k = 0; k < buckets.size(); k++)
        {
            for (size_t l = 0; l < list.size(); l++) 
            {
                if(l < buckets[k].size()) {
                    list[index++] = buckets[k][l]; // 2 lookup, 1 write, 1 increment
                }
            }
            buckets[k].clear();
            
        }    
        place *= 10;  // 1 arithmetic 
    }
    

    if(minimum < 0) {
        valarray<int> list_val(list.data(), list.size());
        list_val += minimum;
        list.assign(begin(list_val), end(list_val));
    }
}

void bucket_sort(vector<float>& numbers) {
    int multiplier = pow(10, get_largest_decimal_amount(numbers));
    valarray<float> numbers_val(numbers.data(), numbers.size());

    numbers_val *= multiplier;
    vector<int> int_numbers(numbers_val.size());
    int_numbers.assign(begin(numbers_val), end(numbers_val));

   
    bucket_sort(int_numbers);
    
    transform(begin(int_numbers), end(int_numbers), begin(numbers), 
                [multiplier](int n) { return static_cast<float>(n) / multiplier; });
    
}

int main() {
    vector<int> ns = { 10, 25, 50, 75, 100, 250, 500, 750, 1000, 2500, 5000, 7500, 10000 };

    for (size_t i = 0; i < 20; i++) {
        for (int n : ns) {
            std::random_device rnd_device;
            // Specify the engine and distribution.
            std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
            std::uniform_real_distribution<float> dist {-1000, 1000};
            
            auto gen = [&](){
                return dist(mersenne_engine);
            };

            vector<float> list(n);
            generate(begin(list), end(list), gen);

            // Start timing
            steady_clock::time_point begin = steady_clock::now();
            // Do the thing!
            bucket_sort(list);
            // Stop timing
            steady_clock::time_point end = steady_clock::now();
            // Calculate and print time
            int time_ms = duration_cast<microseconds>(end - begin).count();

            cout << n << ";" << time_ms << endl;
        }
    }
    return 0;
}
