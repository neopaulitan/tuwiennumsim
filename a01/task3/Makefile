# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Target executable
TARGET = task3

# Source file
SRC = task3.cpp

# Default rule to compile and link
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean the compiled files
clean:
	rm -f $(TARGET)
