CXX = gcc
FLAGS = -Werror

main: main.o parser.o solver.o station_handler.o test.o
	$(CXX) main.o parser.o solver.o station_handler.o test.o $(FLAGS) -o main

main.o: main.c parser.h solver.h station_handler.h test.h
	$(CXX) -c main.c $(FLAGS) -o main.o

test.o: station_handler.h parser.h solver.h test.c
	$(CXX) -c test.c $(FLAGS) -o test.o

station_handler.o: station_handler.h solver.h station_handler.c
	$(CXX) -c station_handler.c $(FLAGS) -o station_handler.o

parser.o: parser.h parser.c
	$(CXX) -c parser.c $(FLAGS) -o parser.o

solver.o: solver.h solver.c
	$(CXX) -c solver.c $(FLAGS) -o solver.o

.PHONY: clean
clean:
	rm -rf *.o
