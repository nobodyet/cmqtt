/******************************************************************
 ** Copyright (c) 2006-2008  Wuhan Mozit Technology Co., Ltd .
 ** FileName:	main.c
 ** Author:	 	BigHead
 ** Mail:		jsrenyw@sohu.com
 ** Date :		2006-11-1 17:38:58
 ** Version: 	2006-11-1
 ** File Description:[下午03:34:30 2007-5-31 2007]
 ******************************************************************/
#include "main.h"

//edit by liuqing 20171128 新加文件锁 避免同一个名称的进程启动多个实例
//http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=29547110&id=5752998
#include <sys/stat.h>
#include <fcntl.h>
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
static int tryLockPidFile(int fd)
{
	struct flock fl;

	fl.l_type = F_WRLCK; /* write lock */
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0; //lock the whole file
	fl.l_pid = getpid();

	return (fcntl(fd, F_SETLK, &fl));
}

static int isInRunning(const char *filename)
{
	int fd;
	char buf[16];

	fd = open(filename, O_RDWR | O_CREAT, LOCKMODE);
	if (fd < 0)
	{
		log("can't create/open %s\n", filename);
		exit(1);
	}

	/* try to lock the file */
	if (tryLockPidFile(fd) == -1)
	{
		if (errno == EACCES || errno == EAGAIN)
		{
			close(fd);
			return 1;
		}
		log("can't lock %s\n", filename);
		exit(1);
	}

	/* write pid */
	ftruncate(fd, 0);
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf) + 1);
	fflush(stdout);

	// do not close the fd to keep the file lock last all the time of program life
	// if close the file, the file-lock close automatic
	// close(fd);
	return 0;
}

//获取进程名称
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
size_t get_executable_path(char *processdir, char *processname, size_t len)
{
	char *path_end;
	if (readlink("/proc/self/exe", processdir, len) <= 0)
		return -1;
	path_end = strrchr(processdir, '/');
	if (path_end == NULL)
		return -1;
	++path_end;
	strcpy(processname, path_end);
	*path_end = '\0';
	return (size_t)(path_end - processdir);
}
//edit by liuqing 20171128 end

/******************************************************************
 *	Function Name:	main
 *	Arguments:		int argc, char **argv
 *	Return Value:	int
 *	Date: 			2006-11-3
 *	Description:	main
 * 		[下午03:11:42 2009-7-25 2009] bighead 修改原来监听端口方式，将全部需要监听端口 统一放在epoll当中
 ******************************************************************/
int main(int argc, char **argv)
{
	int i;
	int ret;

	{
		time_t timep;
		time(&timep);
		printf("\n%s Main 启动时间:%s  pid=%d tid=%ld lwpid=%lu argc=%d\n\n", argv[0], ctime(&timep), getpid(), pthread_self(), syscall(SYS_gettid), argc);
	}

	//edit by liuqing 20171128 新加文件锁 避免同一个名称的进程启动多个实例
	{
		char path[2048] = "";
		char processname[1024] = "";
		char g_lock_file_path[2048] = ""; //文件锁的路径
		get_executable_path(path, processname, sizeof(path));

		snprintf(g_lock_file_path, sizeof(g_lock_file_path) - 1, "/tmp/%s.SVR.pid", processname);
		if (isInRunning(g_lock_file_path))
		{
			log("获取文件锁[%s]失败 exit\n", g_lock_file_path);
			exit(1);
		}
		else
		{
			log("获取文件锁[%s]成功 避免进程启动多个实例\n", g_lock_file_path);
		}
	}

	if (argc == 2)
	{
		log("默认配置文件=%s%s\n", argv[1], CONFIG_FILE);
		i = strlen(argv[1]);
		if ((argv[1][i - 1]) == '/')
		{
			argv[1][i - 1] = 0;
		}
	}
	else
	{
		log("不 支持指定配置文件目录方式启动 %s /home/dizhu\n ", argv[0]);
	}

	/* 获取当前进程的文件数目上限 */
	{
		struct rlimit rt;
		unsigned int max_socket_num;
		getrlimit(RLIMIT_NOFILE, &rt);
		log("当前文件数目上限 hard=%ld,soft=%ld\n", rt.rlim_max, rt.rlim_cur);

		/* 设置每个进程允许打开的最大文件数 */
		max_socket_num = 0 + 1000;
		rt.rlim_max = rt.rlim_cur = max_socket_num >= rt.rlim_max ? max_socket_num : rt.rlim_max;
		if (setrlimit(RLIMIT_NOFILE, &rt) == -1)
		{
			perror("警告：setrlimit fail \n");
			log("设置系统资源参数失败！ Max File Limit=%d 实际文件数目限制见上\n", max_socket_num);
		}
		else
		{
			log("设置系统资源参数成功！ Max File Limit=%ld\n", rt.rlim_cur);
		}
	}

	/* 读取配置文件信息,获取初始化信息 */

	{
		char *configFilePath = NULL;
		char tmpResultStr[512] = {0};
		configFilePath = malloc(1024);
		memset(configFilePath, 0, 1024);
		if (argc == 2)
		{

			sprintf(configFilePath, "%s/%s", argv[1], CONFIG_FILE);
		}
		else
		{
			sprintf(configFilePath, "%s", CONFIG_FILE);
		}

		// 心跳
		ret = atoi(GetInitKey(configFilePath, "MQTT", "GAME_HEART_BEAT"));
		GAME_HEART_BEAT_g = ret ? ret : GAME_HEART_BEAT_g;

		// keepalive
		ret = atoi(GetInitKey(configFilePath, "MQTT", "MQTT_KEEPALIVE"));
		MQTT_KEEPALIVE_g = ret ? ret : MQTT_KEEPALIVE_g;

		memcpy(tmpResultStr, GetInitKey(configFilePath, "MQTT", "MQTT_SVR_IP"), 256);
		if (strlen(tmpResultStr))
			memcpy(MQTT_SVR_IP_g, tmpResultStr, 256);

		memcpy(tmpResultStr, GetInitKey(configFilePath, "MQTT", "MQTT_CLIENTID"), 256);
		if (strlen(tmpResultStr))
			memcpy(MQTT_CLIENTID_g, tmpResultStr, 256);

		memcpy(tmpResultStr, GetInitKey(configFilePath, "MQTT", "MQTT_USERNAME"), 256);
		if (strlen(tmpResultStr))
			memcpy(MQTT_USERNAME_g, tmpResultStr, 256);

		memcpy(tmpResultStr, GetInitKey(configFilePath, "MQTT", "MQTT_PWD"), 256);
		if (strlen(tmpResultStr))
			memcpy(MQTT_PWD_g, tmpResultStr, 256);

		initMysqlAccount(configFilePath); //comment by bull 2017/2/6 //edit by liuqing 20180307

		//free configFilePath
		free(configFilePath);
		configFilePath = NULL;
	}

	//------ 2006-11-12 16:48:24 Above: 关键数据初始化  ------------
	/*心跳计数线程启动  2006-11-2 17:35:30      */
	{
		pthread_t tpthTime;
		pthread_create(&tpthTime, NULL, (void *)pthTime, NULL); //edit by liuqing 20180920 这里有usleep 100ms
		log("启动 pthTime 心跳计数 线程 每秒一次完毕\n");
		//sleep(1);
		usleep(300000); //edit by liuqing 20180917 300ms
	}

	{
		pthread_t tHall;
		/*服务线程启动  2006-11-2 17:35:30	*/
		pthread_create(&tHall, NULL, (void *)Hall, NULL);
		log("启动 Hall 线程完毕 \n");
		sleep(1);
	}
	//edit by liuqing 20180920 这个线程没用 先屏蔽掉
	{
		pthread_t tRepeatRun;
		pthread_create(&tRepeatRun, NULL, (void *)RepeatRun, NULL);
		log("启动 RepeatRun 线程完毕 \n");
		sleep(1);
	}

	/*	休眠10秒，确保其他初始化已经完成，最后打开监听端口  2006-11-12 14:34:41	*/
	sleep(2);

#ifdef DEBUG_NETWORK
	log("打开宏 DEBUG_NETWORK 调试状态\n");
	fflush(stdout);
#endif

	while (1)
	{
		//usleep(GAME_HEART_BEAT); //休息20微秒
		sleep(255);
	} //while

	return 0;
}
