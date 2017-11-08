all: bb

bb: buffer.o main.o
	gcc -Wall -pthread -o bb buffer.o main.o -lm

buffer.o: buffer.h buffer.c
	gcc -Wall -pthread -c buffer.c

main.o: main.c buffer.h
	gcc -Wall -pthread -c main.c

clean:
	-rm bb *.o *~
