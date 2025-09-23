# Compiler and flags
CXX = g++
CXXFLAGS = -Werror -Wpedantic -Wall -std=c++20
LDFLAGS = 

# Directories
BUILDDIR = build
BINDIR = bin

# Targets
TARGET = $(BINDIR)/read_BMP
ALLNEWBMP = BMP_clockwise.bmp BMP_clockwise_filter.bmp BMP_contra_clockwise.bmp BMP_contra_clockwise_filter.bmp 

# Source files
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SRCS))

# Default target
all: $(BUILDDIR) $(BINDIR) $(TARGET)

# Create directories if they don't exist
$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

# Main executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to objects
$(BUILDDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILDDIR) $(BINDIR) $(ALLNEWBMP)

# Run the program
run: $(TARGET)
	./$(TARGET) example.bmp

# Run with valgrind memory checking
test: $(TARGET)
	valgrind --leak-check=full ./$(TARGET) example.bmp

# Phony targets
.PHONY: all clean run test
