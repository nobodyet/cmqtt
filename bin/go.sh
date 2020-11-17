#!/bin/bash
#bighead  make a patch  20141010


#如果没输入参数 则提示下用法
if [ $# -eq 0 ] ; then
    echo "go.sh [需要启动的程序名]"
    exit
fi

svr=$1
######################### 以下内容一般无需修改 #########################
#保存日志的目录
log_path=/opt/log

#设置一些变量
MV="mv -f"
log=${svr}.log              #设置生成日志名
elog=${svr}.log.err         #设置err日志名
#下面代码为保持一天的最近一次重启日志
#log_bak=${svr}.log.$(date +%Y%m%d).log
#elog_bak=${svr}.log.$(date +%Y%m%d).err

log_bak=${svr}.log.$(date +%Y%m%d_%H%M%S).log
elog_bak=${svr}.log.$(date +%Y%m%d_%H%M%S).err


#准备工作 允许生成core文件 并切换到脚本所在路径
ulimit -c 1000000
ShDir=${0%/*}
cd $ShDir


#杀进程
killall -9 -q ${svr}
echo " Kill ${svr} process"

#备份日志
if [ -f ${log_path}/${log} ]; then
    ${MV} ${log_path}/${log} ${log_path}/${log_bak}
    echo "${MV} ${log_path}/${log} ${log_path}/${log_bak}"
fi
#错误日志有内容才需备份
if [ -f ${log_path}/${elog} ]; then
if [ -s ${log_path}/${elog} ]; then
    ${MV} ${log_path}/${elog} ${log_path}/${elog_bak}
    echo "${MV} ${log_path}/${elog} ${log_path}/${elog_bak}"
fi
fi

#启动程序前再次确认 进程是不是被干掉了 如果没被干掉 就多等1秒
INFO=`ps aufx | grep "${svr}" | grep -v "grep"`
if [ -n "$INFO" ]; then
    sleep 1
fi

#启动程序 带上全路径
echo " ${PWD}/${svr} 2>${log_path}/${elog} > ${log_path}/${log}"
nohup ${PWD}/${svr} 2>${log_path}/${elog} > ${log_path}/${log} &
echo  " "

if [ $# -ge 1 ]; then
    if [ $1 = "showlog" ]; then
        echo "showlog..."
        tail -f ${log_path}/${log}
    fi
fi

# END
