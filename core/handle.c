/******************************************************************
** Copyright (c) 2009  Wuhan Mozit Technology Co., Ltd .
** FileName:	handle.c
** Author:	 	BigHead
** Mail:		jsrenyw@icloud.com
** Editor:		bighead
** Date:		2020-11-16 13:34
** Version: 	2020-11-16
** File Description:  业务逻辑核心代码实现部分
******************************************************************/

/********************************************************************************
* @Function: sscp_reboot_system
* @Brief: 解析服务器下发的系统重启指令
* @Param: msg:消息报文
* @Return: 无
* @Date: 2019-08-26 14:54:18
*******************************************************************************/
int do_A1(char *topicName, MQTTAsync_message *recvmsg, MYSQL *_db )
{
	debug("topicName=%s msg=%p db=%p \n", topicName, recvmsg, _db);
    return 1;
}

int do_test(char *topicName, MQTTAsync_message *recvmsg, MYSQL *_db)
{
    debug("topicName=%s msg=%p db=%p \n", topicName, recvmsg, _db);
    return 1;
}

// 消息和其对应的处理函数表格,  建议频度从高往下排
static struct cmd_pro ctr_handle_tab[] = {
    {"RRS123", 0, do_A1}, //远程重启终端设备系统
    {"SFR123", 0, do_A1}, //恢复工厂设置
    {"RS4123", 0, do_A1}, //服务器跳转
    {"RDC123", 0, do_A1}, //远程调光控制
    {"RSC123", 0, do_A1}, //远程开关控制
    {"RCN123", 0, do_A1}, //删除节点
    {"TEST12", 0, do_test},
};
