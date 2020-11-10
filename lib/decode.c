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
#include "../service/db.h"


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

#define DISCONN_CHECK 1
#define DISCONNED 2

//edit by liuqing 20171215 新加超时锁 避免傻等
//#define DEBUG_HALL_COND	1
unsigned int t_hall_running = 0; //用于判断是否在工作中 避免重复唤醒工作线程
unsigned int t_hall_count = 0;	 //用于计数 看收到了多少条消息 超过了一定条数 才唤醒工作线程
pthread_cond_t t_cond_hall;
pthread_mutex_t t_mutex_hall;

pthread_cond_t t_cond_sendmsg;
pthread_mutex_t t_mutex_sendmsg;
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
	int nowTime;
	MYSQL mysqlHallConn;
	MYSQL *mysqlHall = NULL;

	//2017/10/13
	MYSQL mysqlADDConn;
	MYSQL *mysqlADD = NULL;

	log("This is in Hall decode! pid=%d  tid=%ld  lwpid=%lu\n", getpid(), pthread_self(), syscall(SYS_gettid));

	//edit by liuqing 20171215 新加超时锁 避免傻等
	int timeout_ms = 10; //暂定无事可做时延迟10ms吧
	struct timespec abstime;
	struct timeval now;
	int ret_cond = 0;
	//
	int doTimes = 0;
	int lastHeartTime = 0;

	//edit by liuqing 20171215 新加超时锁 避免傻等
	pthread_cond_init(&t_cond_hall, NULL);
	pthread_mutex_init(&t_mutex_hall, NULL);
	//
	pthread_cond_init(&t_cond_sendmsg, NULL);
	pthread_mutex_init(&t_mutex_sendmsg, NULL);

	mysqlHall = &mysqlHallConn;
	mysqlADD = &mysqlADDConn;

	assert(db_init(mysqlHall)); //comment by bull 2017/2/6
	assert(do_init(mysqlADD));	//edit by liuqing 20180920 这里面是执行的脚本初始化 在此之前 需要全局的时间变量被赋值了
	sleep(2);					//edit by liuqing 20180917 fc_init 里 RecvMsg线程在连接成功时sleep 2s. 这里不能比那个还提前
	log("Hall thread start decode msg. now:%ld\n", time(NULL));

	while (1)
	{
		nowTime = timeGloble_g;

#ifdef TIME_USE
		if (TU_itr > TU_SAMPLING_RATE)
		{
			gettimeofday(&TU_tv1, NULL);
		}
#endif

		//doTimes = fc_heart(nowTime);

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
			if (lastHeartTime > nowTime)
			{
				printf("Hall error. lastHeartTime(%d)>nowTime(%d)\n", lastHeartTime, nowTime);
				fflush(stdout);
			}
			else
			{
				lastHeartTime = nowTime;
				do_core(nowTime);
#ifdef DEBUG_HALL_COND
				printf("LQTEST: Hall do_core\n");
				fflush(stdout);
#endif
			}
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
		if (doTimes > 0)
		{ //有数据要处理 就继续处理吧
			t_hall_running = 1;
#ifdef DEBUG_HALL_COND
			printf("LQTEST: Hall working t_hall_count=%d doTimes=%d\n", t_hall_count, doTimes);
			fflush(stdout);
#endif
		}
		else
		{
			//edit by liuqing 20171215 不傻等了 修改为使用超时锁机制
			t_hall_running = 0;
			__sync_lock_release(&t_hall_count); //置t_sendmsg_count=0
			//
			gettimeofday(&now, NULL);
			int nsec = now.tv_usec * 1000 + (timeout_ms % 1000) * 1000000;
			abstime.tv_nsec = nsec % 1000000000;
			abstime.tv_sec = now.tv_sec + nsec / 1000000000 + timeout_ms / 1000;
			pthread_mutex_lock(&t_mutex_hall);
			//
			ret_cond = pthread_cond_timedwait(&t_cond_hall, &t_mutex_hall, &abstime);
			//
			pthread_mutex_unlock(&t_mutex_hall);
			if (ETIMEDOUT != ret_cond)
			{
				t_hall_running = 1; //避免被重复唤醒
#ifdef DEBUG_HALL_COND
				printf("LQTEST: Hall wakeup t_hall_count=%d\n", t_hall_count);
				fflush(stdout);
#endif
			}
			else
			{
#ifdef DEBUG_HALL_COND
				printf("LQTEST: Hall ETIMEDOUT\n");
				fflush(stdout);
#endif
			}
		}
		//usleep(100);

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
		//edit by liuqing 20181201 调整精度 从原来的1s调整到0.2s
		usleep(200000);
		gettimeofday(&tv, NULL);
		timeGloble_g = tv.tv_sec;
	}
}
