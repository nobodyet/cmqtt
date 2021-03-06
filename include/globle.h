#ifndef GLOBLE_H
#define GLOBLE_H

#include <mysql/mysql.h>

#include "data.h"

extern char MQTT_CLIENTID_g[256]; //MQTT.clientID
extern char MQTT_USERNAME_g[256]; //"MQTT_SVR_USER";
extern char MQTT_PWD_g[256];      // "MQTT_SVR_PWD";
extern char MQTT_SVR_IP_g[256];   //"tcp://mqtt.eclipse.org:1883";
extern int MQTT_KEEPALIVE_g;      //KEEPALIVE TIME

extern int DB_SCAN_ENABLE_g;     //是否周期扫描数据库,1hz 默认关闭
extern int TIME_SYNC_INTERVAL_g; // NTP时间同步间隔, 需要 DB_SCAN_ENABLE=1 作为前置条

extern int GAME_HEART_BEAT_g;     // 10hz 心跳时间,默认微妙 1s=1000000ns
extern unsigned int timeGloble_g; //时间计数器 每秒一次
extern unsigned int timeBegin_g;  //时间计数器 每秒一次

extern char MYSQL_SERVERIP[256];
extern char MYSQL_USERID[256];
extern char MYSQL_PASSWD[256];
extern char MYSQL_DATABASE[256];

#ifdef DEBUG_DUP_RECV
//调试 复制接收数据到文件
extern FILE *_debug_recv_file;
#endif

#endif /* GLOBLE_H */
