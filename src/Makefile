FLAGS =	-Os -W -Wall  -Wpointer-arith -Wstrict-prototypes 

LIB = -lpthread 

CC = gcc

TARGET =   main.o

INCLUDE_PATH = ../include

LIB_PATH = ../lib  

COPY = cp --reply=yes

all:	$(TARGET) 
	
main.o :main.c
	$(CC)  $(FLAGS)  -c  $<
	@echo -e "\\033[32m $@ is OK    \\033[0m"
	
clean:
ifneq (,$(wildcard main.c))
	@#如果存在main.c则需要清除及重新编译
	rm -f   $(TARGET) 
endif
	@echo "Clean is Done"	
