CC = g++
STD = -std=c++17
PROGRAM = othello
OPT = -mtune=native -march=native -flto -O3

$(PROGRAM).out: main.o board.o AI.o evaluator.o game.o
	$(CC) main.o board.o AI.o evaluator.o game.o $(STD) $(OPT) -o $(PROGRAM).out

main.o: main.cpp
	$(CC) main.cpp -c $(STD) $(OPT) -o main.o

board.o: board.cpp
	$(CC) board.cpp -c $(STD) $(OPT) -o board.o

game.o: game.cpp
	$(CC) game.cpp -c $(STD) $(OPT) -o game.o

evaluator.o: evaluator.cpp
	$(CC) evaluator.cpp -c $(STD) $(OPT) -o evaluator.o

AI.o: AI.cpp
	$(CC) AI.cpp -c $(STD) $(OPT) -o AI.o

run:
	./$(PROGRAM).out

clean:
	rm -f *.out
	rm -f *.o
