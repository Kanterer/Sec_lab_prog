#include "UI.hpp"
#include "ArraySequence.hpp"
#include "BitSequence.hpp"
#include "BitSequenceIO.hpp"
#include "ListSequence.hpp"
#include "SequenceIO.hpp"

#include <iostream>
#include <limits>
#include <string>

namespace {
    int ReadInt(const std::string& prompt) {
        while (true) {
            std::cout << prompt;

            int value;
            if (std::cin >> value) {
                return value;
            }

            std::cout << "Invalid input. Please enter an integer.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::string ReadString(const std::string& prompt) {
        std::cout << prompt;

        std::string value;
        std::cin >> value;

        return value;
    }

    int MultiplyByTwo(const int& value) {
        return value * 2;
    }

    bool IsEven(const int& value) {
        return value % 2 == 0;
    }

    int SumReducer(const int& accumulator, const int& value) {
        return accumulator + value;
    }

    template <class SequenceType>
    void PrintIntSequence(const SequenceType* sequence) {
        std::cout << "Current sequence: ";

        if (sequence == nullptr) {
            std::cout << "<not created>\n";
            return;
        }

        PrintSequence(*sequence, std::cout);
        std::cout << " | length = " << sequence->GetLength()
                  << " | " << (sequence->IsMutable() ? "mutable" : "immutable")
                  << "\n";
    }

    template <class SequenceType>
    void ReplaceSequenceIfNeeded(SequenceType*& sequence, SequenceType* result) {
        if (result != sequence) {
            delete sequence;
            sequence = result;
        }
    }

    template <class SequenceType>
    void RunIntSequenceMenu(SequenceType* sequence) {
        bool running = true;

        while (running) {
            try {
                std::cout << "\n==== Sequence<int> menu ====\n";
                PrintIntSequence(sequence);

                std::cout << "1. Append(value)\n";
                std::cout << "2. Prepend(value)\n";
                std::cout << "3. InsertAt(value, index)\n";
                std::cout << "4. Get(index)\n";
                std::cout << "5. GetSubsequence(start, end)\n";
                std::cout << "6. Map: x -> x * 2\n";
                std::cout << "7. Where: keep even numbers\n";
                std::cout << "8. Reduce: sum\n";
                std::cout << "9. Slice(index, count, replacement)\n";
                std::cout << "0. Back\n";

                int command = ReadInt("Command: ");

                switch (command) {
                    case 1: {
                        int value = ReadInt("value = ");
                        ReplaceSequenceIfNeeded(sequence, sequence->Append(value));
                        break;
                    }

                    case 2: {
                        int value = ReadInt("value = ");
                        ReplaceSequenceIfNeeded(sequence, sequence->Prepend(value));
                        break;
                    }

                    case 3: {
                        int value = ReadInt("value = ");
                        int index = ReadInt("index = ");
                        ReplaceSequenceIfNeeded(sequence, sequence->InsertAt(value, index));
                        break;
                    }

                    case 4: {
                        int index = ReadInt("index = ");
                        std::cout << "sequence[" << index << "] = " << sequence->Get(index) << "\n";
                        break;
                    }

                    case 5: {
                        int start = ReadInt("startIndex = ");
                        int end = ReadInt("endIndex = ");

                        auto* sub = sequence->GetSubsequence(start, end);

                        std::cout << "Subsequence: ";
                        PrintSequence(*sub, std::cout);
                        std::cout << "\n";

                        delete sub;
                        break;
                    }

                    case 6: {
                        auto* mapped = sequence->Map(MultiplyByTwo);

                        std::cout << "Map result: ";
                        PrintSequence(*mapped, std::cout);
                        std::cout << "\n";

                        delete mapped;
                        break;
                    }

                    case 7: {
                        auto* filtered = sequence->Where(IsEven);

                        std::cout << "Where result: ";
                        PrintSequence(*filtered, std::cout);
                        std::cout << "\n";

                        delete filtered;
                        break;
                    }

                    case 8: {
                        int sum = sequence->template Reduce<int>(SumReducer, 0);

                        std::cout << "Sum = " << sum << "\n";
                        break;
                    }

                    case 9: {
                        int start = ReadInt("index = ");
                        int count = ReadInt("count = ");
                        int replacementCount = ReadInt("How many replacement elements? ");

                        MutableArraySequence<int>* replacement = new MutableArraySequence<int>();

                        for (int i = 0; i < replacementCount; ++i) {
                            int value = ReadInt("replacement[" + std::to_string(i) + "] = ");
                            replacement->Append(value);
                        }

                        auto* sliced = sequence->Slice(start, count, replacement);

                        std::cout << "Slice result: ";
                        PrintSequence(*sliced, std::cout);
                        std::cout << "\n";

                        delete sliced;
                        delete replacement;
                        break;
                    }

                    case 0:
                        running = false;
                        break;

                    default:
                        std::cout << "Unknown command.\n";
                        break;
                }
            } catch (const std::exception& ex) {
                std::cout << "Error: " << ex.what() << "\n";
            }
        }

        delete sequence;
    }

    void RunBitSequenceMenu() {
        try {
            std::string input = ReadString("Enter bit string, for example 101001: ");
            BitSequence bits(input.c_str());

            bool running = true;

            while (running) {
                try {
                    std::cout << "\n==== BitSequence menu ====\n";
                    std::cout << "Current bit sequence: " << bits << "\n";

                    std::cout << "1. NOT\n";
                    std::cout << "2. AND with another bit string\n";
                    std::cout << "3. OR with another bit string\n";
                    std::cout << "4. XOR with another bit string\n";
                    std::cout << "5. SetBit(index, value)\n";
                    std::cout << "6. ToggleBit(index)\n";
                    std::cout << "0. Back\n";

                    int command = ReadInt("Command: ");

                    switch (command) {
                        case 1:
                            std::cout << "NOT result: " << bits.Not() << "\n";
                            break;

                        case 2: {
                            std::string otherInput = ReadString("Another bit string: ");
                            BitSequence other(otherInput.c_str());
                            std::cout << "AND result: " << bits.And(other) << "\n";
                            break;
                        }

                        case 3: {
                            std::string otherInput = ReadString("Another bit string: ");
                            BitSequence other(otherInput.c_str());
                            std::cout << "OR result: " << bits.Or(other) << "\n";
                            break;
                        }

                        case 4: {
                            std::string otherInput = ReadString("Another bit string: ");
                            BitSequence other(otherInput.c_str());
                            std::cout << "XOR result: " << bits.Xor(other) << "\n";
                            break;
                        }

                        case 5: {
                            int index = ReadInt("index = ");
                            int value = ReadInt("value 0/1 = ");
                            bits.SetBit(index, Bit(value));
                            break;
                        }

                        case 6: {
                            int index = ReadInt("index = ");
                            bits.ToggleBit(index);
                            break;
                        }

                        case 0:
                            running = false;
                            break;

                        default:
                            std::cout << "Unknown command.\n";
                            break;
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                }
            }
        } catch (const std::exception& ex) {
            std::cout << "BitSequence creation error: " << ex.what() << "\n";
        }
    }
}

void RunUI() {
    bool running = true;

    while (running) {
        std::cout << "\n========== Laboratory work 2 ==========\n";
        std::cout << "1. MutableArraySequence<int>\n";
        std::cout << "2. ImmutableArraySequence<int>\n";
        std::cout << "3. MutableListSequence<int>\n";
        std::cout << "4. ImmutableListSequence<int>\n";
        std::cout << "5. BitSequence\n";
        std::cout << "0. Exit\n";

        int command = ReadInt("Choose data structure: ");

        switch (command) {
            case 1:
                RunIntSequenceMenu(new MutableArraySequence<int>());
                break;

            case 2:
                RunIntSequenceMenu(new ImmutableArraySequence<int>());
                break;

            case 3:
                RunIntSequenceMenu(new MutableListSequence<int>());
                break;

            case 4:
                RunIntSequenceMenu(new ImmutableListSequence<int>());
                break;

            case 5:
                RunBitSequenceMenu();
                break;

            case 0:
                std::cout << "Program finished.\n";
                running = false;
                break;

            default:
                std::cout << "Unknown command.\n";
                break;
        }
    }
}
