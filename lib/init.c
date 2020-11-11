/******************************************************************
** Copyright (c) 2007-2008  Wuhan Mozit Technology Co., Ltd .
** FileName:	sdk.c
** Author:	 	BigHead
** Mail:		jsrenyw@sohu.com
** Date :		2007-5-31 下午07:17:32
** Version: 	2007-5-31
** File Description:道具系统的SDK 代码
//2010-11-24 bighead 增加对老socket索引 扔弃的处理
        socketIndex[oldPlayer->tcp_sock]=0;
******************************************************************/
#include <iconv.h>
#include <string.h>

#include "../include/init.h"
/******************************************************************
*	Function Name:	getIPlist
*	Arguments:		int[], int*, char*
*	Return Value:	int
*	Date: 			2007-6-2
*	Description:	读取白名单配置，如果未设置白名单，返回0；反之返回读取的条数
* 					ip 转换为 int 存储，方便比较
* 					注意 数值最大长度为 MAX_IPLIST_NUM = 256
******************************************************************/
/****
int getIPlist(int IPlist[MAX_IPLIST_NUM],
			  int *num,
			  char *ipFile)
{
	int ret = 0;

	((void)(IPlist));
	((void)(ipFile));

	*num = 0;
	return (ret);
}
***/
/******************************************************************
*	Function Name:	checkIP
*	Arguments:		int, int[], int
*	Return Value:	int
*	Date: 			2007-6-2
*	Description:	检查ip是否在白名单中，成功返回 True=1，失败返回False=0
* 					如果  _IPnum = 0，返回 True
******************************************************************/
/***
int checkIP(int _ip,
			int _IPlist[MAX_IPLIST_NUM],
			int _IPnum)
{
	int ret = 0;
	int i;

	for (i = 0; i < _IPnum; ++i)
	{
		if (_ip == _IPlist[i])
		{
			ret = 1;
			break;
		} // if (_ip == _IPlist[i])
	}	  //for (i = 0; i < _IPnum; ++i)

  //如果白名单为空，默认允许所有连接请求  2007-6-2 下午02:58:24	
	ret = _IPnum ? ret : 1;

	return (ret);
}
*****/
/******************************************************************
*	Function Name:	initRecordLogin
*	Arguments:		FILE	*logfile,char	*fileDir
*	Return Value:	return_type
*	Date: 			2007-3-10
*	Description:	打开文件，准备保存日志
******************************************************************/
int initRecordLogin(FILE **logfile, char *fileDir)
{
	time_t timep;
	struct tm *p = NULL;
	char fileName[1024];

	*logfile = NULL;

	timep = time(NULL);
	p = localtime(&timep);
	//bzero(fileName,sizeof(fileName));
	memset(fileName, 0, sizeof(fileName));

	sprintf(fileName, "%s/loginInfo.%04d%02d%02d.log",
			fileDir,
			p->tm_year + 1900,
			p->tm_mon + 1,
			p->tm_mday);

	*logfile = fopen(fileName, "a");
	if (*logfile)
	{
		//fprintf(*logfile,"登录时间\t登录用户名\t登录金币\t登录金条\t登录积分\t登录IP\n");
		printf("初始化登录日志文件成功：%s\n", fileName);
	} // if (logfile)

	return (0);
}

#define MAXINTERFACES 16
/******************************************************************
*	Function Name:	getInternetIP
*	Arguments:		int, void*, int, int
*	Return Value:	int
*	Date: 			2006-8-17
*	Description:	获取本机外网IP，eth0 = 0 eth1 =1
******************************************************************/
int getInternetIP(char *localIP, int ethIndex)
{

  strcpy(localIP,"127.0.0.1");
  ethIndex = 1;
  /***
	register int fd, intrface;
	struct ifreq buf[MAXINTERFACES];
	struct ifconf ifc;

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		return -1;
	}

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = (caddr_t)buf;
	if (ioctl(fd, SIOCGIFCONF, (char *)&ifc) < 0)
	{
		return -1;
	}

	intrface = ifc.ifc_len / sizeof(struct ifreq);
	printf("number of Network interface is: %d\n", intrface);
	while (intrface-- > 0)
	{
		printf("net device %s\n", buf[intrface].ifr_name);

		if ((ioctl(fd, SIOCGIFFLAGS, (char *)&buf[intrface])) < 0)
		{
			continue;
		}

		if (buf[intrface].ifr_flags & IFF_PROMISC)
		{
			puts("the interface is PROMISC");
		}
		else
		{
			if (buf[intrface].ifr_flags & IFF_UP)
			{
				puts("the interface status is UP");
			}
			else
			{
				if (buf[intrface].ifr_flags & IFF_RUNNING)
					puts("the interface status is RUNNING");
			}
		}

		if (!(ioctl(fd, SIOCGIFADDR, (char *)&buf[intrface])))
		{
			puts("IP address is:");
			puts((char *)inet_ntoa(((struct sockaddr_in *)(&buf[intrface].ifr_addr))->sin_addr));
			puts("");
		}
		else
		{
			char str[256];
			sprintf(str, "cpm: ioctl device %s", buf[intrface].ifr_name);
			perror(str);
		}
	}
	close(fd);

	//eth0
	strcpy(localIP, (char *)inet_ntoa(((struct sockaddr_in *)(&buf[ethIndex + 1].ifr_addr))->sin_addr));
	printf("Local IP:%s\n\n", localIP);
  ***/
	return 0;
}

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
char *GetInitKey(char *filename, char *title, char *key)
{
	FILE *fp;
	char tmpLine[1024];
	int rtnval;
	int i = 0;
	int flag = 0;
	char *tmp;
	static char tmpstr[1024];

	if ((fp = fopen(filename, "r")) == NULL)
	{
		printf("出错: 严重错误 *** FILE NO EXIST: %s ***\n", filename);
		return "have no such file";
	}
	while (!feof(fp))
	{
		rtnval = fgetc(fp);
		if (rtnval == EOF)
		{
			tmpLine[i++] = 0;
		}
		else
		{
			tmpLine[i++] = rtnval;
		}
		if (rtnval == '\n' || rtnval == EOF)
		{
			tmpLine[--i] = 0;
			i = 0;

			if (tmpLine[0] == '\0' || strchr("#;", tmpLine[0]) != NULL)
				continue; // Skip null line or comment line

			if (tmpLine[0] == '[')
			{
				tmp = NULL;
				if (flag == 1)
				{
// Meet next section
#ifdef PROCESS_DUP_SECTION // INI file exist duplicate section
					flag = 0;
#else
					break; // Skip other context
#endif
				}
			}
			else
			{
				tmp = strchr(tmpLine, '=');
			}

			if ((tmp != NULL) && (flag == 1))
			{
				char *p;

				*tmp = '\0'; // erase '=', spilte Key and Value

				p = strstr(tmpLine, key);
				if (p != NULL)
				{
					if (p > tmpLine && strchr(" \t", *(p - 1)) == NULL)
						continue; // exist prefix, mishit key

					p += strlen(key);
					if (*p == '\0' || strchr(" \t", *p) != NULL)
					{
						tmp++; // Skip '='
						while (*tmp == ' ' || *tmp == '\t')
							tmp++; // Skip left lead ' ' or '\t'
						strcpy(tmpstr, tmp);
						fclose(fp);
						printf("[%s]->%s.value=%s\n", title, key, tmpstr);
						return tmpstr;
					}
				}
			}
			else
			{
				strcpy(tmpstr, "[");
				strcat(tmpstr, title);
				strcat(tmpstr, "]");
				if (strcmp(tmpstr, tmpLine) == 0)
				{
					flag = 1;
				}
			}
		}

		if (rtnval == EOF)
		{
			break;
		}
	} // end of while
	fclose(fp);

	return "";
}
/*--------------*
 *  Test Case:  *
 *--------------*
config.ini

;[test]
#p2= 789
;p2=	9
p2 = 456

[test]
p3 = p2
pp2= 45
p2 =  "456"

[ip]
portexam=777
[user]
port=666
 *--------------*/
//gbk->utf-8
int code_convert(char *inbuf, int inlen, char *outbuf, int outlen)
{
	iconv_t cd;
	//int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open("UTF-8", "GB2312");
	if (cd == 0)
		return -1;
	memset(outbuf, 0, outlen);
	if ((int)iconv(cd, pin, (size_t *)&inlen, pout, (size_t *)&outlen) == -1)
		return -1;
	iconv_close(cd);
	return 0;
}
