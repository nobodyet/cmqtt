.PHONY: all clean cleanall

CC = gcc

MYFLAGS=  -I../include -I../paho.mqtt.c/src/
CFLAGS = $(MYFLAGS) -Os -g -Wall -W -Wpointer-arith -DPAHO_MQTT_EXPORTS=1 -c
SFLAGS = -fPIC -Os -g -Wall -W -Wpointer-arith -shared -D_GNU_SOURCE    -DPAHO_MQTT_EXPORTS=1  -o

# [待完善] 确定这四个lib用不上了 就把源码干掉
OBJS = core.o regfunc.o  mqtta.o  # sync.o lordsdk.o record.o LibMd5.o

#定义最后生成的静态库文件
LIB_A=libcore.a
LIB_SO=libcore.so

#MQTT 相关文件
MQTT_LIB = paho-mqtt3a.so
MQTT_LIB_PATH = ../mqttlib/

AR= ar rcu
RANLIB= ranlib

#all: $(LIB_A) $(LIB_SO)
all:  $(LIB_SO)


#生成 lib.a 方便外面引用
$(LIB_A):$(OBJS)
	$(AR) $@ $(OBJS)
	$(RANLIB) $@
	@echo -e "\\033[32m $(LIB_A) is OK \\033[0m"

# lib.so 
$(LIB_SO):  $(wildcard *.c)  $(wildcard *.h)
	$(CC) -fPIC $(CFLAGS) core.c
	$(CC) -fPIC $(CFLAGS) mqtta.c
	$(CC) -fPIC $(CFLAGS) regfunc.c

	$(CC) $(MYFLAGS)  $(SFLAGS) $@ $(OBJS)
	@echo -e "\\033[32m $(LIB_SO) is OK \\033[0m"

core.o: core.c core.h
	@$(CC) $(MYFLAGS) $(CFLAGS) $<


mqtta.o: mqtta.c mqtta.h
	@$(CC) $(MYFLAGS) $(CFLAGS) $<

regfunc.o:regfunc.c regfunc.h handle.c
	@$(CC) $(MYFLAGS) $(CFLAGS) $<

clean:
	-rm -rf $(OBJS) $(LIB_A) $(LIB_SO)


