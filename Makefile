SRC = src/bmp_reader.cpp

OBJ = bin/bmp_reader.o

WARNINGS=-Wall

FLAGS=-std=c++14 -O3

all:
	g++ -c $(SRC) $(WARNINGS) $(FLAGS)
	ls bin>/dev/null||mkdir bin
	mv *.o ./bin
	g++ src/main.cpp $(OBJ) -o bin/main $(WARNINGS) $(FLAGS)
clean:
	rm -rf bin