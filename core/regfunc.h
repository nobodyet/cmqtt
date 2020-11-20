#ifndef __XYHL_REGFUNC_H
#define __XYHL_REGFUNC_H
/********************************************************************************
* @file name: regfunc.c
* @Description:  注册函数,和 业务逻辑模块
* @version:     v1.0.0
* @Author: bighead
* @Date: 2020-11-15 11:05:28
* @LastEditors:  sunny.sun 
* @LastEditTime: 2020-11-16 09:33:46
*******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "../include/mysql.sdk.h"
#include "../include/globle.h"
#include "../include/debug.h"
#include "../src/main.h"
#include "MQTTAsync.h"
struct cmd_pro
{
    char topic_name[256];                                                            //频道名字
    unsigned int cnt;                                                                //调用次数
    int (*bc)(char *topic, MQTTAsync_message *msg, MYSQL *_db); //处理函数
};
typedef struct cmd_pro *cmd_pro_t;

/********************************************************************************
* @Function: 
* @Brief:    控制类消息处理函数
* @Param: 
* @Return: 无
* @Date: 2020-11-15 17:22:17
*******************************************************************************/
int decode_msg_handle( char *topic, MQTTAsync_message *msg );

// 从配置文件中加载需要启用的Topic列表
int initTopicConf(void);

// 自动注册需要监听的消息列表
int regTopicFromTable(void);
/********************************************************************************
* @Function: print_stats
* @Brief:  打印运行统计
* @Param: msg:消息报文
* @Return: 无
* @Date: 2019-08-26 14:54:18
*******************************************************************************/
int print_stats(void);

#endif
