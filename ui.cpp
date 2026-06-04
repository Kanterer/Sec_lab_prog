#include "UI.hpp"

#include "ArraySequence.hpp"
#include "BitSequence.hpp"
#include "ListSequence.hpp"
#include "Sequence.hpp"

#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

static void ClearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int ReadInt(const std::string& message) {
    int value;
    while (true) {
        std::cout << message;
        std::cin >> value;

        if (std::cin.good()) {
            return value;
        }

        std::cout << "Input error. Enter integer value." << std::endl;
        ClearInput();
    }
}

static void PrintSequence(const Sequence<int>& sequence) {
    std::cout << "[ ";
    for (int i = 0; i < sequence.GetLength(); ++i) {
        std::cout << sequence.Get(i) << " ";
    }
    std::cout << "]" << std::endl;
}

static void ReplaceSequenceIfNeeded(Sequence<int>*& current, Sequence<int>* result) {
    if (result != current) {
        delete current;
        current = result;
    }
}

static void PrintMainMenu() {
    std::cout << std::endl;
    std::cout << "===== Sequence menu =====" << std::endl;
    std::cout << "1. Print sequence" << std::endl;
    std::cout << "2. Append" << std::endl;
    std::cout << "3. Prepend" << std::endl;
    std::cout << "4. Insert at index" << std::endl;
    std::cout << "5. Get by index" << std::endl;
    std::cout << "6. Get first" << std::endl;
    std::cout << "7. Get last" << std::endl;
    std::cout << "8. Get length" << std::endl;
    std::cout << "9. Get subsequence" << std::endl;
    std::cout << "10. Map: multiply all values by 2" << std::endl;
    std::cout << "11. Where: keep even values" << std::endl;
    std::cout << "12. Reduce: sum values" << std::endl;
    std::cout << "13. Use MutableArraySequence" << std::endl;
    std::cout << "14. Use MutableListSequence" << std::endl;
    std::cout << "15. BitSequence demo" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

static Sequence<int>* CopyToArraySequence(const Sequence<int>& source) {
    Sequence<int>* result = new MutableArraySequence<int>();
    for (int i = 0; i < source.GetLength(); ++i) {
        result->Append(source.Get(i));
    }
    return result;
}

static Sequence<int>* CopyToListSequence(const Sequence<int>& source) {
    Sequence<int>* result = new MutableListSequence<int>();
    for (int i = 0; i < source.GetLength(); ++i) {
        result->Append(source.Get(i));
    }
    return result;
}

static void RunBitSequenceDemo() {
    int firstBits[] = {1, 0, 1, 1};
    int secondBits[] = {1, 1, 0, 1};

    BitSequence first(firstBits, 4);
    BitSequence second(secondBits, 4);

    std::cout << "First:  " << first.ToString() << std::endl;
    std::cout << "Second: " << second.ToString() << std::endl;

    BitSequence* andResult = first.And(second);
    BitSequence* orResult = first.Or(second);
    BitSequence* xorResult = first.Xor(second);
    BitSequence* notResult = first.Not();

    std::cout << "AND:    " << andResult->ToString() << std::endl;
    std::cout << "OR:     " << orResult->ToString() << std::endl;
    std::cout << "XOR:    " << xorResult->ToString() << std::endl;
    std::cout << "NOT first: " << notResult->ToString() << std::endl;

    delete andResult;
    delete orResult;
    delete xorResult;
    delete notResult;
}

void RunConsoleInterface() {
    Sequence<int>* sequence = new MutableArraySequence<int>();
    bool running = true;

    while (running) {
        PrintMainMenu();
        int command = ReadInt("Choose command: ");

        try {
            switch (command) {
                case 1: {
                    PrintSequence(*sequence);
                    break;
                }
                case 2: {
                    int value = ReadInt("Value: ");
                    Sequence<int>* result = sequence->Append(value);
                    ReplaceSequenceIfNeeded(sequence, result);
                    break;
                }
                case 3: {
                    int value = ReadInt("Value: ");
                    Sequence<int>* result = sequence->Prepend(value);
                    ReplaceSequenceIfNeeded(sequence, result);
                    break;
                }
                case 4: {
                    int value = ReadInt("Value: ");
                    int index = ReadInt("Index: ");
                    Sequence<int>* result = sequence->InsertAt(value, index);
                    ReplaceSequenceIfNeeded(sequence, result);
                    break;
                }
                case 5: {
                    int index = ReadInt("Index: ");
                    std::cout << "Value: " << sequence->Get(index) << std::endl;
                    break;
                }
                case 6: {
                    std::cout << "First: " << sequence->GetFirst() << std::endl;
                    break;
                }
                case 7: {
                    std::cout << "Last: " << sequence->GetLast() << std::endl;
                    break;
                }
                case 8: {
                    std::cout << "Length: " << sequence->GetLength() << std::endl;
                    break;
                }
                case 9: {
                    int startIndex = ReadInt("Start index: ");
                    int endIndex = ReadInt("End index: ");
                    Sequence<int>* subSequence = sequence->GetSubsequence(startIndex, endIndex);
                    PrintSequence(*subSequence);
                    delete subSequence;
                    break;
                }
                case 10: {
                    Sequence<int>* mapped = sequence->Map([](const int& value) {
                        return value * 2;
                    });
                    PrintSequence(*mapped);
                    delete mapped;
                    break;
                }
                case 11: {
                    Sequence<int>* filtered = sequence->Where([](const int& value) {
                        return value % 2 == 0;
                    });
                    PrintSequence(*filtered);
                    delete filtered;
                    break;
                }
                case 12: {
                    int sum = sequence->Reduce([](const int& accumulator, const int& value) {
                        return accumulator + value;
                    }, 0);
                    std::cout << "Sum: " << sum << std::endl;
                    break;
                }
                case 13: {
                    Sequence<int>* newSequence = CopyToArraySequence(*sequence);
                    delete sequence;
                    sequence = newSequence;
                    std::cout << "Current implementation: MutableArraySequence" << std::endl;
                    break;
                }
                case 14: {
                    Sequence<int>* newSequence = CopyToListSequence(*sequence);
                    delete sequence;
                    sequence = newSequence;
                    std::cout << "Current implementation: MutableListSequence" << std::endl;
                    break;
                }
                case 15: {
                    RunBitSequenceDemo();
                    break;
                }
                case 0: {
                    running = false;
                    break;
                }
                default: {
                    std::cout << "Unknown command." << std::endl;
                    break;
                }
            }
        } catch (const std::exception& exception) {
            std::cout << "Error: " << exception.what() << std::endl;
        }
    }

    delete sequence;
}
