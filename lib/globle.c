#include "../include/globle.h"

char MQTT_CLIENTID_g[256] = "Default_ID_250";            //MQTT.clientID
char MQTT_USERNAME_g[256] = "Default_XinYun";            //"MQTT_SVR_USER";
char MQTT_PWD_g[256] = "Default_PWD";                    // "MQTT_SVR_PWD";
char MQTT_SVR_IP_g[256] = "tcp://spare.yuanwe.net:8081"; //"tcp://mqtt.eclipse.org:1883";
int MQTT_KEEPALIVE_g = 45;                               //KEEPALIVE TIME

//#define       MAX_SOCK_LIFE           15
unsigned int timeGloble_g = 0; //全局时间计数器，整型，每秒跳到一次
int GAME_HEART_BEAT_g = 10000; //默认心跳间隔,时间单位纳秒 1s=1000000ns

char MYSQL_SERVERIP[32] = "localhost";
char MYSQL_USERID[32] = "jsb";
char MYSQL_PASSWD[64] = "password";
char MYSQL_DATABASE[32] = "jj";

#ifdef DEBUG_DUP_RECV
FILE *_debug_recv_file = NULL; //调试 复制接收数据到文件
#endif
