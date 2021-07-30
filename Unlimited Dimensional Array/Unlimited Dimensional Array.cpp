// Unlimited Dimensional Array.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <utility>

#define MAX_STACK_SIZE 256U
#define MAX_HEAP_SIZE 4096U

template<class T, size_t... Ds>
class array;

template<class T>
class array<T> {
private:
    T value;
public:
    array<T> &operator=(const T &value) {
        this->value = value;

        return *this;
    }

    array<T> &operator=(T &&value) {
        this->value = std::move(value);

        return *this;
    }

    operator T &() {
        return this->value;
    }
};

template<class T, size_t D>
class array<T, D> {
private:
    T data[D];
public:
    T &operator[](const size_t n) {
        return this->data[n];
    }
};

template<class T, size_t D1, size_t... Ds>
class array<T, D1, Ds...> {
private:
    array<T, Ds...> data[D1];
public:
    static constexpr size_t total_storage_size = sizeof(T) * D1 * (Ds * ...);

    static_assert(
        total_storage_size < MAX_STACK_SIZE, 
        "Array exceeded MAX_STACK_SIZE; reduce size of array or increase "
        "MAX_STACK_SIZE threshold."
    );

    array<T, Ds...> operator[](const size_t n) const {
        return this->data[n];
    }
};

constexpr size_t storage_size_test_1 = array<int, 3, 3, 3>::total_storage_size;

int main(int argc, const char *argv[]) {

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
