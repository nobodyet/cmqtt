#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/syscall.h>

#define _SYS_EPOLL_H__

#include "../include/debug.h"
#include "../include/data.h"
#include "../include/status.h"
#include "../include/init.h"
#include "../include/decode.h"
#include "../include/mysql.sdk.h"
#include "../include/globle.h"

#define CONFIG_FILE "main.config"
//#define MAX_LISTEN_CONNECT 64 // 最大同时连接请求数  2009-7-23 由512 修改为 64，该数值过大导致系统开销

#endif /*MAIN_H_*/
