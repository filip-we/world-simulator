#!/bin/bash
#gcc src/main.c -o bin/main -I /usr/local/include/gsl/include
gcc -Wall -I/usr/include/ -c $1 -o obj_file.o
gcc -L/usr/lib/ obj_file.o -lgsl -lgslcblas -lm -o bin/world_program
rm obj_file.o
