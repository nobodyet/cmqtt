/******************************************************************
** Copyright (c) 2012  Wuhan Mozit Technology Co., Ltd .
** FileName:	cmd.h
** Author:	 	YellowBug
** Mail:		Yellow_bug@163.com
** Date:		2012-7-16
** Version: 	v1.0
** File Description: 服务器指令接口
******************************************************************/
#include "../include/cmd.init.h"
#include "../include/debug.h"
#include <malloc.h>

//#define MAX_CMD_TYPE                          256     /* 指令类型最大个数 */
//#define MAX_CMD_VALUE                         256     /* 每种类型最大指令个数 */

/**************
* 指令处理集合
***************/
static CMDPROC *sCmdMap[MAX_CMD_TYPE] = {NULL};

/*******************************************
* Function Name:	cmdAddType
* Arguments:		unsigned short
* Return Value:		int, 1: 成功, 0: 失败
* Author:			YellowBug
* Date:				2012-7-16
* Description:		添加指令类型
********************************************/
int cmdAddType(unsigned short cmd)
{
	if (sCmdMap[cmd])
	{
		log("[ERROR] 指令类型已存在！");
		return 0;
	}
	sCmdMap[cmd] = (CMDPROC *)calloc(sizeof(CMDPROC), MAX_CMD_VALUE);
	return sCmdMap[cmd] ? 1 : 0;
}

/*************************************************************
* Function Name:	cmdRegister
* Arguments:		unsigned short, unsigned char, CMDPROC
* Return Value:		int, 1: 成功, 0: 失败
* Author:			YellowBug
* Date:				2012-7-16
* Description:		指令处理函数注册
**************************************************************/
int cmdRegister(unsigned short cmd, unsigned char value, CMDPROC proc)
{
	debug("[CMD] 注册指令! type: %u, value: %#x", cmd, value);
	if (sCmdMap[cmd])
	{
		if (sCmdMap[cmd][value])
		{
			log("[ERROR] 指令已存在! cmd.type = %u, cmd.value = %#x", cmd, value);
			return 0;
		}
	}
	return (sCmdMap[cmd][value] = proc) ? 1 : 0; /* 注意这里是赋值(=)不是判断(==) */
}

/******************************************************
* Function Name:	cmdGetProc
* Arguments:		unsigned short, unsigned char
* Return Value:		int, 1: 成功, 0: 失败
* Author:			YellowBug
* Date:				2012-7-16
* Description:		获取对应指令的处理函数
*******************************************************/
CMDPROC cmdGetProc(unsigned char cmd, unsigned char value)
{
	if (sCmdMap[cmd])
		if (sCmdMap[cmd][value])
			return sCmdMap[cmd][value];
	return NULL;
}

/* END */
