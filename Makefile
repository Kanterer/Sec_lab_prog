CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
TARGET = lab2
SRC = src/main.cpp src/tests.cpp src/ui.cpp

all: $(TARGET)

$(TARGET): $(SRC) include/*.hpp
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
