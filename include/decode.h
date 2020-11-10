#ifndef DECODE_H_
#define DECODE_H_
#include <pthread.h>
#include <sys/time.h>

#include "status.h"
#include "globle.h"
#include "main.h"
#include "init.h"

// #include "cmd.init.h"

#define DECODE_DATA_FILE "decode.file"

/******************************************************************
 *	Function Name:	PushMessage
 *	Arguments:
 *	Return Value:	void
 *	Date: 			2006-11-4
 *	Description:	主动向客户端发送信息，包括广告，推送，个人信息;
 * 					以及定时从数据库读取信息，刷新全局变量
 ******************************************************************/
void RepeatRun(void);

/******************************************************************
 *	Function Name:	Hall
 *	Arguments:
 *	Return Value:	void
 *	Date: 			2006-11-2
 *	Description:	处理用户消息，所有与数据库相关的地方，除了第一次登录
 ******************************************************************/
void Hall(void);

/******************************************************************
 *      Function Name:  pthTime
 *      Arguments:       void
 *      Return Value:   void
 *      Date:                   2006-11-2
 *      Description:  时间计数器，每秒一次心跳，每秒自加一次
 *                     以单独线程方式运行
 ******************************************************************/
void pthTime(void);

#endif /*DECODE_H_*/
