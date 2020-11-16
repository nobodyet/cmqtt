/********************************************************************************
* @file name: regfunc.c
* @Description:  注册函数,和 业务逻辑模块
* @version:     v1.0.0
* @Author: bighead
* @Date: 2020-11-15 11:05:28
* @LastEditors:  sunny.sun 
* @LastEditTime: 2020-11-16 09:33:46
*******************************************************************************/
#include "regfunc.h"

#include "handle.c"
//------------------------------- 全局变量声明 ------------------------------
// struct cmd_pro
// {
//     const char *topic_name;                                                           //频道名字
//     unsigned int cnt;                                                                 //调用次数
//     void (*bc)(const char *topic, MQTTAsync_message *msg, MYSQL *_db, void *context); //处理函数
// };

// 消息和其对应的处理函数表格,  建议频度从高往下排
static const struct cmd_pro ctr_handle_tab[] = {
    {"RRS", 0, do_A1}, //远程重启终端设备系统
    {"SFR", 0, do_A1}, //恢复工厂设置
    {"RS", 0, do_A1},  //服务器跳转
    {"RDC", 0, do_A1}, //远程调光控制
    {"RSC", 0, do_A1}, //远程开关控制
    {"RCN", 0, do_A1}, //删除节点
};

static ctr_table_size = sizeof(ctr_handle_tab) / sizeof(struct cmd_pro); //数组大小
static unsigned int msgcnt = 0;                                          //全局消息计数器
//------------------------------- 全局变量声明 ------------------------------

/********************************************************************************
* @Function: 
* @Brief:    控制类消息处理函数
* @Param: 
* @Return: 无
* @Date: 2020-11-15 17:22:17
*******************************************************************************/
int decode_msg_handle(const char *topic, MQTTAsync_message *msg, void *context)
{
    static int idx = 0;
    static MYSQL *_mysqlcon = NULL;

    msgcnt++; //全局消息计数器 +1

    if (!_mysqlcon)
    {
        _mysqlcon = malloc(sizeof(MYSQL));
        db_init(&_mysqlcon);
    }

    // Try  cache and hit
    if ((idx < ctr_table_size) && strcmp(topic, ctr_handle_tab[idx.topic_name]) == 0)
    {
        ctr_handle_tab[idx].bc(topic, msg, _mysqlcon, conext);
        ctr_handle_tab[idx].cnt++; //消息计数器 +1
        return 0;
    }

    // loop all of it
    for (idx = 0; idx < ctr_table_size; idx++)
    {
        if (strcmp(topic, ctr_handle_tab[idx].topic_name) == 0)
        {
            //rt_kprintf("-->> Receive server %02X cmd\n",cmd);
            ctr_handle_tab[idx].bc((char *)msg_data->message->payload, msg_data->message->payloadlen);
            ctr_handle_tab[idx].cnt++; //消息计数器 +1
            break;
        }
    }

    return 0;
}
/********************************************************************************
* @Function: print_stats
* @Brief:  打印运行统计
* @Param: msg:消息报文
* @Return: 无
* @Date: 2019-08-26 14:54:18
*******************************************************************************/
int print_stats()
{
    int i;
    int sec;

    sec = timgGloble_G - timeBegin_g;
    log("__STATS Running.time.cost=%dh%dm%ds second.total=%d\n", sec / 3600, sec / 60, sec % 60, sec);

    log("__STATS Rcv.msg.total=%d\n", msgcnt);
    for (i = 0; i < crt_table_size; i++)
    {
        log("__STATS topic=%s num=%d func=%p\n", ctr_handle_tab[i].topic_name, ctr_handle_tab[i].cnt, ctr_handle_tab[i].bc);
    }

    return 0;
}
