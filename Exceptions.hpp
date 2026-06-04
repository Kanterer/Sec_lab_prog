#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

class IndexOutOfRange : public std::out_of_range {
public:
    explicit IndexOutOfRange(const std::string& message)
        : std::out_of_range(message) {}
};

class NegativeSizeException : public std::invalid_argument {
public:
    explicit NegativeSizeException(const std::string& message)
        : std::invalid_argument(message) {}
};

#endif
