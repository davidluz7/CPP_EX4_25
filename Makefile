CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

MAIN_EXEC = main
TEST_EXEC = test

SRC = Demo.cpp
TEST = Test.cpp
HDR = MyContainer.hpp

.PHONY: all clean test valgrind

all: Main

Main: $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(MAIN_EXEC)
	./main

test: $(TEST) $(HDR)
	$(CXX) $(CXXFLAGS) $(TEST) -o $(TEST_EXEC)
	./$(TEST_EXEC)

valgrind: test
	$(VALGRIND) ./$(TEST_EXEC)

clean:
	rm -f $(MAIN_EXEC) $(TEST_EXEC)