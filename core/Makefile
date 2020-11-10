CC = gcc

#CFLAGS = -O2 -g -Wall -W -Wpointer-arith -Wstrict-prototypes -c
CFLAGS = -O2 -g -Wall -W -Wpointer-arith -c
#MYFLAGS= -DLOCK
MYFLAGS=

LIB_INC = -I/opt/sybase/include

# [待完善] 确定这四个lib用不上了 就把源码干掉
OBJS = core.o db.o db2.o db3.o monitor.o result.o # sync.o lordsdk.o record.o LibMd5.o

#定义最后生成的静态库文件
LIB2_A=lib2.a
AR= ar rcu
RANLIB= ranlib

all: $(LIB2_A)

#生成 lib2.a 方便外面引用
$(LIB2_A):$(OBJS)
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

core.o: core.h core.c
	$(CC) $(MYFLAGS) $(CFLAGS) core.c

db.o: db.h db.c
	$(CC) $(CFLAGS) db.c

db2.o: db2.h db2.c
	$(CC) $(CFLAGS) db2.c

db3.o: db3.h db3.c
	$(CC) $(CFLAGS) db3.c

lordsdk.o: lordsdk.c
	$(CC) $(CFLAGS) lordsdk.c

monitor.o: monitor.h monitor.c
	$(CC) $(CFLAGS) monitor.c

sync.o: sync.h sync.c
	$(CC) $(CFLAGS) sync.c

record.o: record.h record.c
	$(CC) $(CFLAGS) record.c

LibMd5.o: LibMd5.h LibMd5.c
	$(CC) $(CFLAGS) LibMd5.c

result.o: result.h result.c
	$(CC) $(CFLAGS) result.c

.PHONY: all clean

clean:
	rm -rf $(OBJS) $(LIB2_A)


