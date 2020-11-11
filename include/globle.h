#ifndef GLOBLE_H
#define GLOBLE_H

#include <sys/epoll.h>
#include <mysql/mysql.h>

#include "data.h"

extern char MQTT_CLIENTID_g[256]; //MQTT.clientID
extern char MQTT_USERNAME_g[256]; //"MQTT_SVR_USER";
extern char MQTT_PWD_g[256];      // "MQTT_SVR_PWD";
extern char MQTT_ADDRESS_g[256];  //"tcp://mqtt.eclipse.org:1883";
extern int MQTT_KEEPALIVE_g;      //KEEPALIVE TIME

extern int GAME_HEART_BEAT; //心跳时间,默认微妙 1s=1000000ns
extern int timeGloble_g;    //时间计数器 每秒一次

extern char MYSQL_SERVERIP[32];
extern char MYSQL_USERID[32];
extern char MYSQL_PASSWD[64];
extern char MYSQL_DATABASE[32];

#ifdef DEBUG_DUP_RECV
//调试 复制接收数据到文件
extern FILE *_debug_recv_file;
#endif

#endif /* GLOBLE_H */
