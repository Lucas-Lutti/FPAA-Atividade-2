CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = build/tango
SRC = $(wildcard src/*.cpp)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	$(TARGET) exemplos/exemplo_facil_6x6.txt ambos

clean:
	rm -rf build
