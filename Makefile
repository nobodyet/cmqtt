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
LIB2_LIB = ./lib2/lib2.a

#lua lib库
LUA_LIB = ./lua/liblua.a

#redis相关文件
REDIS_LIB = ./redis/redis.a

#协议库
LUALIB_PROTO_O = ./lualib/proto.o

#cjson路径及库文件
CJSON_PATH = ./cjson
CJSON_OBJS = $(CJSON_PATH)/lua_cjson.o $(CJSON_PATH)/strbuf.o $(CJSON_PATH)/fpconv.o

#luasql路径及库文件
LUASQL_PATH = ./luasql-mysql
LUASQL_OBJS = $(LUASQL_PATH)/src/luasql.o $(LUASQL_PATH)/src/ls_mysql.o



#基础的lua文件
#INIT_LUA_SCRIPT = init.lua
#DEVLOG_LUA_SCRIPT = devlog.lua

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
	cd lua && $(MAKE) linux
	cd cjson && $(MAKE)
	cd redis && $(MAKE)
	cd lib && $(MAKE)
	cd lib2 && $(MAKE)
	cd lualib && $(MAKE)
	@#cd game && $(MAKE)
ifneq (,$(wildcard ./fc/fc.c))
	@#如果fc目录下有文件fc.c 才能进去啊
	cd fc && $(MAKE)
endif
ifneq (,$(wildcard ./src/main.c))
	@#如果存在main.c则需要清除及重新编译
	cd src && $(MAKE)
endif

#目标 生成游戏主程序 并附带copy一些脚本
$(TARGET): $(CJSON_OBJS) $(LUASQL_OBJS) $(FC_OBJS) $(LUALIB_PROTO_O) $(FC_LIB) $(LIB2_LIB) $(LIB_LIB) $(REDIS_LIB) $(LUA_LIB)
	@# 后面的lib库在调整先后位置后可能导致编译出错哦
	$(CC) -o $(TARGET) $(CFLAGS) $(MYLIBS) $(CJSON_OBJS) $(LUASQL_OBJS) $(FC_OBJS) $(LUALIB_PROTO_O) $(FC_LIB) $(LIB2_LIB) $(LIB_LIB) $(REDIS_LIB) $(LUA_LIB)
	$(CP) $(TARGET) $(RUN_PATH)
	$(RM) $(TARGET)

#cjson库
$(CJSON_OBJS):$(CJSON_PATH)/lua_cjson.c $(CJSON_PATH)/strbuf.c $(CJSON_PATH)/fpconv.c
	cd cjson && $(MAKE)

#luasql库
$(LUASQL_OBJS):$(LUASQL_PATH)/src/luasql.c $(LUASQL_PATH)/src/luasql.h $(LUASQL_PATH)/src/ls_mysql.c
	cd $(LUASQL_PATH) && $(MAKE)

#liblua.a的生成方法
$(LUA_LIB):
	cd lua && $(MAKE) linux

#普通清理
clean:
	cd lib && $(MAKE) clean
	cd lib2 && $(MAKE) clean
	cd lualib && $(MAKE) clean
	cd redis && $(MAKE) clean
	$(RM) $(TARGET)
ifneq (,$(wildcard ./fc/fc.c))
	@#如果fc目录下有文件fc.c 才能进去啊
	cd fc && $(MAKE) clean
endif
ifneq (,$(wildcard ./src/main.c))
	@#如果存在main.c则需要清除及重新编译
	cd src && $(MAKE) clean
endif


#深度清理
cleanall: clean
	cd lua && $(MAKE) clean
	cd cjson && $(MAKE) clean
	cd lib && $(MAKE) cleanall
	cd lib2 && $(MAKE) clean

ifneq (,$(wildcard fc/feicui.c))
	@#如果fc目录下有文件feicui.c 才能进去啊
	cd fc && $(MAKE) cleanall
endif

	@#如果是liuqing的目录 则还多做点事情
ifeq "$(HOME)" "/home/liuqing"
	find . -name cscope\.*  -exec rm -f {} \;
	find . -name tags -exec rm -f {} \;
endif
