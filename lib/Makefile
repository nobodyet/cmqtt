.PHONY: all clean cleanall 

CC = gcc

INC_PATH = ../include
MY_CFLAGS = -I../include  -I../paho.mqtt.c/src/

CFLAGS = $(MY_CFLAGS) -Os -lrt -g -Wall -W -Wpointer-arith -Wstrict-prototypes -c

OBJS = decode.o globle.o mysql.sdk.o init.o 


#定义最后生成的静态库文件
LIB_A=libcommon.a
AR= ar rcu
RANLIB= ranlib


all: $(LIB_A)

#生成 lib.a 方便外面引用
$(LIB_A):$(OBJS)
	$(AR) $@ $(OBJS)
	$(RANLIB) $@
	@echo -e "\\033[32m $(LIB_A) is OK \\033[0m"

decode.o: decode.c $(INC_PATH)/decode.h
	$(CC) $(CFLAGS) $<

globle.o: globle.c $(INC_PATH)/globle.h
	$(CC) $(CFLAGS) $<

mysql.sdk.o:mysql.sdk.c
	$(CC) $(CFLAGS) $<

init.o:init.c
	$(CC) $(CFLAGS) $<


clean: cleanall

cleanall:
ifneq (,$(wildcard *.c))
	@#如果存在 c 则需要清除及重新编译
	rm -f $(OBJS) $(LIB_A)
endif
	@echo "clean DONE"
