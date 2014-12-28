#!/bin/bash
clang++ -o bin/sa -I inc src/*.cpp -std=c++11 -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio
./bin/sa
