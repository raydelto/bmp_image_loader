SRC = src/bmp_reader.cpp

OBJ = bin/bmp_reader.o

WARNINGS=-w

FLAGS=-std=c++14

all:
	g++ -c -g $(SRC) $(WARNINGS) $(FLAGS)
	ls bin>/dev/null||mkdir bin
	mv *.o ./bin
	g++ src/main.cpp -g $(OBJ) -o bin/main $(WARNINGS) $(FLAGS)
clean:
	rm -rf bin