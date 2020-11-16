#定义 伪目标
.PHONY: all lib script close clean cleanall

#设置生成的可执行程序名
TARGET = xinyun_mqtt.svr

######################### 以下内容一般无需修改 #########################

#设置默认编译器
CC = gcc

INCLUDE_PATH = -Iinclude  -Icore -Ipaho.mqtt.c/src  -I.
#编译参数
CFLAGS = -Wall -W -O2 -g -Wpointer-arith -Wstrict-prototypes $(INCLUDE_PATH)
#这个 -Wl,-E 不能少 要不然脚本里require *.so 会报错
#MYLIBS = -Wl,-E -lrt -lpthread -lm -lz -ldl  /usr/lib64/mysql/libmysqlclient.so  #-lmysqlclient
MYLIBS = -Wl,-E  -lrt -lpthread -lm -lz -ldl  -L/usr/lib64/mysql  -lmysqlclient -Lsolib -Wl,-rpath=solib



SO_LIB=solib
PWD_PATH =$(shell pwd)
#通用lib库
COMMON_LIB =   libcommon.a   
COMMON_LIB_PATH = lib/

#通用lib库
CORE_LIB = libcore.so
CORE_LIB_PATH = core/

#MQTT 相关文件
MQTT_LIB = libpaho-mqtt3a.so
MQTT_LIB_SRC = paho.mqtt.c
MQTT_LIB_PATH = paho.mqtt.c/src/

#设置运行目录 及脚本目录
RUN_PATH = bin/

#自定义命令名
CP = cp -rf
RM = rm -rf

#默认的make目标
all: lib $(TARGET) $(MQTT_LIB) $(CORE_LIB) 

#
lib:
	cd lib && $(MAKE)
	cd $(CORE_LIB_PATH) && $(MAKE)
	cd $(MQTT_LIB_SRC)  && $(MAKE)
ifneq (,$(wildcard ./src/main.c))
	@#如果存在main.c则需要清除及重新编译
	cd src && $(MAKE)
endif

#目标 生成游戏主程序 并附带copy一些脚本
$(TARGET): ./src/main.o $(MQTT_LIB) $(CORE_LIB)  $(wildcard $(CORE_LIB_PATH)/*.c) $(wildcard $(CORE_LIB_PATH)/*.h)
	@# 后面的lib库在调整先后位置后可能导致编译出错哦
	$(CP) $(CORE_LIB_PATH)$(CORE_LIB) $(SO_LIB)
	$(CP) $(MQTT_LIB_PATH)$(MQTT_LIB) $(SO_LIB) 
	$(CC) $(CFLAGS) ./src/main.o $(COMMON_LIB_PATH)$(COMMON_LIB)  $(MYLIBS)  $(SO_LIB)/$(CORE_LIB)  $(SO_LIB)/$(MQTT_LIB)  -o $(TARGET)
	$(CP) $(TARGET) $(RUN_PATH)
	$(RM) $(RUN_PATH)/$(SO_LIB) && $(CP)   $(SO_LIB) $(RUN_PATH)
	#$(RM) $(TARGET)
	@echo -e "\\033[32m $(TARGET) is OK and move to Dir $(RUN_PATH)  \\033[0m"

# MQTT_LIB 的生成方法
$(MQTT_LIB): $(wildcard $(MQTT_LIB_SRC)/src/*.c) $(wildcard $(MQTT_LIB_SRC)/src/*.h)
		cd $(MQTT_LIB_SRC)  && $(MAKE) 


$(CORE_LIB): $(wildcard $(CORE_LIB_PATH)/*.c) $(wildcard $(CORE_LIB_PATH)/*.h)
		cd $(CORE_LIB_PATH) && $(MAKE)


#普通清理
clean:
	cd lib && $(MAKE) clean
	cd $(MQTT_PATH)  && $(MAKE) clean
	$(RM) $(TARGET)
ifneq (,$(wildcard ./src/main.c))
	@#如果存在main.c则需要清除及重新编译
	cd src && $(MAKE) clean
endif
	cd $(CORE_LIB_PATH)  && $(MAKE) clean
	cd $(MQTT_LIB_SRC) && $(MAKE) clean


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
