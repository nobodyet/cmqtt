# makefile的引用文件
# 定义一些变量 供其他makefile调用

############################# 以下是makefile用到 ###############################

# 本文件在 Makefile 及 restart 中会用到

# 可执行程序的名称
EXE_NAME = xinyun_mqtt.svr

# 设置测试服的HOSTNAME 若判断是测试服 则make时默认执行 make script 否则不执行[因为正式环境下更新so可能文件会导致程序宕掉]
LOCAL_DEVELOP_HOSTNAME = mqtt_dev

############################# 以下是生成main.conf时用到 ###############################


