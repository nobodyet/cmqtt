#加载 配置文件
include config.mk

######################### 以下内容所有H5游戏后台公用 #########################

#定义 伪目标
.PHONY: all

# 设置生成的可执行程序名
TARGET = ${EXE_NAME}

# 配置独特的脚本目录名称
L_UNIQUE_DIR = ${UNIQUE_DIR}

# 设置测试服的HOSTNAME 若判断是测试服 则make时默认执行 make script 否则不执行[因为正式环境下更新so文件会导致程序宕掉]
L_DEVELOP_HOSTNAME = ${LOCAL_DEVELOP_HOSTNAME}

# 
DEBUG_FLAGS=
ifeq (${HOSTNAME}, ${L_DEVELOP_HOSTNAME})
  # ifeq (${USER}, liuqing)
  #   DEBUG_FLAGS=TRUE
  # endif
  # ifeq (${USER}, chenxing)
  #   DEBUG_FLAGS=TRUE
  # endif
  # edit by liuqing 20180806 
  ifneq (${USER}, jjcgamerun)
    DEBUG_FLAGS=TRUE
  endif
endif

# 配置文件名称
CONFIG_FILE=main.config

.PHONY: all reboot config link aireboot
# 默认的make目标
all:
	echo ${`PWD`##*/}
	@# 打印用法
	@ echo "make config : 更新配置文件的内容 及 创建日志的链接"
	@ echo "make reboot : 重新启动服务"
	@ echo "make reboot : 重新启动服务"

# 重启程序
reboot:
	@echo -----------------------------------------------------------------------
	@echo "开始重启游戏进程"
	./go.sh ${EXE_NAME}
	@echo -----------------------------------------------------------------------

#判断守护进程是否存在 若存在 则不用真的启动程序
# sn = `ps aufx | grep "keep.sh ${EXE_NAME}" |grep -v "grep"`
# exist = $(shell if [ "" != "${sn}" ]; then echo "exist"; else echo "notexist"; fi;)
# edit by liuqing 20180522 调整策略 外网守护进程由单独程序控制 不再是我自己的守护进程
# 软重启
aireboot:
	@echo -----------------------------------------------------------------------
	@echo -e "\\033[34m 在此仅关闭进程 之后由守护进程唤起 \\033[0m"
	-killall -9 -q ${EXE_NAME}
	@# 如果是正式服 且有守护进程 则不用主动启动
#  ifneq (${HOSTNAME}, ${L_DEVELOP_HOSTNAME})
	@#	@#echo "exist:${exist}"
	@#   ifneq (${exist}, exist)
	@#	@echo "无守护进程 开始重启游戏进程"
	@#	./go.sh ${EXE_NAME}
	@#   else
	@#	@echo "有守护进程 不重启游戏进程"
	@#   endif
#  endif
	@echo -----------------------------------------------------------------------

# 定义需要替换的内容
.PHONY: DBID MARK GAMEID SERVICE_TYPE UNIQUE_DIR BIGCMD D_MAX_USER SVR_UNIQID GAME_RULE_TYPE

config: link DBID MARK GAMEID SERVICE_TYPE UNIQUE_DIR BIGCMD D_MAX_USER SVR_UNIQID GAME_RULE_TYPE
ifeq (${DEBUG_FLAGS}, TRUE)
	@# 如果当前是测试服 则置测试模式为1 否则正式环境为0
	sed -i "s/^[ \t]*DEBUG_MODE[ \t]*=.*/DEBUG_MODE = 1/g" ${CONFIG_FILE} 2>/dev/null
else
	sed -i "s/^[ \t]*DEBUG_MODE[ \t]*=.*/DEBUG_MODE = 0/g" ${CONFIG_FILE} 2>/dev/null
endif
	@echo -----------------------------------------------------------------------

# 自动替换的字段
DBID MARK GAMEID SERVICE_TYPE UNIQUE_DIR BIGCMD D_MAX_USER SVR_UNIQID GAME_RULE_TYPE:
	sed -i "s/^[ \t]*$@[ \t]*=.*/$@ = ${$@}/g" ${CONFIG_FILE} 2>/dev/null

# 创建快捷方式
link:
ifeq (,$(wildcard log))
  # edit by liuqing 20180619 内外网日志名不一样了
  ifeq (${HOSTNAME}, ${L_DEVELOP_HOSTNAME})
	ln -s /opt/log/${TARGET}.log log
  else
	ln -s /opt/log/r.${TARGET}.log log
  endif
endif
