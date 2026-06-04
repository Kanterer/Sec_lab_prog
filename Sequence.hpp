#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <functional>

template <class T>
class Sequence {
public:
    virtual ~Sequence() = default;

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;

    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(const T& item, int index) = 0;
    virtual Sequence<T>* Concat(const Sequence<T>& sequence) = 0;

    virtual Sequence<T>* Map(const std::function<T(const T&)>& function) const = 0;
    virtual Sequence<T>* Where(const std::function<bool(const T&)>& predicate) const = 0;
    virtual T Reduce(const std::function<T(const T&, const T&)>& function, const T& startValue) const = 0;

    virtual Sequence<T>* Clone() const = 0;
};

#endif
