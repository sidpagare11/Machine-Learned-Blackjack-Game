# Makefile for blackjack_project

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11

# Target executable name
TARGET = blackjack

# Source files
SRC = main.cpp blackjack.cpp

# Default target to build the program
all: $(TARGET)

# Rule to compile the source files into an executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Rule to run the program
run: $(TARGET)
	./$(TARGET)

# Rule to clean up the generated files
clean:
	rm -f $(TARGET)
