#ifndef __MOZIT_MQTT_H
#define __MOZIT_MQTT_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "../mqttlib/MQTTAsync.h"
#include "../include/globle.h"
#include "../include/debug.h"


//初始化 mqtt 实例的函数
 int init_mqtt_client(void);

 // 消息发送函数
 int my_mqqta_sendmsg(char *topicName, MQTTAsync_message *pubmsg);


 // 订阅  单条 topic
int my_subsribe_topic(char * topic,int qos);

//取消订阅  单条topicname
 int my_unsubsribe_topic(char * topic,int qos);
#endif /* #ifndef __MOZIT_MQTT_H */
