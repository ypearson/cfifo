all:
	g++ -g main.cpp -std=c++17 -lpthread -o cfifo -Wfatal-errors
# 	g++ -g main.cpp cfifo.cpp -std=c++17 -lpthread -o cfifo
