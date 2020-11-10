#ifndef INIT_H_
#define INIT_H_

#include "debug.h"
#include "data.h"
/******************************************************************
*	Function Name:	getIPlist
*	Arguments:		int[], int*, char*
*	Return Value:	int
*	Date: 			2007-6-2
*	Description:	读取白名单配置，如果未设置白名单，返回0；反之返回读取的条数
* 					ip 转换为 int 存储，方便比较
* 					注意 数值最大长度为 MAX_IPLIST_NUM = 256
******************************************************************/
int getIPlist(int IPlist[MAX_IPLIST_NUM], int *num, char *ipFile);

/******************************************************************
*	Function Name:	checkIP
*	Arguments:		int, int[], int
*	Return Value:	int
*	Date: 			2007-6-2
*	Description:	检查ip是否在白名单中，成功返回 True=1，失败返回False=0
* 					如果  _IPnum = 0，返回 True
******************************************************************/
int checkIP(int _ip, int _IPlist[MAX_IPLIST_NUM], int _IPnum);

/******************************************************************
*	Function Name:	initRecordLogin
*	Arguments:		FILE	*logfile,char	*fileDir
*	Return Value:	return_type
*	Date: 			2007-3-10
*	Description:	打开文件，准备保存日志
******************************************************************/
int initRecordLogin(FILE **logfile, char *fileDir);

/******************************************************************
*	Function Name:	getInternetIP
*	Arguments:		int, void*, int, int
*	Return Value:	int
*	Date: 			2006-8-17
*	Description:	获取本机外网IP，eth0 = 0 eth1 =1
******************************************************************/
int getInternetIP(char *localIP, int ethIndex);

/*
 * char* GetInitKey(FileName, Section, Key)
 * Return Key=>Value
 * Ex:
 *
 * config.ini
 * [config]
 * dbhost=localhost
 *
 * Usage:
 * char dbhost[20];
 * strcpy(dbhost,GetInitKey("config.ini", "config", "dbhost"));
 *
 * History:
 * 	Chen QunShan@20061010	(chqs_at_sina.com)
 *				Handle comment line, duplicate section,
 *				similar key, partly same key, key string existing
 *				in value, trim left space char
 *				and the last char is EOF.
 */
/******************************************************************
*	Function Name:	GetInitKey
*	Arguments:		char*, char*, char*
*	Return Value:	char*
*	Date: 			2007-1-17
*	Description:	读取配置文件，支持单独一行写注释 以"#"开头
******************************************************************/
char *GetInitKey(char *filename, char *title, char *key);

//gbk->utf-8
int code_convert(char *inbuf, int inlen, char *outbuf, int outlen);
#endif /*INIT_H_*/
