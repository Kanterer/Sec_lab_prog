#ifndef ARRAY_SEQUENCE_HPP
#define ARRAY_SEQUENCE_HPP
#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include <functional>
#include <string>

template <class T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* items;

    void AppendInternal(const T& item) {
        const int size = items->GetSize();
        items->Resize(size + 1);
        items->Set(size, item);
    }

    void PrependInternal(const T& item) {
        const int size = items->GetSize();
        items->Resize(size + 1);
        for (int i = size; i > 0; --i) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(0, item);
    }

    void InsertAtInternal(const T& item, int index) {
        const int size = items->GetSize();
        if (index < 0 || index > size) {
            throw IndexOutOfRange("ArraySequence: insert index is out of range");
        }
        if (index == size) {
            AppendInternal(item);
            return;
        }

        items->Resize(size + 1);
        for (int i = size; i > index; --i) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(index, item);
    }

    virtual ArraySequence<T>* InstanceForChange() {
        return this;
    }

    virtual ArraySequence<T>* CreateEmpty() const {
        return new ArraySequence<T>();
    }

public:
    ArraySequence()
        : items(new DynamicArray<T>()) {}

    ArraySequence(const T* sourceItems, int count)
        : items(new DynamicArray<T>(sourceItems, count)) {}

    explicit ArraySequence(const DynamicArray<T>& dynamicArray)
        : items(new DynamicArray<T>(dynamicArray)) {}

    ArraySequence(const ArraySequence<T>& other)
        : items(new DynamicArray<T>(*other.items)) {}

    ArraySequence<T>& operator=(const ArraySequence<T>& other) {
        if (this == &other) {
            return *this;
        }
        delete items;
        items = new DynamicArray<T>(*other.items);
        return *this;
    }

    ~ArraySequence() override {
        delete items;
    }

    T GetFirst() const override {
        if (items->GetSize() == 0) {
            throw IndexOutOfRange("ArraySequence: sequence is empty");
        }
        return items->Get(0);
    }

    T GetLast() const override {
        const int size = items->GetSize();
        if (size == 0) {
            throw IndexOutOfRange("ArraySequence: sequence is empty");
        }
        return items->Get(size - 1);
    }

    T Get(int index) const override {
        return items->Get(index);
    }

    int GetLength() const override {
        return items->GetSize();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        const int size = items->GetSize();
        if (startIndex < 0 || endIndex < 0 || startIndex > endIndex || endIndex >= size) {
            throw IndexOutOfRange("ArraySequence: invalid subsequence indexes");
        }

        ArraySequence<T>* result = CreateEmpty();
        for (int i = startIndex; i <= endIndex; ++i) {
            result->AppendInternal(items->Get(i));
        }
        return result;
    }

    Sequence<T>* Append(const T& item) override {
        ArraySequence<T>* result = InstanceForChange();
        result->AppendInternal(item);
        return result;
    }

    Sequence<T>* Prepend(const T& item) override {
        ArraySequence<T>* result = InstanceForChange();
        result->PrependInternal(item);
        return result;
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        ArraySequence<T>* result = InstanceForChange();
        result->InsertAtInternal(item, index);
        return result;
    }

    Sequence<T>* Concat(const Sequence<T>& sequence) override {
        ArraySequence<T>* result = InstanceForChange();
        const int sourceLength = sequence.GetLength();
        for (int i = 0; i < sourceLength; ++i) {
            result->AppendInternal(sequence.Get(i));
        }
        return result;
    }

    Sequence<T>* Map(const std::function<T(const T&)>& function) const override {
        ArraySequence<T>* result = CreateEmpty();
        for (int i = 0; i < GetLength(); ++i) {
            result->AppendInternal(function(Get(i)));
        }
        return result;
    }

    Sequence<T>* Where(const std::function<bool(const T&)>& predicate) const override {
        ArraySequence<T>* result = CreateEmpty();
        for (int i = 0; i < GetLength(); ++i) {
            T value = Get(i);
            if (predicate(value)) {
                result->AppendInternal(value);
            }
        }
        return result;
    }

    T Reduce(const std::function<T(const T&, const T&)>& function, const T& startValue) const override {
        T result = startValue;
        for (int i = 0; i < GetLength(); ++i) {
            result = function(result, Get(i));
        }
        return result;
    }

    Sequence<T>* Clone() const override {
        return new ArraySequence<T>(*this);
    }
};

template <class T>
class MutableArraySequence : public ArraySequence<T> {
protected:
    ArraySequence<T>* InstanceForChange() override {
        return this;
    }

    ArraySequence<T>* CreateEmpty() const override {
        return new MutableArraySequence<T>();
    }

public:
    MutableArraySequence()
        : ArraySequence<T>() {}

    MutableArraySequence(const T* items, int count)
        : ArraySequence<T>(items, count) {}

    explicit MutableArraySequence(const DynamicArray<T>& array)
        : ArraySequence<T>(array) {}

    MutableArraySequence(const MutableArraySequence<T>& other)
        : ArraySequence<T>(other) {}

    Sequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }
};

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
protected:
    ArraySequence<T>* InstanceForChange() override {
        return new ImmutableArraySequence<T>(*this);
    }

    ArraySequence<T>* CreateEmpty() const override {
        return new ImmutableArraySequence<T>();
    }

public:
    ImmutableArraySequence()
        : ArraySequence<T>() {}

    ImmutableArraySequence(const T* items, int count)
        : ArraySequence<T>(items, count) {}

    explicit ImmutableArraySequence(const DynamicArray<T>& array)
        : ArraySequence<T>(array) {}

    ImmutableArraySequence(const ImmutableArraySequence<T>& other)
        : ArraySequence<T>(other) {}

    Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }
};

#endif
