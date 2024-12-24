CXX = g++
CXXFLAGS = -fopenmp -O2
TARGET = mc_int
SRC = mc_int.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)