FLAGS=-Wall -Wextra -Wfatal-errors -Wpedantic -Werror=return-type

all: run_test template timer

run_test:
	g++ -g -std=c++17 main.cpp circularBuffer.cpp  ${FLAGS} -lpthread -o run_test

template:
	g++ -g -std=c++17 cb_template_test.cpp  ${FLAGS} -lpthread -o cb_template_test

timer:
	g++ -g -std=c++17 TimerTask.cpp timerTask_test.cpp TaskRunner.cpp ${FLAGS} -lpthread -o timerTask_test

cli:
	g++ -g -std=c++17 cli.cpp  circularBuffer.cpp ${FLAGS} -o cli_test

clean:
	rm -f run_test timerTask_test cb_template