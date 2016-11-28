all: bb

bb: buffer.o main.o
	gcc -Wall -pthread -std=c99 -D_POSIX_C_SOURCE=199309L -o bb buffer.o main.o

buffer.o: buffer.h buffer.c
	gcc -Wall -pthread -std=c99 -D_POSIX_C_SOURCE=199309L -c buffer.c

main.o: main.c buffer.h
	gcc -Wall -pthread -std=c99 -D_POSIX_C_SOURCE=199309L -c main.c

clean:
	-rm bb *.o *~
