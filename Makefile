all:
	g++ -g -std=c++17 main.cpp circularBuffer.cpp  -lpthread -o cfifo -Wall -Wextra -Wfatal-errors -Wpedantic -Werror=return-type
