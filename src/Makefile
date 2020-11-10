FLAGS =	-O2 -W -Wall  -Wpointer-arith -Wstrict-prototypes 

LIB = -lpthread 

CC = gcc

TARGET =   main.o

INCLUDE_PATH = ../include

LIB_PATH = ../lib

COPY = cp --reply=yes

all:	$(TARGET) 
	
main.o :main.c
	$(CC)  $(FLAGS)  -c  main.c
	
clean:
	rm -f   $(TARGET) 
