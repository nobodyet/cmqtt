/******************************************************************
** Copyright (c) 2007-2008  Wuhan Mozit Technology Co., Ltd .
** FileName:	mysql.sdk.c
** Author:	 	BigHead  
** Mail:		jsrenyw@sohu.com
** Date :		2009-8-21 下午07:17:32
** Version: 	2009-8-21
** File Description: 操作数据库的封装函数
******************************************************************/
#include "../include/mysql.sdk.h"

/******************************************************************
*	Function Name:	initMysqlAccount
*	Arguments:
*	Return Value:	int
*	Date: 			2007-6-2
*	Description:	数据库账号初始化函数，必须在使用任何sql之前调用
* 		同一进程，只需要调用一次，建议在main中启动时候调用
* 	
******************************************************************/
int initMysqlAccount(char *dataFile)
{
	//char         MYSQL_SERVERIP[32]=                  "localhost";
	//char         MYSQL_USERID[32]=                    "user";
	//char         MYSQL_PASSWD[64]=                    "pwd";
	//char         MYSQL_DATABASE[32]=                  "dbname";
	char result[256];
	memcpy(result, GetInitKey(dataFile, "GATE", "MYSQL_SERVERIP"), 256);
	if (strlen(result))
		memcpy(MYSQL_SERVERIP, result, 32);

	memcpy(result, GetInitKey(dataFile, "GATE", "MYSQL_USERID"), 256);
	if (strlen(result))
		memcpy(MYSQL_USERID, result, 32);

	memcpy(result, GetInitKey(dataFile, "GATE", "MYSQL_PASSWD"), 256);
	if (strlen(result))
		memcpy(MYSQL_PASSWD, result, 64);

	memcpy(result, GetInitKey(dataFile, "GATE", "MYSQL_DATABASE"), 256);
	if (strlen(result))
		memcpy(MYSQL_DATABASE, result, 32);

	printf("hello\n");
	return 0;
}

int mysqlConnetInit(MYSQL *_mysql)
{
	mysql_init(_mysql);
	if (!mysql_real_connect(_mysql, MYSQL_LOGIN_STRING, 0, NULL, 0))
	{
		printf("mysqlConnetInit:ERROR PANIC: CANNOT CONNECT MYSQL DATABASE!!!\n database -1: %s\n", mysql_error(_mysql));
		exit(1);
	}
	printf("mysqlConnetInit:succ MYSQL_SERVERIP=%s MYSQL_DATABASE=%s \n", MYSQL_SERVERIP, MYSQL_DATABASE);
	return 0;
}

/******************************************************************
 *	Function Name:	excuteSql
 *	Arguments:		MYSQL*, char[]
 *	Return Value:	int
 *	Date: 			2009-8-24
 *	Description:	 执行SQL语句，如果失败，自动重练sql数据库2次
******************************************************************/
int excuteSql(MYSQL *mysql, char sqlChar[512])
{
	int ret;
	log("SQL:%s", sqlChar);
	ret = mysql_real_query(mysql, sqlChar, (unsigned int)strlen(sqlChar));
	if (ret)
	{
		printf("Error making query-11: %s %s ret=%d\n", mysql_error(mysql), sqlChar, ret);
		//edit by liuqing 20180529 反正失败的时候也不多 这么判断下应该没什么问题吧
		if (!strncasecmp(mysql_error(mysql), "MySQL server has gone away", strlen("MySQL server has gone away")))
		{
			ret = CR_SERVER_GONE_ERROR;
		}
		if ((ret == CR_SERVER_GONE_ERROR) || (ret == CR_SERVER_LOST))
		{

			/*      释放临时空间，防止内存泄漏  2006-11-13 16:25:40 */
			/*      Mysql 重新初始化初始化  2006-11-13 16:30:15     */

			//第一次重新连接数据库
			mysql_close(mysql);
			mysql_init(mysql);
			sleep(2);
			if (!mysql_real_connect(mysql, MYSQL_LOGIN_STRING, 0, NULL, 0))
			{
				printf("Error Reconnecting to database -1: %s\n", mysql_error(mysql));
			}
			else
			{
				ret = mysql_real_query(mysql, sqlChar, (unsigned int)strlen(sqlChar));
			}
			if ((ret == CR_SERVER_GONE_ERROR) || (ret == CR_SERVER_LOST))
			{
				printf("Error making query-22: %s ret=%d\n", mysql_error(mysql), ret);

				/*      释放临时空间，防止内存泄漏  2006-11-13 16:25:40 */
				/*      Mysql 重新初始化初始化  2006-11-13 16:30:15     */
				//第二次重新连接数据库
				mysql_close(mysql);
				mysql_init(mysql);
				sleep(2);
				if (!mysql_real_connect(mysql, MYSQL_LOGIN_STRING, 0, NULL, 0))
				{
					printf("Error Reconnecting to database -2: %s\n", mysql_error(mysql));
				}
				else
				{
					ret = mysql_real_query(mysql, sqlChar, (unsigned int)strlen(sqlChar));
				}
			}
		}
	}
	return ret;
}

//edit by liuqing 20180320 新加一个不带日志的
int excuteSql2(MYSQL *mysql, char sqlChar[512])
{
	int ret;
	//log("SQL:%s\n", sqlChar);
	ret = mysql_real_query(mysql, sqlChar, (unsigned int)strlen(sqlChar));
	if (ret)
	{
		printf("Error making query-111: %s %s ret=%d\n", mysql_error(mysql), sqlChar, ret);
		//edit by liuqing 20180529 反正失败的时候也不多 这么判断下应该没什么问题吧
		if (!strncasecmp(mysql_error(mysql), "MySQL server has gone away", strlen("MySQL server has gone away")))
		{
			ret = CR_SERVER_GONE_ERROR;
		}
		if ((ret == CR_SERVER_GONE_ERROR) || (ret == CR_SERVER_LOST))
		{

			/*      释放临时空间，防止内存泄漏  2006-11-13 16:25:40 */
			/*      Mysql 重新初始化初始化  2006-11-13 16:30:15     */

			//第一次重新连接数据库
			mysql_close(mysql);
			mysql_init(mysql);
			sleep(2);
			if (!mysql_real_connect(mysql, MYSQL_LOGIN_STRING, 0, NULL, 0))
			{
				printf("Error Reconnecting to database -1: %s\n", mysql_error(mysql));
			}
			else
			{
				ret = mysql_real_query(mysql, sqlChar, (unsigned int)strlen(sqlChar));
			}
			if ((ret == CR_SERVER_GONE_ERROR) || (ret == CR_SERVER_LOST))
			{
				printf("Error making query-222: %s ret=%d\n", mysql_error(mysql), ret);

				/*      释放临时空间，防止内存泄漏  2006-11-13 16:25:40 */
				/*      Mysql 重新初始化初始化  2006-11-13 16:30:15     */
				//第二次重新连接数据库
				mysql_close(mysql);
				mysql_init(mysql);
				sleep(2);
				if (!mysql_real_connect(mysql, MYSQL_LOGIN_STRING, 0, NULL, 0))
				{
					printf("Error Reconnecting to database -2: %s\n", mysql_error(mysql));
				}
				else
				{
					ret = mysql_real_query(mysql, sqlChar, (unsigned int)strlen(sqlChar));
				}
			}
		}
	}
	return ret;
}
