.PHONY: all clean cleanall

CC = gcc
#CFLAGS = -O2 -g -Wall -W -Wpointer-arith -Wstrict-prototypes -c
CFLAGS = -O2 -g -Wall -W -Wpointer-arith -c
#MYFLAGS= -DLOCK
MYFLAGS=

# [待完善] 确定这四个lib用不上了 就把源码干掉
OBJS = core.o  mqtta.o  # sync.o lordsdk.o record.o LibMd5.o

#定义最后生成的静态库文件
LIB_A=core.a
AR= ar rcu
RANLIB= ranlib

all: $(LIB_A)

#生成 lib.a 方便外面引用
$(LIB_A):$(OBJS)
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

core.o: core.h core.c
	$(CC) $(MYFLAGS) $(CFLAGS) core.c


mqtta.o: mqtta.h mqtta.c
	$(CC) $(CFLAGS) mqtta.c

clean:
	rm -rf $(OBJS) $(LIB_A)


