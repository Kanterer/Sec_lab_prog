#ifndef LIST_SEQUENCE_HPP
#define LIST_SEQUENCE_HPP
#include "LinkedList.hpp"
#include "Sequence.hpp"
#include <functional>

template <class T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T>* items;

    void AppendInternal(const T& item) {
        items->Append(item);
    }

    void PrependInternal(const T& item) {
        items->Prepend(item);
    }

    void InsertAtInternal(const T& item, int index) {
        items->InsertAt(item, index);
    }

    virtual ListSequence<T>* InstanceForChange() {
        return this;
    }

    virtual ListSequence<T>* CreateEmpty() const {
        return new ListSequence<T>();
    }

public:
    ListSequence()
        : items(new LinkedList<T>()) {}

    ListSequence(const T* sourceItems, int count)
        : items(new LinkedList<T>(sourceItems, count)) {}

    explicit ListSequence(const LinkedList<T>& list)
        : items(new LinkedList<T>(list)) {}

    ListSequence(const ListSequence<T>& other)
        : items(new LinkedList<T>(*other.items)) {}

    ListSequence<T>& operator=(const ListSequence<T>& other) {
        if (this == &other) {
            return *this;
        }
        delete items;
        items = new LinkedList<T>(*other.items);
        return *this;
    }

    ~ListSequence() override {
        delete items;
    }

    T GetFirst() const override {
        return items->GetFirst();
    }

    T GetLast() const override {
        return items->GetLast();
    }

    T Get(int index) const override {
        return items->Get(index);
    }

    int GetLength() const override {
        return items->GetLength();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        const int length = items->GetLength();
        if (startIndex < 0 || endIndex < 0 || startIndex > endIndex || endIndex >= length) {
            throw IndexOutOfRange("ListSequence: invalid subsequence indexes");
        }

        ListSequence<T>* result = CreateEmpty();
        for (int i = startIndex; i <= endIndex; ++i) {
            result->AppendInternal(items->Get(i));
        }
        return result;
    }

    Sequence<T>* Append(const T& item) override {
        ListSequence<T>* result = InstanceForChange();
        result->AppendInternal(item);
        return result;
    }

    Sequence<T>* Prepend(const T& item) override {
        ListSequence<T>* result = InstanceForChange();
        result->PrependInternal(item);
        return result;
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        ListSequence<T>* result = InstanceForChange();
        result->InsertAtInternal(item, index);
        return result;
    }

    Sequence<T>* Concat(const Sequence<T>& sequence) override {
        ListSequence<T>* result = InstanceForChange();
        const int sourceLength = sequence.GetLength();
        for (int i = 0; i < sourceLength; ++i) {
            result->AppendInternal(sequence.Get(i));
        }
        return result;
    }

    Sequence<T>* Map(const std::function<T(const T&)>& function) const override {
        ListSequence<T>* result = CreateEmpty();
        for (int i = 0; i < GetLength(); ++i) {
            result->AppendInternal(function(Get(i)));
        }
        return result;
    }

    Sequence<T>* Where(const std::function<bool(const T&)>& predicate) const override {
        ListSequence<T>* result = CreateEmpty();
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
        return new ListSequence<T>(*this);
    }
};

template <class T>
class MutableListSequence : public ListSequence<T> {
protected:
    ListSequence<T>* InstanceForChange() override {
        return this;
    }

    ListSequence<T>* CreateEmpty() const override {
        return new MutableListSequence<T>();
    }

public:
    MutableListSequence()
        : ListSequence<T>() {}

    MutableListSequence(const T* items, int count)
        : ListSequence<T>(items, count) {}

    explicit MutableListSequence(const LinkedList<T>& list)
        : ListSequence<T>(list) {}

    MutableListSequence(const MutableListSequence<T>& other)
        : ListSequence<T>(other) {}

    Sequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this);
    }
};

template <class T>
class ImmutableListSequence : public ListSequence<T> {
protected:
    ListSequence<T>* InstanceForChange() override {
        return new ImmutableListSequence<T>(*this);
    }

    ListSequence<T>* CreateEmpty() const override {
        return new ImmutableListSequence<T>();
    }

public:
    ImmutableListSequence()
        : ListSequence<T>() {}

    ImmutableListSequence(const T* items, int count)
        : ListSequence<T>(items, count) {}

    explicit ImmutableListSequence(const LinkedList<T>& list)
        : ListSequence<T>(list) {}

    ImmutableListSequence(const ImmutableListSequence<T>& other)
        : ListSequence<T>(other) {}

    Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }
};

#endif
