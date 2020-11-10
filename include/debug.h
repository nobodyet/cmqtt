/******************************************************************
** Copyright (c) 2007-2008  Wuhan Mozit Technology Co., Ltd .
** FileName:	debug.h
** Author:	 	BigHead
** Mail:		jsrenyw@sohu.com
** Date :		2007-5-31 下午07:19:16
** Version: 	2007-5-31
** File Description:ALL  the DEBUG ON/OFFs define here!
******************************************************************/
#ifndef DEBUG_H_
#define DEBUG_H_

/*	复制全部接收到的数据*/
//#define DEBUG_DUP_RECV
#ifdef DEBUG_DUP_RECV
#define DEBUG_DUP_RECV_FILE "/tmp/myDebug" //镜像数据的文件名前缀
#endif									   /*  */

/*	network.c  2006-11-2 13:04:15	*/
//#define DEBUG_NETWORK

/*	main.c  2006-11-2 13:04:26	*/
#define DEBUG
#define DEBUG_HALL_LOGIN
//#define DEBUG_HALL
//#define DEBUG_MAIN
//#define DEBUG_SEND
//#define	DEBUG_RECV
//#define DEBUG_PRE

/*	db.c  2006-11-12 15:03:16	*/
#define DEBUG_DB_PARAM

/*	sdk.c  2006-11-2 13:04:33	*/
#define DEBUG_SDK_PARAM
#define DEBUG_SDK_RETURN
/*	Time Used Debug  2006-12-19 20:01:03	*/
#define DEBUG_TIME
/*	test.h  2006-11-3 14:48:56	*/

#define DEBUG_TEST_H

/* ASSERT, add by YellowBug 2013-1-7 */
#define ASSERT
#ifdef ASSERT
#include <assert.h>
#else
#define assert(exp)
#endif

/* 调试相关宏 */
#define DEBUG_ON
#define LOG_ON
#define ERRLOG_ON

#if defined(DEBUG_ON) || defined(LOG_ON) || defined(ERRLOG_ON)
#include <stdio.h>
#endif

//打印调试信息宏 add by YellowBug 2012-7-17
#ifdef DEBUG_ON
#define debug(...)                                                          \
	do                                                                      \
	{                                                                       \
		fprintf(stdout, "D/(%s,%d,%s) ", __FILE__, __LINE__, __FUNCTION__); \
		fprintf(stdout, __VA_ARGS__);                                       \
		fprintf(stdout, "\n");                                              \
		fflush(stdout);                                                     \
	} while (0)
#else /*  */
#define debug(...)
#endif /* #ifdef DEBUG_ON */

//打印日志宏(必要信息) add by YellowBug 2012-7-17
#ifdef LOG_ON
#define log(...)                                                            \
	do                                                                      \
	{                                                                       \
		fprintf(stdout, "I/(%s,%d,%s) ", __FILE__, __LINE__, __FUNCTION__); \
		fprintf(stdout, __VA_ARGS__);                                       \
		fprintf(stdout, "\n");                                              \
		fflush(stdout);                                                     \
	} while (0)
#else /*  */
#define log(...)
#endif //OEPN_LOG

// 答应错误日志
#ifdef ERRLOG_ON
#define elog(...)                                                           \
	do                                                                      \
	{                                                                       \
		fprintf(stdout, "E/(%s,%d,%s) ", __FILE__, __LINE__, __FUNCTION__); \
		fprintf(stdout, __VA_ARGS__);                                       \
		fprintf(stdout, "\n");                                              \
		fflush(stdout);                                                     \
	} while (0)
#else
#define elog(...)
#endif

#endif /* DEBUG_H_ */

/* END */
