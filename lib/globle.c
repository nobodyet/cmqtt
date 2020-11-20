#include "../include/globle.h"

char MQTT_CLIENTID_g[256] = "Default_ID_250";            //MQTT.clientID
char MQTT_USERNAME_g[256] = "Default_XinYun";            //"MQTT_SVR_USER";
char MQTT_PWD_g[256] = "Default_PWD";                    // "MQTT_SVR_PWD";
char MQTT_SVR_IP_g[256] = "tcp://spare.yuanwe.net:8081"; //"tcp://mqtt.eclipse.org:1883";
int MQTT_KEEPALIVE_g = 45;                               //MQTT protocal KEEPALIVE TIME

int DB_SCAN_ENABLE_g = 0;       // 是否周期扫描数据库,1hz 默认关闭
int TIME_SYNC_INTERVAL_g = 600; // NTP时间同步间隔, 需要 DB_SCAN_ENABLE=1 作为前置条件

//#define       MAX_SOCK_LIFE           15
unsigned int timeGloble_g = 0;  //全局时间计数器，整型，每秒跳到一次
unsigned int timeBegin_g = 0;   //启动时间
int GAME_HEART_BEAT_g = 100000; //10hz 默认心跳间隔,时间单位纳秒 1s=1000000ns

char MYSQL_SERVERIP[256] = "localhost";
char MYSQL_USERID[256] = "jsb";
char MYSQL_PASSWD[256] = "password";
char MYSQL_DATABASE[256] = "jj";

#ifdef DEBUG_DUP_RECV
FILE *_debug_recv_file = NULL; //调试 复制接收数据到文件
#endif
