bin/temp : bin/main.o
	clang++ -std=c++14 bin/main.o -o bin/temp

bin/main.o : src/main.cpp
	clang++ -std=c++11 -I inc -c src/main.cpp -o bin/main.o

