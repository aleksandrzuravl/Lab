CXX = g++
CXXFLAGS = -Werror -Wpedantic -Wall -std=c++17

TARGET = read_BMP
ALLNEWBMP = BMP_clockwise.bmp BMP_contra_clockwise.bmp 

SRC_FILES = $(wildcard *.cpp)

OBJ_FILES = $(SRC_FILES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET) $(ALLNEWBMP)

run: $(TARGET)
	./$(TARGET) example.bmp

run test: $(TARGET)
	valgrind --leak-check=full ./$(TARGET) example.bmp
