#ifndef BIT_SEQUENCE_HPP
#define BIT_SEQUENCE_HPP

#include "ArraySequence.hpp"
#include "Bit.hpp"
#include "Exceptions.hpp"

#include <string>

class BitSequence : public ArraySequence<Bit> {
protected:
    ArraySequence<Bit>* InstanceForChange() override {
        return this;
    }

    ArraySequence<Bit>* CreateEmpty() const override {
        return new BitSequence();
    }

    void CheckSameLength(const BitSequence& other) const {
        if (GetLength() != other.GetLength()) {
            throw std::invalid_argument("BitSequence: sequences must have the same length");
        }
    }

public:
    BitSequence()
        : ArraySequence<Bit>() {}

    BitSequence(const Bit* bits, int count)
        : ArraySequence<Bit>(bits, count) {}

    BitSequence(const int* bits, int count)
        : ArraySequence<Bit>() {
        if (count < 0) {
            throw NegativeSizeException("BitSequence: count cannot be negative");
        }
        if (count > 0 && bits == nullptr) {
            throw std::invalid_argument("BitSequence: bits pointer is null");
        }
        for (int i = 0; i < count; ++i) {
            AppendInternal(Bit(bits[i]));
        }
    }

    BitSequence(const BitSequence& other)
        : ArraySequence<Bit>(other) {}

    Sequence<Bit>* Clone() const override {
        return new BitSequence(*this);
    }

    BitSequence* And(const BitSequence& other) const {
        CheckSameLength(other);
        BitSequence* result = new BitSequence();
        for (int i = 0; i < GetLength(); ++i) {
            result->AppendInternal(Get(i) & other.Get(i));
        }
        return result;
    }

    BitSequence* Or(const BitSequence& other) const {
        CheckSameLength(other);
        BitSequence* result = new BitSequence();
        for (int i = 0; i < GetLength(); ++i) {
            result->AppendInternal(Get(i) | other.Get(i));
        }
        return result;
    }

    BitSequence* Xor(const BitSequence& other) const {
        CheckSameLength(other);
        BitSequence* result = new BitSequence();
        for (int i = 0; i < GetLength(); ++i) {
            result->AppendInternal(Get(i) ^ other.Get(i));
        }
        return result;
    }

    BitSequence* Not() const {
        BitSequence* result = new BitSequence();
        for (int i = 0; i < GetLength(); ++i) {
            result->AppendInternal(!Get(i));
        }
        return result;
    }

    std::string ToString() const {
        std::string result;
        for (int i = 0; i < GetLength(); ++i) {
            result += Get(i).ToInt() == 1 ? '1' : '0';
        }
        return result;
    }
};

#endif
