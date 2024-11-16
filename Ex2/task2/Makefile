# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native -ffast-math -DEIGEN_DONT_PARALLELIZE
LDFLAGS = -lopenblas

# Output binary
TARGET = mmm

# Source files
SRC = main.cpp

# Environment variables
export OPENBLAS_NUM_THREADS = 1
export OMP_NUM_THREADS = 1

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

# Clean target
clean:
	rm -f $(TARGET)

