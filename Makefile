# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -pthread

# Source files
SRC_MULTITHREADED = multithread.cpp
SRC_SINGLE_THREADED = singlethread.cpp

# Output binaries
BIN_MULTITHREADED = multithread
BIN_SINGLE_THREADED = singlethread

# Compilation time calculator script
SCRIPT = compile_time_calculator.sh

# Default rule
all: $(BIN_MULTITHREADED) $(BIN_SINGLE_THREADED)

# Rule to build multithreaded program
$(BIN_MULTITHREADED): $(SRC_MULTITHREADED)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Rule to build single-threaded program
$(BIN_SINGLE_THREADED): $(SRC_SINGLE_THREADED)
	$(CXX) -o $@ $<

# Rule to run the compilation time calculator script
compile_time: clean
	./$(SCRIPT)

# Clean rule
clean:
	rm -f $(BIN_MULTITHREADED) $(BIN_SINGLE_THREADED)

# Phony targets
.PHONY: all compile_time clean

