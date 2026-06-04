#ifndef BIT_HPP
#define BIT_HPP
#include <iostream>
#include <stdexcept>

class Bit {
private:
    bool value;

public:
    Bit()
        : value(false) {}

    Bit(bool value)
        : value(value) {}

    Bit(int value) {
        if (value != 0 && value != 1) {
            throw std::invalid_argument("Bit can be only 0 or 1");
        }
        this->value = (value == 1);
    }

    bool GetValue() const {
        return value;
    }

    int ToInt() const {
        return value ? 1 : 0;
    }

    Bit operator!() const {
        return Bit(!value);
    }

    Bit operator&(const Bit& other) const {
        return Bit(value && other.value);
    }

    Bit operator|(const Bit& other) const {
        return Bit(value || other.value);
    }

    Bit operator^(const Bit& other) const {
        return Bit(value != other.value);
    }

    bool operator==(const Bit& other) const {
        return value == other.value;
    }

    bool operator!=(const Bit& other) const {
        return !(*this == other);
    }
};

inline std::ostream& operator<<(std::ostream& output, const Bit& bit) {
    output << bit.ToInt();
    return output;
}

#endif
