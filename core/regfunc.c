/********************************************************************************
* @file name: regfunc.c
* @Description:  注册函数,和 业务逻辑模块
* @version:     v1.0.0
* @Author: bighead
* @Date: 2020-11-15 11:05:28
* @LastEditors:  sunny.sun 
* @LastEditTime: 2020-11-16 09:33:46
*******************************************************************************/
#include "core.h"
#include "regfunc.h"
#include "handle.c"
//------------------------------- 全局变量声明 ------------------------------
// struct cmd_pro
// {
//     char topic_name[256];                                                           //频道名字
//     unsigned int cnt;                                                                 //调用次数
//     void (*bc)(const char *topic, MQTTAsync_message *msg, MYSQL *_db, void *context); //处理函数
// };
//  int do_A1(char *topicName, MQTTAsync_message *recvmsg, MYSQL *_db, void *context)

static int ctr_table_size = sizeof(ctr_handle_tab) / sizeof(struct cmd_pro); //数组大小
static int topicNumLocal = sizeof(ctr_handle_tab) / sizeof(struct cmd_pro);  //配置文件中读取的Topic 数目
static struct cmd_pro *handleTableLocal;                                     //根据配置文件中的设定,启用向对应的topic_function
static unsigned int msgcnt = 0;                                              //全局消息计数器

//------------------------------- 全局变量声明 ------------------------------
/********************************************************************************
* @Function:initTopicConf
* @Brief:   从配置文件中加载Topic内容 
* @Param: 
* @Return: 无
* @Date: 2020-11-15 17:22:17
*******************************************************************************/
int initTopicConf()
{
    int i, ret, idx;
    char tmpstr[10] = {0};

    ret = atoi(GetInitKey(CONFIG_FILE, "TOPIC", "NUM"));
    topicNumLocal = ret ? ret : topicNumLocal;

    if (topicNumLocal > ctr_table_size)
    {
        topicNumLocal = ctr_table_size;
        elog("配置的Topic数目超过 程序内置Topic数目, 默认截断处理  num=%d\h", topicNumLocal);
    }

    handleTableLocal = malloc(sizeof(struct cmd_pro) * topicNumLocal);
    if (handleTableLocal == NULL)
        exit(1);
    memset(handleTableLocal, 0,sizeof(struct cmd_pro) * topicNumLocal);

    // loading  topic 配置
    idx = 0;
    for (i = 0; i < topicNumLocal; i++)
    {
        sprintf(tmpstr, "%d", i);
        ret = atoi(GetInitKey(CONFIG_FILE, "TOPIC", tmpstr));
        if (ret == 1)
        {
            //表示启用对应的Topic 服务,进行相应的注册
            handleTableLocal[idx].bc = ctr_handle_tab[i].bc;
            handleTableLocal[idx].cnt = 0;
            strcpy(handleTableLocal[idx].topic_name, ctr_handle_tab[i].topic_name);
            log("从配置文件中启用 index=%d TopicName=%s bc=%p 服务启用 \n", i, ctr_handle_tab[i].topic_name, ctr_handle_tab[i].bc);
            ++idx;
        }
        else
        {
            log("从配置文件中禁用 index=%d TopicName=%s bc=%p 服务禁用+++\n", i, ctr_handle_tab[i].topic_name, ctr_handle_tab[i].bc);
        }
    }

    //校验topic 配置, bc字段不能为空;
    for ( i = 0; i < idx; i++)
    {
        assert(handleTableLocal[i].bc);
    }

    return topicNumLocal;
}
/********************************************************************************
* @Function: 
* @Brief:    控制类消息处理函数
* @Param: 
* @Return: 无
* @Date: 2020-11-15 17:22:17
*******************************************************************************/
int regTopicFromTable(void)
{
    int idx = 0;
    struct cmd_pro *phandleTable = NULL;

    for (idx = 0; idx < topicNumLocal; idx++)
    {
        phandleTable = handleTableLocal + idx;
        if (phandleTable->bc)
        {
            my_subsribe_topic(phandleTable->topic_name, 0);
            sleep(2);
        }
    }
    return 0;
}

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
    static struct cmd_pro *pHandle = NULL;

    msgcnt++; //全局消息计数器 +1

    if (!_mysqlcon)
    {
        _mysqlcon = malloc(sizeof(MYSQL));
        db_init(_mysqlcon);
        pHandle = handleTableLocal;
    }

    // Try  cache and hit
    if (strcmp(topic, pHandle->topic_name) == 0)
    {
        pHandle->bc(topic, msg, _mysqlcon, context);
        pHandle->cnt++; //消息计数器 +1
        debug("idx=%d topic=%s func=%p decode msg\n", idx, topic, pHandle->bc);
        return 0;
    }

    // loop all of it
    for (idx = 0; idx < topicNumLocal; idx++)
    {
        pHandle = handleTableLocal + idx;
        if (strcmp(topic, pHandle->topic_name) == 0)
        {
            pHandle->bc(topic, msg, _mysqlcon, context);
            pHandle->cnt++; //消息计数器 +1
            debug("idx=%d topic=%s func=%p decode msg\n", idx, topic, pHandle->bc);
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

    sec = timeGloble_g - timeBegin_g;
    log("__STATS Running.time.cost=%dh%dm%ds second.total=%d nowTime=%d\n", sec / 3600, sec / 60, sec % 60, sec, timeGloble_g);

    log("__STATS Rcv.msg.total=%d\n", msgcnt);
    for (i = 0; i < topicNumLocal; i++)
    {
        log("__STATS topic=%s num=%d func=%p\n", handleTableLocal[i].topic_name, handleTableLocal[i].cnt, handleTableLocal[i].bc);
    }

    return 0;
}
