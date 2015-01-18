bin/sa : bin/main.o bin/game.o
	clang++ -o bin/sa -O3 -std=c++11 -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio -v

bin/main.o : src/main.cpp
	clang++ -std=c++11 -I inc -c src/main.cpp -o bin/main.o

bin/game.o : src/game.cpp
	clang++ -std=c++11 -I inc -c src/game.cpp -o bin/game.o

