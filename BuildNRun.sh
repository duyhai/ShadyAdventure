#!/bin/bash
clang++ -o bin/sp -I inc src/*.cpp -std=c++11 -O3 -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio
./bin/sp
