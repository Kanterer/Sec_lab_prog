#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include "Exceptions.hpp"

#include <algorithm>
#include <string>
#include <utility>

template <class T>
class DynamicArray {
private:
    T* data;
    int size;

    void CheckIndex(int index) const {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange("DynamicArray: index is out of range: " + std::to_string(index));
        }
    }

public:
    DynamicArray()
        : data(nullptr), size(0) {}

    explicit DynamicArray(int newSize)
        : data(nullptr), size(newSize) {
        if (newSize < 0) {
            throw NegativeSizeException("DynamicArray: size cannot be negative");
        }
        if (newSize > 0) {
            data = new T[newSize];
        }
    }

    DynamicArray(const T* items, int count)
        : DynamicArray(count) {
        if (count > 0 && items == nullptr) {
            throw std::invalid_argument("DynamicArray: items pointer is null");
        }
        for (int i = 0; i < count; ++i) {
            data[i] = items[i];
        }
    }

    DynamicArray(const DynamicArray<T>& other)
        : DynamicArray(other.size) {
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this == &other) {
            return *this;
        }
        DynamicArray<T> copy(other);
        Swap(copy);
        return *this;
    }

    ~DynamicArray() {
        delete[] data;
    }

    void Swap(DynamicArray<T>& other) noexcept {
        std::swap(data, other.data);
        std::swap(size, other.size);
    }

    int GetSize() const {
        return size;
    }

    T Get(int index) const {
        CheckIndex(index);
        return data[index];
    }

    void Set(int index, const T& value) {
        CheckIndex(index);
        data[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw NegativeSizeException("DynamicArray: size cannot be negative");
        }

        T* newData = nullptr;
        if (newSize > 0) {
            newData = new T[newSize];
            const int countToCopy = std::min(size, newSize);
            for (int i = 0; i < countToCopy; ++i) {
                newData[i] = data[i];
            }
        }

        delete[] data;
        data = newData;
        size = newSize;
    }

    T& operator[](int index) {
        CheckIndex(index);
        return data[index];
    }

    const T& operator[](int index) const {
        CheckIndex(index);
        return data[index];
    }
};

#endif
