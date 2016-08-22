# You have to have tabs at the beginning of the command lines
all: main

.PHONY main: main.o
	g++ main.o -o main


main.o: main.cpp
	clear
	g++ -c main.cpp 
	
test: main
	clear;

	./main 0.40 100 5
	./main 0.80 100 5
	./main 0.40 100 100
	./main 0.60 100 100
	./main 0.80 100 100
	./main 0.40 100 1000
	./main 0.60 100 1000
	./main 0.80 100 1000
	./main game1.txt
	./main game2.txt
	./main game3.txt
	./main game4.txt	
	
	
	
	
	
	
	
#	./main .593 10000 100
#	./main 	p n s
#p = prob, n = runs, s = size

clean:
	rm -rf *.o main *~ *\#

#.PHONY makes main recompile even if no changes are found
