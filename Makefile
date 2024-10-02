all: compile run

compile:
	g++ main.cpp -o main

run:
	./main ./t1-ex/ex1.lp