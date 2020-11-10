#include <string.h>
#include <stdio.h>

#ifndef MYSQL_SDK_H_
#define MYSQL_SDK_H_

#include <mysql/mysql.h>
#include <mysql/errmsg.h>

#include "globle.h"
#include "init.h"

#define MYSQL_EXEC_ERROR 22 //mysql 执行错误

//#define		MYSQL_SERVERIP			"localhost"
//#define	 	MYSQL_USERID			"jsb"
//#define		MYSQL_PASSWD			"laizijishubu"
//#define		MYSQL_DATABASE			"jj"

#define MYSQL_LOGIN_STRING MYSQL_SERVERIP, MYSQL_USERID, MYSQL_PASSWD, MYSQL_DATABASE

/******************************************************************
*       Function Name:  initMysqlAccount
*       Arguments:
*       Return Value:   int
*       Date:                   2007-6-2
*       Description:    数据库账号初始化函数，必须在使用任何sql之前调用
*               同一进程，只需要调用一次，建议在main中启动时候调用
*       
******************************************************************/
extern int initMysqlAccount(char *file);

/******************************************************************
 *      Function Name:  excuteSql
 *      Arguments:              MYSQL*, char[]
 *      Return Value:   int
 *      Date:                   2009-8-24
 *      Description:     执行SQL语句，如果失败，自动重练sql数据库2次
******************************************************************/
extern int excuteSql(MYSQL *mysql, char sqlChar[512]);
//edit by liuqing 20180320 新加一个不带日志的
extern int excuteSql2(MYSQL *mysql, char sqlChar[512]);

extern int mysqlConnetInit(MYSQL *_mysql);

#endif /*MYSQL_SDK_H_*/
