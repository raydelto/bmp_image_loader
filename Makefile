SRC = src/bmp_reader.cpp

OBJ = bin/bmp_reader.o

WARNINGS=-Wall

FLAGS=-std=c++14 -O3

CXX = clang++

all:
	$(CXX) -c $(SRC) $(WARNINGS) $(FLAGS)
	ls bin>/dev/null||mkdir bin
	mv *.o ./bin
	$(CXX) src/main.cpp $(OBJ) -o bin/main $(WARNINGS) $(FLAGS)
clean:
	rm -rf bin