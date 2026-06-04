#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include "Exceptions.hpp"

#include <string>
#include <utility>

template <class T>
class LinkedList {
private:
    struct Node {
        T value;
        Node* next;

        explicit Node(const T& value)
            : value(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int length;

    void CheckIndex(int index) const {
        if (index < 0 || index >= length) {
            throw IndexOutOfRange("LinkedList: index is out of range: " + std::to_string(index));
        }
    }

    Node* GetNode(int index) const {
        CheckIndex(index);
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current;
    }

public:
    LinkedList()
        : head(nullptr), tail(nullptr), length(0) {}

    LinkedList(const T* items, int count)
        : LinkedList() {
        if (count < 0) {
            throw NegativeSizeException("LinkedList: count cannot be negative");
        }
        if (count > 0 && items == nullptr) {
            throw std::invalid_argument("LinkedList: items pointer is null");
        }
        for (int i = 0; i < count; ++i) {
            Append(items[i]);
        }
    }

    LinkedList(const LinkedList<T>& other)
        : LinkedList() {
        Node* current = other.head;
        while (current != nullptr) {
            Append(current->value);
            current = current->next;
        }
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this == &other) {
            return *this;
        }
        LinkedList<T> copy(other);
        Swap(copy);
        return *this;
    }

    ~LinkedList() {
        Clear();
    }

    void Swap(LinkedList<T>& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(length, other.length);
    }

    void Clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    int GetLength() const {
        return length;
    }

    T GetFirst() const {
        if (length == 0) {
            throw IndexOutOfRange("LinkedList: list is empty");
        }
        return head->value;
    }

    T GetLast() const {
        if (length == 0) {
            throw IndexOutOfRange("LinkedList: list is empty");
        }
        return tail->value;
    }

    T Get(int index) const {
        return GetNode(index)->value;
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex < 0 || startIndex > endIndex || endIndex >= length) {
            throw IndexOutOfRange("LinkedList: invalid sublist indexes");
        }

        LinkedList<T>* result = new LinkedList<T>();
        for (int i = startIndex; i <= endIndex; ++i) {
            result->Append(Get(i));
        }
        return result;
    }

    void Append(const T& item) {
        Node* node = new Node(item);
        if (length == 0) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        ++length;
    }

    void Prepend(const T& item) {
        Node* node = new Node(item);
        if (length == 0) {
            head = node;
            tail = node;
        } else {
            node->next = head;
            head = node;
        }
        ++length;
    }

    void InsertAt(const T& item, int index) {
        if (index < 0 || index > length) {
            throw IndexOutOfRange("LinkedList: insert index is out of range");
        }

        if (index == 0) {
            Prepend(item);
            return;
        }
        if (index == length) {
            Append(item);
            return;
        }

        Node* previous = GetNode(index - 1);
        Node* node = new Node(item);
        node->next = previous->next;
        previous->next = node;
        ++length;
    }

    LinkedList<T>* Concat(const LinkedList<T>& list) {
        const int sourceLength = list.GetLength();
        for (int i = 0; i < sourceLength; ++i) {
            Append(list.Get(i));
        }
        return this;
    }
};

#endif
