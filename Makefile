SRC += src/main.cpp

OBJ += bin/main.o

WARNINGS=-w

FLAGS=-std=c++11

all:
	g++ -c -g $(SRC) $(WARNINGS) $(FLAGS)
	ls bin>/dev/null||mkdir bin
	mv *.o ./bin
	g++ -g $(OBJ) -o bin/main $(WARNINGS) $(FLAGS)
