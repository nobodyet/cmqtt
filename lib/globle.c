#include "../include/globle.h"

int CHECK_ALIVE_TIME = 600;  //通知客户端定期汇报点击等用户信息的间隔时长 秒 数据类型=整型
int HALL_REFRESH_TIME = 600; //#定期更新（配置文件信息，平台下载信息，回写服务器负载信息，重新分配WEB服衿鳎┑氖奔浼涓?单位 秒 数据类型=整型
int LOGINED_LIFE = 100;      //用户登录后多长时间无消息，断掉连接
int CONNECT_LIFE = 5;        //连接以后不发数据而不会被服务器关闭连接的最大时间 CONNECT_LIFE*2
int CONFIG_REFRESH = 60;     //相关配置文件定期更新间隔
int MAX_SOCK_LIFE = 2;       //系统判断超时用户的参数 超时时间= MAX_SOCK_LIFE * 2 秒

//#define       MAX_SOCK_LIFE           15
int timeGloble_g = 0; //全局时间计数器，整型，每秒跳到一次

char MYSQL_SERVERIP[32] = "localhost";
char MYSQL_USERID[32] = "jsb";
char MYSQL_PASSWD[64] = "laizijishubu";
char MYSQL_DATABASE[32] = "jj";

#ifdef DEBUG_DUP_RECV
FILE *_debug_recv_file = NULL; //调试 复制接收数据到文件
#endif
