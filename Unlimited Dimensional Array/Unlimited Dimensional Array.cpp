// Unlimited Dimensional Array.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <utility>
#include <concepts>
#include <initializer_list>
#include <iterator>
#include <cstdint>
#include <memory>

#define MAX_INFINITE_ARRAY_STACK_SIZE 256U
#define MAX_INFINITE_ARRAY_HEAP_SIZE 4096U

template<std::integral I>
constexpr I max(I i1, I i2) {
    return i1 > i2 ? i1 : i2;
}

template<std::integral I>
constexpr I min(I i1, I i2) {
    return i1 < i2 ? i1 : i2;
}

template<bool STACK_ALLOCATED, class T, size_t... Ds>
class array;

template<class T, size_t D>
class array<true, T, D> {
private:
    T data[D];
public:
    static constexpr size_t total_storage_size = sizeof(T) * D;

    static_assert(D != 0, "Cannot have a zero dimensional array on the stack");

    static_assert(
        total_storage_size < MAX_INFINITE_ARRAY_STACK_SIZE,
        "Array exceeded MAX_INFINITE_ARRAY_STACK_SIZE; reduce size of array or increase "
        "MAX_INFINITE_ARRAY_STACK_SIZE threshold."
    );

    array(std::initializer_list<T> i_list) {
        for (size_t i = 0; i < min(i_list.size(), D); i++) {
            this->data[i] = *(i_list.begin());
        }
    }

    T &operator[](const size_t n) {
        return this->data[n];
    }
};

template<class T, class U, size_t D1, size_t... Ds>
concept stack_array = std::same_as<T, array<true, U, D1, Ds...>>;

template<class T, size_t D1, size_t... Ds>
class array<true, T, D1, Ds...> {
private:
    array<true, T, Ds...> data[D1];
public:
    static constexpr size_t total_storage_size = sizeof(T) * D1 * (Ds * ...);

    static_assert(D1 != 0, "Cannot have a zero dimensional array on the stack");

    static_assert(
        total_storage_size < MAX_INFINITE_ARRAY_STACK_SIZE, 
        "Array exceeded MAX_INFINITE_ARRAY_STACK_SIZE; reduce size of array or increase "
        "MAX_INFINITE_ARRAY_STACK_SIZE threshold."
    );

    template<stack_array<T, Ds...>... SA>
    array(SA... sa) : data{sa...} {}

    array<true, T, Ds...> &operator[](const size_t n) {
        return this->data[n];
    }
};

constexpr size_t storage_size_test_1 = array<true, int, 3, 3, 3>::total_storage_size;
constexpr size_t storage_size_test_2 = array<true, int, 3>::total_storage_size;

template<class T, size_t D>
class array<false, T, D> {
private:
    std::shared_ptr<T> data;
public:
    static constexpr size_t total_storage_size = sizeof(T) * D;

    static_assert(
        total_storage_size < MAX_INFINITE_ARRAY_HEAP_SIZE,
        "Array exceeded MAX_INFINITE_ARRAY_HEAP_SIZE; reduce size of array or increase "
        "MAX_INFINITE_ARRAY_HEAP_SIZE threshold."
    );

    operator T &() {
        return *this->data;
    }

    T &operator[](const size_t n) {
        return this->data[n];
    }
};

template<class T, size_t D1, size_t... Ds>
class array<false, T, D1, Ds...> {
private:
    std::shared_ptr<array<false, T, Ds...>> data;
public:
    static constexpr size_t total_storage_size = sizeof(T) * D1 * (Ds * ...);

    static_assert(
        total_storage_size < MAX_INFINITE_ARRAY_HEAP_SIZE,
        "Array exceeded MAX_INFINITE_ARRAY_HEAP_SIZE; reduce size of array or increase "
        "MAX_INFINITE_ARRAY_HEAP_SIZE threshold."
    );

    array<false, T, Ds...> &operator[](const size_t n) {
        return this->data[n];
    }
};

int main(int argc, const char *argv[]) {
    array<true, int, 1, 2, 3> stack_1;

    array<true, int, 1> stack_2 = { 1, 2, 3, 4, 5 };

    std::cout << " -- Stack -- \n";

    for (size_t i = 0; i < 1; i++) {
        for (size_t j = 0; j < 2; j++) {
            for (size_t k = 0; k < 3; k++) {
                std::cout << stack_1[i][j][k] << "\n";
            }
        }
    }

    std::cout << " -- Heap -- \n";

    for (size_t i = 0; i < 5; i++) {
        std::cout << stack_2[i] << "\n";
    }

    array<false, int, 1, 2, 3> heap_1;

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
