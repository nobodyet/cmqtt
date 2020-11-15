
#ifndef __MOZIT_CORE_H
#define __MOZIT_CORE_H
#include <stdint.h>
#include <assert.h>
#include "../include/mysql.sdk.h"
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
#include "../include/debug.h"
#include "mqtta.h"
/******************************************************************
 *	Function Name:	core_init
 *	Arguments:
 *	Return Value:	void
 *	Date: 			2020-11-10
 *	Editor:			bighead
 *	Description:	程序启动时刻,调用该初始化函数仅 一次,该函数不允许重复调用
 ******************************************************************/
int core_init( void);

/******************************************************************
 *	Function Name:	core_heart
 *	Arguments:
 *	Return Value:	void
 *	Date: 			2020-11-10
 *	Editor:			bighead
 *	Description:  每次主线程心跳调用, 约200ms一次, 可以调整频率在decode.c
 ******************************************************************/
int core_heart( unsigned int nowtime);

/******************************************************************
 *	Function Name:	core_heart_1s
 *	Arguments:
 *	Return Value:	void
 *	Date: 			2020-11-10
 *	Editor:			bighead
 *	Description:	每次主线程心跳调用, 约1s一次,不超过2s, 在core_heart心跳函数之后调用
 *               可以调整频率在decode.c
 ******************************************************************/
int core_heart_1s( unsigned int nowtime);

/******************************************************************
 *	Function Name:	db_init
 *	Arguments:
 *	Return Value:	void
 *	Date: 			2020-11-10
 *	Editor:			bighead
 *	Description:	程序启动时刻,调用该初始化函数多次,初始化不同mysql 连接
 ******************************************************************/
int db_init(MYSQL *db);

#endif /* #ifndef __MOZITI_CORE_H */
