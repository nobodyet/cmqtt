#定义 伪目标
.PHONY: all lib script close clean cleanall

#设置生成的可执行程序名
TARGET = mqtt.lua.svr

######################### 以下内容一般无需修改 #########################

#设置默认编译器
CC = gcc

#编译参数
CFLAGS = -Wall -W -O2 -g -Wpointer-arith -Wstrict-prototypes
#这个 -Wl,-E 不能少 要不然脚本里require *.so 会报错
MYLIBS = -Wl,-E -lrt -lpthread -lm -lz -ldl  /usr/lib64/mysql/libmysqlclient.so  #-lmysqlclient

#通用lib库
LIB_LIB = ./src/main.o ./lib/lib.a

#通用lib库
SERVICE_LIB = 

#MQTT 相关文件
MQTT_LIB = ./mqttlib/MQTTAsync.a
MQTT_PATH = ./mqttlib/

#设置运行目录 及脚本目录
RUN_PATH = ../
#SCRIPT_PATH = ./service

#自定义命令名
CP = cp -rf
RM = rm -rf

#默认的make目标
all: lib $(TARGET)

#
lib:
	cd lib && $(MAKE)
	@#cd game && $(MAKE)
ifneq (,$(wildcard ./src/main.c))
	@#如果存在main.c则需要清除及重新编译
	cd src && $(MAKE)
endif

#目标 生成游戏主程序 并附带copy一些脚本
$(TARGET): $(LIB_LIB) $(MQTT_LIB)
	@# 后面的lib库在调整先后位置后可能导致编译出错哦
	$(CC) -o $(TARGET) $(CFLAGS) $(MYLIBS) $(LIB_LIB) $(MQTT_LIB)
	$(CP) $(TARGET) $(RUN_PATH)
	$(RM) $(TARGET)

# MQTT_LIB 的生成方法
$(MQTT_LIB):
		cd $(MQTT_PATH)  && $(MAKE) 


#普通清理
clean:
	cd lib && $(MAKE) clean
	$(RM) $(TARGET)
ifneq (,$(wildcard ./src/main.c))
	@#如果存在main.c则需要清除及重新编译
	cd src && $(MAKE) clean
endif


#深度清理
cleanall: clean
	cd lib && $(MAKE) cleanall
	cd $(MQTT_PATH)  && $(MAKE) clean
	$(RM) $(TARGET)
ifneq (,$(wildcard src/main.c))
	@#如果src 目录下有文件 main.c 才能进去啊
	cd src && $(MAKE) clean
endif

	@#如果是 bighead 的目录 则还多做点事情
ifeq "$(HOME)" "/home/bighead"
	find . -name cscope\.*  -exec rm -f {} \;
	find . -name tags -exec rm -f {} \;
endif
