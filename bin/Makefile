#加载 配置文件
include config.mk

######################### 以下内容所有H5游戏后台公用 #########################

#定义 伪目标
.PHONY: all

# 设置生成的可执行程序名
TARGET = ${EXE_NAME}

# 配置文件名称
CONFIG_FILE=main.config

.PHONY: all reboot kill stop
#remove  config
# 默认的make目标
all:
	@echo -----------------------------------------------------------------------
	@# 打印用法
	@ echo "make reboot : 重新启动服务"
	@ echo "make stop/kill: shut down 服务"
	@echo -----------------------------------------------------------------------

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
stop:kill

kill:
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

