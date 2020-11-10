#ifndef GLOBLE_H
#define GLOBLE_H

#include <sys/epoll.h>
#include <mysql/mysql.h>

#include "data.h"

extern int CHECK_ALIVE_TIME;  //=             600;            //通知客户端定期汇报点击等用户信息的间隔时长 秒 数据类型=整型
extern int HALL_REFRESH_TIME; //=             600;            //#定期更新（配置文件信息，平台下载信息，回写服务器负载信息，重新分配WEB服衿鳎┑氖奔浼涓?单位 秒 数据类型=整型
extern int LOGINED_LIFE;      //=             100;            //用户登录后多长时间无消息，断掉连接
extern int CONNECT_LIFE;      //=             5;                      //连接以后不发数据而不会被服务器关闭连接的最大时间 CONNECT_LIFE*2

extern int CONFIG_REFRESH; //=             60;         //相关配置文件定期更新间隔
extern int MAX_SOCK_LIFE;

extern int timeGloble_g; //时间计数器 每秒一次

extern char MYSQL_SERVERIP[32];
extern char MYSQL_USERID[32];
extern char MYSQL_PASSWD[64];
extern char MYSQL_DATABASE[32];

#ifdef DEBUG_DUP_RECV
//调试 复制接收数据到文件
extern FILE *_debug_recv_file;
#endif

#endif /* GLOBLE_H */
