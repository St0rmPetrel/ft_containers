FLAGS = -Wall -Wextra -Werror
CC = clang++
SRC = testContainers.cpp $(wildcard testContainers/*.cpp)

.PHONY: all
all: make_testes

.PHONY: make_testes
make_testes: test run clean

test:
	$(CC) $(FLAGS) $(SRC) -o test

.PHONY: run
run:
	./test

.PHONY: clean
clean:
	rm -f test
