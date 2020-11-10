/******************************************************************
** Copyright (c) 2007-2008  Wuhan Mozit Technology Co., Ltd .
** FileName:	status.h
** Author:	 	BigHead
** Mail:		jsrenyw@sohu.com
** Date :		2007-6-1 下午03:38:49
** Version: 	2007-6-1
** File Description:定义用户不同状态，以及使用的指令码
******************************************************************/
#ifndef STATUS_H_
#define STATUS_H_

#include "debug.h"

//用户状态
#define ROM_CONNED_STATE 0
#define ROM_WAIT_QUIT -1
#define ROM_QUIT -10

//消息错误返回码
#define SUCCESS 0
#define FAILURE 1

//网卡的定义
#define ETH2 2
#define ETH1 1
#define ETH0 0

#endif /*STATUS_H_*/
