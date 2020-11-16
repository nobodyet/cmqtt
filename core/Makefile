.PHONY: all clean cleanall

CC = gcc
#CFLAGS = -O2 -g -Wall -W -Wpointer-arith -Wstrict-prototypes -c
CFLAGS = -fPIC -O2 -g -Wall -W -Wpointer-arith -DPAHO_MQTT_EXPORTS=1 -c
#SFLAGS = -fPIC -O2 -g -Wall -W -Wpointer-arith -shared -o
SFLAGS = -fPIC -O2 -g -Wall -W -Wpointer-arith -shared -D_GNU_SOURCE -fvisibility=hidden   -DPAHO_MQTT_EXPORTS=1 -Wno-deprecated-declarations -DOSX -o
MYFLAGS= -I../mqttlib

# [待完善] 确定这四个lib用不上了 就把源码干掉
OBJS = core.o  mqtta.o  # sync.o lordsdk.o record.o LibMd5.o

#定义最后生成的静态库文件
LIB_A=libcore.a
LIB_SO=libcore.so

#MQTT 相关文件
MQTT_LIB = paho-mqtt3a.so
MQTT_LIB_PATH = ../mqttlib/

AR= ar rcu
RANLIB= ranlib

all: $(LIB_A) $(LIB_SO)

#生成静态库 libtest.a
	#$(STATIC_TARGET):$(OBJS) 
    #  ar cru $(STATIC_TARGET) $(OBJS) 
    #  ranlib $(STATIC_TARGET)

#生成动态库 libtest.so
	#$(SHARE_TARGET):$(OBJS)
    #  $(CXX) -shared -fPIC -o $(SHARE_TARGET) $(SOURCE)

#生成 lib.a 方便外面引用
$(LIB_A):$(OBJS)
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

$(LIB_SO):$(OBJS)
	$(CC) $(MYFLAGS) -I$(MQTT_LIB_PATH) $(SFLAGS) $@ $^

core.o: core.c core.h
	$(CC) $(MYFLAGS) $(CFLAGS) $<


mqtta.o: mqtta.c mqtta.h
	$(CC) $(MYFLAGS) $(CFLAGS) $<

clean:
	-rm -rf $(OBJS) $(LIB_A) $(LIB_SO)


