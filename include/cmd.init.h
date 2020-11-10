/******************************************************************
** Copyright (c) 2012  Wuhan Mozit Technology Co., Ltd .
** FileName:	cmd.h
** Author:	 	YellowBug
** Mail:		Yellow_bug@163.com
** Date:		2012-7-16
** Version: 	v1.0
** File Description: 服务器指令接口
******************************************************************/  
#ifndef __MOZIT_CMD_H
#define __MOZTI_CMD_H

#define MAX_CMD_TYPE                            256     /* 指令类型最大个数 */
#define MAX_CMD_VALUE                           256     /* 每种类型最大指令个数 */
    
#include "../include/data.h"
#include <mysql/mysql.h>
    
/**********************
*	消息处理通用类型
***********************/ 
typedef int (*CMDPROC) (MYSQL *mysql,Player *, Msg *);

/*******************************************
* Function Name:	cmdAddType
* Arguments:		unsigned short
* Return Value:		int, 1: 成功, 0: 失败
* Author:			YellowBug
* Date:				2012-7-16
* Description:		添加指令类型
********************************************/ 
int  cmdAddType(unsigned short cmd);

/*************************************************************
* Function Name:	cmdRegister
* Arguments:		unsigned short, unsigned char, CMDPROC
* Return Value:		int, 1: 成功, 0: 失败
* Author:			YellowBug
* Date:				2012-7-16
* Description:		指令处理函数注册
**************************************************************/ 
int  cmdRegister(unsigned short cmd, unsigned char value, CMDPROC proc);

/******************************************************
* Function Name:	cmdGetProc
* Arguments:		unsigned short, unsigned char
* Return Value:		int, 1: 成功, 0: 失败
* Author:			YellowBug
* Date:				2012-7-16
* Description:		获取对应指令的处理函数
*******************************************************/ 
    CMDPROC  cmdGetProc(unsigned short cmd, unsigned char value);

#endif	/* #ifndef __MOZIT_CMD_H */
    
/* END */ 
