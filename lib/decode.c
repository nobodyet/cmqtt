/******************************************************************
** Copyright (c) 2009  Wuhan Mozit Technology Co., Ltd .
** FileName:	decode.c
** Author:	 	BigHead
** Mail:		jsrenyw@sohu.com
** Date:		2007-5-31 下午07:17:32
** Editor:		YellowBug
** Date:		2010-5-26 13:34
** Version: 	2010-5-26
** File Description: 后台框架结构
******************************************************************/
#include "../include/decode.h"
#include "../core/core.h"

/******************************************************************
 *	Function Name:	RepeatRun
 *	Arguments:
 *	Return Value:	void
 *  Editor:			YellowBug
 *	Date: 			2010-5-26
 *	Description:	负责监测用户状态，发送公告等
 ******************************************************************/
void RepeatRun(void)
{
	// 该线程关闭, 目前不在启动
	log("This is in RepeatRun! pid=%d  tid=%ld  lwpid=%lu\n", getpid(), pthread_self(), syscall(SYS_gettid));
	while (1)
	{
		sleep(10);
	}
}

/******************************************************************
 *	Function Name:	Hall
 *	Arguments:
 *	Return Value:	void
 *	Date: 			2010-5-26
 *	Editor:			YellowBug
 *	Description:	处理用户消息, 以及游戏后台逻辑处理
 ******************************************************************/
void Hall(void)
{
	//CMDPROC proc = NULL;
	unsigned int nowTime;
	//MYSQL mysqlHallConn;
	//MYSQL *pmysqlHall = NULL;
	unsigned int lastHeartTime = 0;

	log("This is in Hall decode! pid=%d  tid=%ld  lwpid=%lu\n", getpid(), pthread_self(), syscall(SYS_gettid));

	//pmysqlHall = &mysqlHallConn;
	//assert(0 == db_init(pmysqlHall));
	assert(0 == core_init());
	sleep(2);
	core_do_test();
	log("Hall thread start decode msg. now:%ld\n", time(NULL));

	if (0 == DB_SCAN_ENABLE_g)
	{
		// 不开启心跳,则默认 空循环, 不在进入后面的心跳逻辑代码
		while (1)
		{
			sleep(600);
		}
	}

	while (1)
	{
		// 10ms Once
		usleep(GAME_HEART_BEAT_g);
		nowTime = timeGloble_g;
		core_heart(nowTime);

		//逻辑的心跳只需要1秒调用一次
		if (lastHeartTime != nowTime)
		{
			lastHeartTime = nowTime;
			core_heart_1s(nowTime);
		}

		// 可能本次心跳处理耗时超过了1s
		if ((timeGloble_g - nowTime) > 1)
		{
			log("Hall 心跳间隔过长  used:%ds=(%d-%d)\n", timeGloble_g - nowTime, timeGloble_g, nowTime);
			fflush(stdout);
		}
	} // while

	log("hall thread be closed!\n");
	exit(0);
}

/******************************************************************
 *      Function Name:  pthTime
 *      Arguments:      void
 *      Return Value:   void
 *      Date:                   2006-11-2
 *      Description:  时间计数器，每秒一次心跳，每秒自加一次
 *                     以单独线程方式运行
 ******************************************************************/
void pthTime(void)
{
	static struct timeval tv;

	timeGloble_g = timeBegin_g = time(NULL);
	srand(time(NULL) + getpid() + random() + timeGloble_g);
	sleep(1);
	log("在pthTime 中 初始化随机种子完毕,请勿再次初始化随机\n");
	srandom(time(NULL) + getpid() + random() + random());
	log("This is in  pthTime 心跳计数现场! pid=%d  tid=%ld  lwpid=%lu\n", getpid(), pthread_self(), syscall(SYS_gettid));

	gettimeofday(&tv, NULL);
	timeGloble_g = tv.tv_sec;

	while (1)
	{
		//edit by liuqing 20181201 调整精度 从原来的1s调整到0.3s   5hz
		usleep(200000);
		gettimeofday(&tv, NULL);
		timeGloble_g = tv.tv_sec;
	}
}
