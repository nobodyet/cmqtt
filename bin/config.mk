# makefile的引用文件
# 定义一些变量 供其他makefile调用

############################# 以下是makefile用到 ###############################

# 本文件在 Makefile 及 restart 中会用到

# 可执行程序的名称
EXE_NAME = xinyun_mqtt.svr

# 设置测试服的HOSTNAME 若判断是测试服 则make时默认执行 make script 否则不执行[因为正式环境下更新so可能文件会导致程序宕掉]
LOCAL_DEVELOP_HOSTNAME = mqtt_dev

############################# 以下是生成main.conf时用到 ###############################


# 配置 redis的库索引
DBID = 3

#  [只有游戏服务器才会读取] 是为了区分redis中不同游戏的key 需要与loginserver和matchserver中的relaxgame.lua配置的相同
MARK = _template

# [只有游戏服务器才会读取] 游戏编号
GAMEID = 1100

# 服务类型 [login:登录服务器 match:比赛服务器 game:游戏服务器 archive:存档管理后台 task:任务活动服务器]
SERVICE_TYPE = game

# 配置独特的脚本目录名称
UNIQUE_DIR = template

# 本服务器用的cmdbig 一般只需改这一个
BIGCMD = 9

# 配置游戏一桌中的最大人数 方便快速假设不同人数的游戏后台 [为了兼容旧模式 这里只在有正确的数字值时才替换游戏内部的变量]
# D_MAX_USER = 3

# 后台的唯一编号 int类型 暂时就认证后台和比赛后台用到 [认证后台用100000系列 比赛后台用200000系列] [只要是共MYSQL库 就要保证各后台的该编号是唯一的]
# SVR_UNIQID = 100000

# 新加游戏后台的规则类型 int类型 用于同一个游戏后台代码 兼容不同玩法 比如卡五星 X番胡
GAME_RULE_TYPE = 1

