#include "Test.hpp"

#include "ArraySequence.hpp"
#include "BitSequence.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "ListSequence.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>

static void TestDynamicArray() {
    int values[] = {1, 2, 3};
    DynamicArray<int> array(values, 3);

    assert(array.GetSize() == 3);
    assert(array.Get(0) == 1);
    assert(array.Get(1) == 2);
    assert(array.Get(2) == 3);

    array.Set(1, 20);
    assert(array.Get(1) == 20);

    array.Resize(5);
    assert(array.GetSize() == 5);
    assert(array.Get(0) == 1);
    assert(array.Get(1) == 20);
    assert(array.Get(2) == 3);

    array.Resize(2);
    assert(array.GetSize() == 2);
    assert(array.Get(0) == 1);
    assert(array.Get(1) == 20);

    bool exceptionThrown = false;
    try {
        array.Get(100);
    } catch (const IndexOutOfRange&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
}

static void TestLinkedList() {
    int values[] = {1, 2, 3};
    LinkedList<int> list(values, 3);

    assert(list.GetLength() == 3);
    assert(list.GetFirst() == 1);
    assert(list.GetLast() == 3);
    assert(list.Get(1) == 2);

    list.Append(4);
    assert(list.GetLength() == 4);
    assert(list.GetLast() == 4);

    list.Prepend(0);
    assert(list.GetLength() == 5);
    assert(list.GetFirst() == 0);

    list.InsertAt(99, 2);
    assert(list.GetLength() == 6);
    assert(list.Get(2) == 99);

    LinkedList<int>* subList = list.GetSubList(1, 3);
    assert(subList->GetLength() == 3);
    assert(subList->Get(0) == 1);
    assert(subList->Get(1) == 99);
    assert(subList->Get(2) == 2);
    delete subList;
}

static void TestMutableArraySequence() {
    int values[] = {1, 2, 3};
    MutableArraySequence<int> sequence(values, 3);

    assert(sequence.GetLength() == 3);
    assert(sequence.GetFirst() == 1);
    assert(sequence.GetLast() == 3);
    assert(sequence.Get(1) == 2);

    Sequence<int>* afterAppend = sequence.Append(4);
    assert(afterAppend == &sequence);
    assert(sequence.GetLength() == 4);
    assert(sequence.GetLast() == 4);

    Sequence<int>* afterPrepend = sequence.Prepend(0);
    assert(afterPrepend == &sequence);
    assert(sequence.GetFirst() == 0);

    Sequence<int>* afterInsert = sequence.InsertAt(99, 2);
    assert(afterInsert == &sequence);
    assert(sequence.Get(2) == 99);

    Sequence<int>* subSequence = sequence.GetSubsequence(1, 3);
    assert(subSequence->GetLength() == 3);
    assert(subSequence->Get(0) == 1);
    assert(subSequence->Get(1) == 99);
    assert(subSequence->Get(2) == 2);
    delete subSequence;
}

static void TestImmutableArraySequence() {
    int values[] = {1, 2, 3};
    ImmutableArraySequence<int> sequence(values, 3);

    Sequence<int>* newSequence = sequence.Append(4);

    assert(sequence.GetLength() == 3);
    assert(sequence.GetLast() == 3);
    assert(newSequence->GetLength() == 4);
    assert(newSequence->GetLast() == 4);

    delete newSequence;
}

static void TestMutableListSequence() {
    int values[] = {1, 2, 3};
    MutableListSequence<int> sequence(values, 3);

    assert(sequence.GetLength() == 3);
    assert(sequence.GetFirst() == 1);
    assert(sequence.GetLast() == 3);
    assert(sequence.Get(1) == 2);

    Sequence<int>* afterAppend = sequence.Append(4);
    assert(afterAppend == &sequence);
    assert(sequence.GetLength() == 4);
    assert(sequence.GetLast() == 4);

    Sequence<int>* afterPrepend = sequence.Prepend(0);
    assert(afterPrepend == &sequence);
    assert(sequence.GetFirst() == 0);

    Sequence<int>* afterInsert = sequence.InsertAt(99, 2);
    assert(afterInsert == &sequence);
    assert(sequence.Get(2) == 99);
}

static void TestImmutableListSequence() {
    int values[] = {1, 2, 3};
    ImmutableListSequence<int> sequence(values, 3);

    Sequence<int>* newSequence = sequence.Append(4);

    assert(sequence.GetLength() == 3);
    assert(sequence.GetLast() == 3);
    assert(newSequence->GetLength() == 4);
    assert(newSequence->GetLast() == 4);

    delete newSequence;
}

static void TestConcat() {
    int firstValues[] = {1, 2, 3};
    int secondValues[] = {4, 5};

    MutableArraySequence<int> first(firstValues, 3);
    MutableListSequence<int> second(secondValues, 2);

    first.Concat(second);

    assert(first.GetLength() == 5);
    assert(first.Get(0) == 1);
    assert(first.Get(1) == 2);
    assert(first.Get(2) == 3);
    assert(first.Get(3) == 4);
    assert(first.Get(4) == 5);
}

static void TestMapWhereReduce() {
    int values[] = {1, 2, 3, 4};
    MutableArraySequence<int> sequence(values, 4);

    Sequence<int>* squares = sequence.Map([](const int& value) {
        return value * value;
    });
    assert(squares->GetLength() == 4);
    assert(squares->Get(0) == 1);
    assert(squares->Get(1) == 4);
    assert(squares->Get(2) == 9);
    assert(squares->Get(3) == 16);
    delete squares;

    Sequence<int>* even = sequence.Where([](const int& value) {
        return value % 2 == 0;
    });
    assert(even->GetLength() == 2);
    assert(even->Get(0) == 2);
    assert(even->Get(1) == 4);
    delete even;

    int sum = sequence.Reduce([](const int& accumulator, const int& value) {
        return accumulator + value;
    }, 0);
    assert(sum == 10);
}

static void TestBitSequence() {
    int firstBits[] = {1, 0, 1, 1};
    int secondBits[] = {1, 1, 0, 1};

    BitSequence first(firstBits, 4);
    BitSequence second(secondBits, 4);

    assert(first.GetLength() == 4);
    assert(first.ToString() == "1011");

    BitSequence* andResult = first.And(second);
    assert(andResult->ToString() == "1001");
    delete andResult;

    BitSequence* orResult = first.Or(second);
    assert(orResult->ToString() == "1111");
    delete orResult;

    BitSequence* xorResult = first.Xor(second);
    assert(xorResult->ToString() == "0110");
    delete xorResult;

    BitSequence* notResult = first.Not();
    assert(notResult->ToString() == "0100");
    delete notResult;
}

static void TestExceptions() {
    MutableArraySequence<int> arraySequence;
    MutableListSequence<int> listSequence;

    bool arrayException = false;
    bool listException = false;

    try {
        arraySequence.GetFirst();
    } catch (const IndexOutOfRange&) {
        arrayException = true;
    }

    try {
        listSequence.GetLast();
    } catch (const IndexOutOfRange&) {
        listException = true;
    }

    assert(arrayException);
    assert(listException);
}

void RunAllTests() {
    TestDynamicArray();
    TestLinkedList();
    TestMutableArraySequence();
    TestImmutableArraySequence();
    TestMutableListSequence();
    TestImmutableListSequence();
    TestConcat();
    TestMapWhereReduce();
    TestBitSequence();
    TestExceptions();

    std::cout << "All tests passed successfully!" << std::endl;
}
