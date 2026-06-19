#include "Tests.hpp"
#include "ArraySequence.hpp"
#include "BitSequence.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "ListSequence.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {
    int testsRun = 0;

    template <class Expected, class Actual>
    void ExpectEq(const Expected& expected, const Actual& actual) {
        ++testsRun;
        assert(expected == actual);
    }

    void ExpectTrue(bool condition) {
        ++testsRun;
        assert(condition);
    }

    int DoubleValue(const int& value) {
        return value * 2;
    }

    int AddIndex(const int& value, int index) {
        return value + index;
    }

    bool IsEven(const int& value) {
        return value % 2 == 0;
    }

    int SumReducer(const int& accumulator, const int& value) {
        return accumulator + value;
    }

    bool GreaterThanThree(const int& value) {
        return value > 3;
    }

    bool GreaterThanHundred(const int& value) {
        return value > 100;
    }

    std::string BitSequenceToString(const BitSequence& bits) {
        std::string result;

        for (int i = 0; i < bits.GetLength(); ++i) {
            result += bits.GetBit(i).ToInt() == 0 ? '0' : '1';
        }

        return result;
    }

    void TestDynamicArray() {
        int source[] = {1, 2, 3};
        DynamicArray<int> array(source, 3);

        ExpectEq(3, array.GetSize());
        ExpectEq(2, array.Get(1));

        array.Set(1, 10);
        ExpectEq(10, array.Get(1));

        array.Resize(5);
        ExpectEq(5, array.GetSize());
        ExpectEq(1, array.Get(0));
        ExpectEq(10, array.Get(1));
        ExpectEq(3, array.Get(2));

        array.Resize(2);
        ExpectEq(2, array.GetSize());
        ExpectEq(10, array.Get(1));

        bool indexExceptionWasThrown = false;
        try {
            array.Get(2);
        } catch (const IndexOutOfRangeException&) {
            indexExceptionWasThrown = true;
        }
        ExpectTrue(indexExceptionWasThrown);

        bool invalidArgumentWasThrown = false;
        try {
            DynamicArray<int> bad(-1);
        } catch (const InvalidArgumentException&) {
            invalidArgumentWasThrown = true;
        }
        ExpectTrue(invalidArgumentWasThrown);
    }

    void TestLinkedList() {
        int source[] = {2, 3, 4};
        LinkedList<int> list(source, 3);

        list.Prepend(1);
        list.Append(5);
        list.InsertAt(100, 2);

        ExpectEq(6, list.GetLength());
        ExpectEq(1, list.GetFirst());
        ExpectEq(5, list.GetLast());
        ExpectEq(100, list.Get(2));

        LinkedList<int>* sub = list.GetSubList(1, 3);

        ExpectEq(3, sub->GetLength());
        ExpectEq(2, sub->Get(0));
        ExpectEq(3, sub->Get(2));

        delete sub;

        LinkedList<int> other;
        other.Append(7);
        other.Append(8);

        LinkedList<int>* concatenated = list.Concat(&other);

        ExpectEq(8, concatenated->GetLength());
        ExpectEq(7, concatenated->Get(6));
        ExpectEq(8, concatenated->Get(7));

        delete concatenated;

        bool getExceptionWasThrown = false;
        try {
            list.Get(-1);
        } catch (const IndexOutOfRangeException&) {
            getExceptionWasThrown = true;
        }
        ExpectTrue(getExceptionWasThrown);

        bool insertExceptionWasThrown = false;
        try {
            list.InsertAt(0, 100);
        } catch (const IndexOutOfRangeException&) {
            insertExceptionWasThrown = true;
        }
        ExpectTrue(insertExceptionWasThrown);
    }

    void TestMutableArraySequence() {
        int source[] = {1, 2, 3};
        MutableArraySequence<int> sequence(source, 3);

        MutableArraySequence<int>* returned = sequence.Append(4);

        ExpectTrue(returned == &sequence);
        ExpectEq(4, sequence.GetLength());
        ExpectEq(4, sequence.GetLast());

        sequence.Prepend(0);
        sequence.InsertAt(100, 2);

        ExpectEq(0, sequence.GetFirst());
        ExpectEq(100, sequence.Get(2));

        MutableArraySequence<int>* sub = sequence.GetSubsequence(1, 3);

        ExpectEq(3, sub->GetLength());
        ExpectEq(1, sub->Get(0));
        ExpectEq(2, sub->Get(2));

        delete sub;
    }

    void TestImmutableArraySequence() {
        int source[] = {1, 2, 3};
        ImmutableArraySequence<int> original(source, 3);

        ImmutableArraySequence<int>* changed = original.Append(4);

        ExpectTrue(changed != &original);
        ExpectEq(3, original.GetLength());
        ExpectEq(4, changed->GetLength());
        ExpectEq(4, changed->GetLast());

        delete changed;

        ImmutableArraySequence<int>* prepended = original.Prepend(0);

        ExpectEq(1, original.GetFirst());
        ExpectEq(0, prepended->GetFirst());

        delete prepended;
    }

    void TestListSequences() {
        int source[] = {1, 2, 3};

        MutableListSequence<int> mutableList(source, 3);

        mutableList.Append(4);
        mutableList.Prepend(0);
        mutableList.InsertAt(100, 2);

        ExpectEq(6, mutableList.GetLength());
        ExpectEq(100, mutableList.Get(2));

        ImmutableListSequence<int> immutableList(source, 3);
        ImmutableListSequence<int>* changed = immutableList.Append(4);

        ExpectEq(3, immutableList.GetLength());
        ExpectEq(4, changed->GetLength());
        ExpectEq(4, changed->GetLast());

        delete changed;
    }

    void TestMapReduceOptionSlice() {
        int source[] = {1, 2, 3, 4, 5};
        MutableArraySequence<int> sequence(source, 5);

        MutableArraySequence<int>* mapped = sequence.Map(DoubleValue);

        ExpectEq(2, mapped->Get(0));
        ExpectEq(10, mapped->Get(4));

        delete mapped;

        MutableArraySequence<int>* indexed = sequence.MapIndexed(AddIndex);

        ExpectEq(1, indexed->Get(0));
        ExpectEq(7, indexed->Get(3));

        delete indexed;

        MutableArraySequence<int>* even = sequence.Where(IsEven);

        ExpectEq(2, even->GetLength());
        ExpectEq(2, even->Get(0));
        ExpectEq(4, even->Get(1));

        delete even;

        int sum = sequence.Reduce<int>(SumReducer, 0);

        ExpectEq(15, sum);

        Option<int> found = sequence.TryGetFirst(GreaterThanThree);

        ExpectTrue(found.HasValue());
        ExpectEq(4, found.Value());

        Option<int> notFound = sequence.TryGetLast(GreaterThanHundred);

        ExpectTrue(notFound.IsNone());

        int replacementItems[] = {9, 10};
        MutableArraySequence<int> replacement(replacementItems, 2);

        MutableArraySequence<int>* sliced = sequence.Slice(1, 2, &replacement);

        ExpectEq(5, sliced->GetLength());
        ExpectEq(1, sliced->Get(0));
        ExpectEq(9, sliced->Get(1));
        ExpectEq(10, sliced->Get(2));
        ExpectEq(4, sliced->Get(3));

        delete sliced;
    }

    void TestBitSequence() {
        BitSequence a("1010");
        BitSequence b("1100");

        ExpectEq(std::string("1000"), BitSequenceToString(a.And(b)));
        ExpectEq(std::string("1110"), BitSequenceToString(a.Or(b)));
        ExpectEq(std::string("0110"), BitSequenceToString(a.Xor(b)));
        ExpectEq(std::string("0101"), BitSequenceToString(a.Not()));

        a.SetBit(1, Bit(true));
        ExpectEq(std::string("1110"), BitSequenceToString(a));

        a.ToggleBit(0);
        ExpectEq(std::string("0110"), BitSequenceToString(a));

        bool invalidStringWasThrown = false;
        try {
            BitSequence bad("102");
        } catch (const InvalidArgumentException&) {
            invalidStringWasThrown = true;
        }
        ExpectTrue(invalidStringWasThrown);

        bool unequalLengthWasThrown = false;
        try {
            a.And(BitSequence("1"));
        } catch (const InvalidArgumentException&) {
            unequalLengthWasThrown = true;
        }
        ExpectTrue(unequalLengthWasThrown);
    }
}

void RunAllTests() {
    testsRun = 0;

    TestDynamicArray();
    TestLinkedList();
    TestMutableArraySequence();
    TestImmutableArraySequence();
    TestListSequences();
    TestMapReduceOptionSlice();
    TestBitSequence();

    std::cout << "All strict tests passed: " << testsRun << " checks." << std::endl;
}
