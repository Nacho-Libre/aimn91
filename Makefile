CC=g++
WARN=-Wall -Wextra
CFLAGS=-O2 -fno-strict-aliasing

all: testing

aimn91.o : aimn91.cpp aimn91.h
		$(CC) $(WARN) $(CFLAGS) -g -c aimn91.cpp  

functions.o : functions.cpp aimn91.o aimn91.h 
		$(CC) $(WARN) $(CFLAGS) -g -c functions.cpp  

main.o : main.cpp aimn91.h
		$(CC) $(WARN) $(CFLAGS) -g -c main.cpp  

testing: aimn91.o functions.o main.o
		$(CC) $(CFLAGS) -g  aimn91.o functions.o main.o -lm -o $@
        
clean:
	rm -rf *.o testing
