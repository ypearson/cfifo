FLAGS=-Wall -Wextra -Wfatal-errors -Wpedantic -Werror=return-type

all: run_test

run_test:
	g++ -g -std=c++17 main.cpp circularBuffer.cpp  ${FLAGS} -lpthread -o run_test
	./run_test

