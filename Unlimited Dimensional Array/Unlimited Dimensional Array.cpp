// Unlimited Dimensional Array.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <variant>
#include <memory>
#include <array>
#include <stdexcept>
#include <type_traits>
#include <algorithm>
#include <string>

#define MAX_STACK_STORAGE_SIZE 1024U
#define MAX_HEAP_STORAGE_SIZE 4096U

#if MAX_STACK_STORAGE_SIZE >= MAX_HEAP_STORAGE_SIZE
#error MAX_HEAP_STORAGE_SIZE must be greater than MAX_STORAGE_SIZE
#endif // MAX_STACK_STORAGE_SIZE >= MAX_HEAP_STORAGE_SIZE

template<size_t N1, size_t... Ns>
struct multiply_accumulate {
    static constexpr size_t value = N1 * multiply_accumulate<Ns...>::value;
};

template<size_t N>
struct multiply_accumulate<N> {
    static constexpr size_t value = N;
};

class potential_out_of_memory_error : public std::runtime_error {
public:
    potential_out_of_memory_error(size_t requested) : 
        std::runtime_error(
            "Storage size requested breaches both MAX_STACK_STORAGE_SIZE and "
            "MAX_HEAP_STORAGE_SIZE thresholds. Increase these thresholds to "
            "prevent this error. Bytes requested: " + std::to_string(requested)
        ) {}
};

template<class T, size_t... DIMENSION_SIZES>
class unlimited_dimensional_array {
public:
    static constexpr size_t storage_size = multiply_accumulate<DIMENSION_SIZES...>::value * sizeof(T);
private:
    std::variant<
        std::array<T, storage_size>,
        std::shared_ptr<T>
    > storage;
public:
    constexpr unlimited_dimensional_array(const T &init = T()) {
        if constexpr (storage_size < MAX_STACK_STORAGE_SIZE) {
            std::array<T, storage_size> &ref_arr =
                std::get<0>(this->storage);

            ref_arr.fill(init);
        } else if (storage_size < MAX_HEAP_STORAGE_SIZE) {
            std::shared_ptr<T> &sp_t = std::get<1>(this->storage);

            sp_t = std::make_shared<T>(new T[storage_size]);
        } else {
            throw potential_out_of_memory_error(storage_size);
        }
    }
};

constexpr size_t test = multiply_accumulate<3, 3, 3>::value;

int main(int argc, const char *argv[]) {
    unlimited_dimensional_array<int, 2, 2> udai2x2;

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
