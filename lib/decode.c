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
#define LOG_TAG "decode"

#include "../include/decode.h"
#include "../core/core.h"

//#define TIME_USE
#ifdef TIME_USE
#define TU_SAMPLING_RATE 800 // 采样频率
static long TU_itr = 0;
static long TU_tmp = 0;
static long TU_sum1 = 0;
static long TU_sum2 = 0;
static long TU_cnt1 = 0;
static long TU_cnt2 = 0;
static struct timeval TU_tv1;
static struct timeval TU_tv2;
#endif

unsigned int t_hall_running = 0; //用于判断是否在工作中 避免重复唤醒工作线程

pthread_cond_t t_cond_hall;
pthread_mutex_t t_mutex_hall;
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
	log("This is in RepeatRun! pid=%d  tid=%ld  lwpid=%lu\n", getpid(), pthread_self(), syscall(SYS_gettid));
	while (1)
	{
		sleep(1);
		//core_repeat_1s(timeGloble_g);
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
	MYSQL mysqlHallConn;
	MYSQL *mysqlHall = NULL;

	int doTimes = 0;
	unsigned int lastHeartTime = 0;

	log("This is in Hall decode! pid=%d  tid=%ld  lwpid=%lu\n", getpid(), pthread_self(), syscall(SYS_gettid));

	// 新加超时锁 避免傻等
	pthread_cond_init(&t_cond_hall, NULL);
	pthread_mutex_init(&t_mutex_hall, NULL);

	mysqlHall = &mysqlHallConn;

	assert(0 == db_init(mysqlHall));
	assert(0 == core_init());
	sleep(2);
	log("Hall thread start decode msg. now:%ld\n", time(NULL));

	while (1)
	{
		// 10ms Once
		usleep(GAME_HEART_BEAT_g);
		nowTime = timeGloble_g;

#ifdef TIME_USE
		if (TU_itr > TU_SAMPLING_RATE)
		{
			gettimeofday(&TU_tv1, NULL);
		}
#endif

		doTimes = core_heart(nowTime);

		//edit by liuqing 20181031 可能本次心跳处理耗时超过了1s 导致本应调用 do_core 而推迟到下次调用了
		// 外网就出现过脚本中连续两次心跳时间相差了5s 且5s内处理的消息条数为531>256*2
		if ((timeGloble_g - nowTime) > 1)
		{
			printf("Hall doTimes:%d used:%ds(%d-%d)\n", doTimes, timeGloble_g - nowTime, timeGloble_g, nowTime);
			fflush(stdout);
		}
		nowTime = timeGloble_g;

#ifdef TIME_USE
		if (TU_itr > TU_SAMPLING_RATE)
		{
			gettimeofday(&TU_tv2, NULL);
			TU_tmp = (TU_tv2.tv_sec - TU_tv1.tv_sec) * 1000000 + (TU_tv2.tv_usec - TU_tv1.tv_usec);
			TU_cnt1++;
			TU_sum1 += TU_tmp;
			log("1.TIME USED: %ld us, AVG: %ld us", TU_tmp, TU_sum1 / TU_cnt1);
		}
#endif
		//edit by liuqing 20171215 游戏逻辑的心跳只需要1秒调用一次
		if (lastHeartTime != nowTime)
		{
			lastHeartTime = nowTime;
			core_heart_1s(nowTime);
		}

#ifdef TIME_USE
		if (TU_itr > TU_SAMPLING_RATE)
		{
			TU_itr = 0;

			gettimeofday(&TU_tv1, NULL);
			TU_tmp = (TU_tv1.tv_sec - TU_tv2.tv_sec) * 1000000 + (TU_tv1.tv_usec - TU_tv2.tv_usec);
			TU_cnt2++;
			TU_sum2 += TU_tmp;
			log("2.TIME USED: %ld us, AVG: %ld us", TU_tmp, TU_sum2 / TU_cnt2);
			log("3.SUM TIME USED AVG: %ld us", (TU_sum1 + TU_sum2) / (TU_cnt2));
		}
		TU_itr++;
#endif

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
	//edit by liuqing 20170320 添加一个日志 打印该可执行文件的编译时间
	printf("\n--------\n本次编译时间:%s %s\n--------\n\n", __DATE__, __TIME__);
	printf("\n-----------------------------------------------------------------\n");
	fflush(stdout);

	timeGloble_g = time(NULL);
	srand(time(NULL) + getpid() + random() + timeGloble_g);
	sleep(1);
	log("在pthTime 中 初始化随机种子完毕,请勿再次初始化随机\n");
	srandom(time(NULL) + getpid() + random() + random());
	log("This is in  pthTime 心跳计数现场! pid=%d  tid=%ld  lwpid=%lu\n", getpid(), pthread_self(), syscall(SYS_gettid));

	gettimeofday(&tv, NULL);
	timeGloble_g = tv.tv_sec;

	while (1)
	{
		//edit by liuqing 20181201 调整精度 从原来的1s调整到0.3s
		usleep(300000);
		gettimeofday(&tv, NULL);
		timeGloble_g = tv.tv_sec;
	}
}
