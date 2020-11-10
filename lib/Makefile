.PHONY: all clean cleanall 

CC = gcc

INC_PATH = ../include

CFLAGS = -O2 -lrt -g -Wall -W -Wpointer-arith -Wstrict-prototypes -c

OBJS = decode.o globle.o mysql.sdk.o init.o 


#定义最后生成的静态库文件
LIB_A=lib.a
AR= ar rcu
RANLIB= ranlib


all: $(LIB_A)

#生成 lib.a 方便外面引用
$(LIB_A):$(OBJS)
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

decode.o: decode.c $(INC_PATH)/decode.h
	$(CC) $(CFLAGS) decode.c

globle.o: globle.c $(INC_PATH)/globle.h
	$(CC) $(CFLAGS) globle.c

clean: cleanall

cleanall:
ifneq (,$(wildcard *.c))
	@#如果存在 c 则需要清除及重新编译
	rm -f $(OBJS) $(LIB_A)
endif
	@echo "clean DONE"
