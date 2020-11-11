/******************************************************************
** Copyright (c) 2007-2008  Wuhan Mozit Technology Co., Ltd .
** FileName:	data.h
** Author:	 	BigHead
** Mail:		jsrenyw@sohu.com
** Date :		2007-5-31 下午07:19:04
** Version: 	2007-5-31
** File Description:数据结构定义
******************************************************************/
#ifndef DATA_H_
#define DATA_H_

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mysql/mysql.h>

#define GAME_HEART_BEAT 10000 //10ms

#define FALSE 0
#define TRUE 1

// add by yellowbug
#define xptr(p, o, t) (*((t *)((p) + (o))))
#define ptr(p, o, t) ((t *)((p) + (o)))
#define unuse(p) ((void)(p))

#endif /* DATA_H_ */

/* END */
