/******************************************************************
** Copyright (c) 2009  Wuhan Mozit Technology Co., Ltd .
** FileName:	core.c
** Author:	 	BigHead
** Mail:		jsrenyw@icloud.com
** Editor:		bighead
** Date:		2020-11-10 13:34
** Version: 	2020-11-10
** File Description:  业务逻辑核心代码实现部分
******************************************************************/
#include "core.h"

/******************************************************************
 *	Function Name:	db_init
 *	Arguments:
 *	Return Value:	void
 *	Date: 			2020-11-10
 *	Editor:			bighead
 *	Description:	程序启动时刻,调用该初始化函数多次,初始化不同mysql 连接
 ******************************************************************/
int db_init(MYSQL *db)
{
  int ret = 1;

  if (db)
  {
    MYSQL *_db = db;
    ret = mysqlConnetInit(_db) ? 1 : 0;
  }
  else
  {
    elog("MYSQL.var is NULL \n  ");
    exit(0);
  }

  return ret;
}

/******************************************************************
 *	Function Name:	core_init
 *	Arguments:
 *	Return Value:	void
 *	Date: 			2020-11-10
 *	Editor:			bighead
 *	Description:	程序启动时刻,调用该初始化函数仅 一次,该函数不允许重复调用
 ******************************************************************/
int core_init(void)
{
  int ret = 0;

  // init mqtt.client
  init_mqtt_client();
  sleep(1);
  // 加载Topic服务配置
  initTopicConf();
  // 注册Topic
  regTopicFromTable();
  return ret;
}

/******************************************************************
 *	Function Name:	core_heart
 *	Arguments:
 *	Return Value:	void
 *	Date: 			2020-11-10
 *	Editor:			bighead
 *	Description:  每次主线程心跳调用, 约200ms一次, 可以调整频率在decode.c
 ******************************************************************/
int core_heart(unsigned int nowTime)
{
	int ret=0;
	ret=nowTime;
  return ret;
}

/******************************************************************
 *	Function Name:	core_heart_1s
 *	Arguments:
 *	Return Value:	void
 *	Date: 			2020-11-10
 *	Editor:			bighead
 *	Description:	每次主线程心跳调用, 约1s一次,不超过2s, 在core_heart心跳函数之后调用
 *               可以调整频率在decode.c
 ******************************************************************/
int core_heart_1s(unsigned int nowTime)
{
  int ret = 0;
	ret = nowTime;
  return ret;
}

// 初始化时候进行部分测试,可以随意添加
int core_do_test(void)
{
  //decode_msg_handle("TEST", NULL, NULL);
  print_stats();
  return 0;
}

// 定期调用会话统计信息, 分钟级别
int core_stats(void)
{
  print_stats();
  return 0;
}

/* END */
