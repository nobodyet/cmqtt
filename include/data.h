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

#define MSG_LENGTH 2032 //248	//116                //消息体的最大大小
#define RWCACHE_SIZE 64 //等待读写消息缓冲区容纳消息条数

#define RCACHE_SIZE 64
#define WCACHE_SIZE 64
#define POLL_THREAD_NUM 8

#define GAME_HEART_BEAT 2 //2微秒  基准心跳 1秒 ＝ 1000000 微秒

#define TOKEN_LEN 32			  //令牌长度
#define USERID_LEN 16			  //用户名长度
#define MAX_IPLIST_NUM 256		  //最大支持从256个不同服务器地址链接过来的请求
#define MAX_LISTEN_NUM 16		  //目前支持最大支持同时监听16个端口  2009-7-23
#define MAX_RECV_BUFFER_SIZE 8192 //8k 大小 读消息的缓冲区大小

#define MAX_GAMENAME_LEN 32
#define MAX_PLAYER_LIFE 30 // 玩家最大生命值
#define CONNECTION_LIFE 12 // 连接最大生命值 add by yellowbug 20140106

//填充符
#define PADDING 0x0ffff //填充起始符
//分隔符
#define TOKEN ' ' //消息填充分隔符

#define FALSE 0
#define TRUE 1

#define NET_DX 1
#define NET_WT 2

// add by yellowbug
#define xptr(p, o, t) (*((t *)((p) + (o))))
#define ptr(p, o, t) ((t *)((p) + (o)))
#define unuse(p) ((void)(p))

typedef struct
{
	unsigned short int pad;	 //2byte 填充起始符
	unsigned short int size; //2byte 数据报大小
	unsigned int index;		 //4byte 发送顺序号

	unsigned short int cmd; //2byte 指令类型,沿用之前的指令即可
	unsigned char cmdBig;	//1byte 大指令号，这个需要进行分配，注册指令
	unsigned char scmd;		//1byte Server内部指令类型

} MsgHead;

typedef struct
{
	MsgHead msghead;
	unsigned char data[MSG_LENGTH];
} Msg;

/*	Message必须是结构体，因为后面有直接赋值的操作  2006-7-26 23:01:18	*/
typedef Msg Message;

//add by YellowBug 2009-8-24
#define MSGHEAD_LEN sizeof(MsgHead) //消息头长度
#define INITMSG(_msg)                 \
	memset(&(_msg), 0, sizeof(_msg)); \
	(_msg).msghead.pad = PADDING //初始化消息宏

typedef struct _S_PLAYER
{
	int status; //用户状态
	int life;	// 生命值

	struct in_addr ipaddr; //用户IP地址
	int uport;			   //端口
	int tcp_sock;
	unsigned long tcp_addr;
	unsigned int tcp_port;

	int msgInc; //消息计数器

	int uindex; //用户在内存中的索引
	int isUsed; //为1是有信息，为-1是没有信息或者已经关闭的socket，没有使用

} Player;

#endif /* DATA_H_ */

/* END */
