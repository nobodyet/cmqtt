/*******************************************************************************
 * Copyright (c) 2012, 2020 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   https://www.eclipse.org/legal/epl-2.0/
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *******************************************************************************/

#include "mqtta.h"
#include "regfunc.h"

static void *client_context;
static MQTTAsync_connectOptions client_conn_opts = MQTTAsync_connectOptions_initializer;

void myReconnctMQTT(void *contxt)
{
    MQTTAsync client = (MQTTAsync)contxt;
    MQTTAsync_connectOptions *conn_opts = &client_conn_opts;
    int rc;
    int count = 100;

    conn_opts->keepAliveInterval = MQTT_KEEPALIVE_g;
    conn_opts->cleansession = 1;

    do
    {
        /* code */
        sleep(3);
        rc = MQTTAsync_connect(client, conn_opts);
        printf(" ret=%d Reconncting MQTT.Server \n", rc);
        --count;
    } while ((rc != MQTTASYNC_SUCCESS) && (0 != count));
}

void connlost(void *context, char *cause)
{

    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);

    printf("Reconnecting\n");
    myReconnctMQTT(context);
}

void onDisconnectFailure(void *context, MQTTAsync_failureData *response)
{
    printf("Disconnect failed contxt=%p response=%p\n",context,response);
}

void onDisconnect(void *context, MQTTAsync_successData *response)
{
    printf("Successful disconnection contxt=%p response=%p\n",context,response);
}

void onSendFailure(void *context, MQTTAsync_failureData *response)
{
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
    int rc;

    printf("Message send failed token %d error code %d\n", response->token, response->code);
    opts.onSuccess = onDisconnect;
    opts.onFailure = onDisconnectFailure;
    opts.context = client;
    if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start disconnect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
}
/****
void onSubscribe(void *context, MQTTAsync_successData *response)
{
   
    return;
}

void onSubscribeFailure(void *context, MQTTAsync_successData *response)
{

    return;
}
****/
void onSend(void *context, MQTTAsync_successData *response)
{
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
    int rc;

    printf("Message with token value %d delivery confirmed\n", response->token);
    opts.onSuccess = onDisconnect;
    opts.onFailure = onDisconnectFailure;
    opts.context = client;
    if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start disconnect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
}

void onConnectFailure(void *context, MQTTAsync_failureData *response)
{
    printf("Connect failed, rc %d\n", response ? response->code : 0);
    myReconnctMQTT(context);
}

void onConnect(void *context, MQTTAsync_successData *response)
{

    log("Successful connection contxt=%p response=%p\n",context,response);
}

int my_mqqta_sendmsg(char *topicName, MQTTAsync_message *pubmsg)
{
    MQTTAsync client = (MQTTAsync)client_context;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    int rc;

    //opts.onSuccess = onSend;
    //opts.onFailure = onSendFailure;
    opts.context = client;
    if ((rc = MQTTAsync_sendMessage(client, topicName, pubmsg, &opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start sendMessage, return code %d\n", rc);
        return (EXIT_FAILURE);
    }
    debug("Message Send: topic:%s \n", topicName);
    return rc;
}

// 注意该回调函数必须返回 1 (此时必须 free (topic和message));  如果ret=0, 表示错误,会触发上层重传处理,此时不能进行 free操作
int my_mqqta_recvmsg(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{

    debug("Message arrived: topic: %s topic.len=%d  payload.len=%d\n", topicName, topicLen,message->payloadlen);
    // 解码消息,实际的业务逻辑函数
    decode_msg_handle(topicName, message );

    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1;
}

// 订阅topic
int my_subsribe_topic(char *topic, int qos)
{
    MQTTAsync client = (MQTTAsync)client_context;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    int rc;
    log("Subscribing to topic %s for client %s using QoS%d \n", topic, MQTT_CLIENTID_g, qos);
    //opts.onSuccess = onSubscribe;
    //opts.onFailure = onSubscribeFailure;
    opts.context = client;
    //LIBMQTT_API int MQTTAsync_subscribe(MQTTAsync handle, const char* topic, int qos, MQTTAsync_responseOptions* response);
    rc = MQTTAsync_subscribe(client, topic, qos, &opts);
    return rc;
}

int my_unsubsribe_topic(char *topic, int qos)
{
    MQTTAsync client = (MQTTAsync)client_context;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    int rc;
    log("unSubscribing to topic %s for client %s using QoS%d\n", topic, MQTT_CLIENTID_g, qos);
    //opts.onSuccess = onUnSubscribe;
    //opts.onFailure = onUnSubscribeFailure;
    opts.context = client;
    //LIBMQTT_API int MQTTAsync_unsubscribe(MQTTAsync handle, const char* topic, MQTTAsync_responseOptions* response);
    rc = MQTTAsync_unsubscribe(client, topic, &opts);

    return rc;
}

int init_mqtt_client()
{
    MQTTAsync client;
    MQTTAsync_connectOptions *conn_opts = &client_conn_opts;
    int rc;

    if ((rc = MQTTAsync_create(&client_context, MQTT_SVR_IP_g, MQTT_CLIENTID_g, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to create client object, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    client = client_context;

    if ((rc = MQTTAsync_setCallbacks(client, client, connlost, my_mqqta_recvmsg, NULL)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to set callback, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    conn_opts->keepAliveInterval = MQTT_KEEPALIVE_g;
    conn_opts->cleansession = 1;
    //conn_opts->onSuccess = onConnect;
    conn_opts->onFailure = onConnectFailure;
    conn_opts->context = client;

    //注意是指针赋值 , 小心一点
    conn_opts->username = MQTT_USERNAME_g;
    conn_opts->password = MQTT_PWD_g;

    if ((rc = MQTTAsync_connect(client, conn_opts)) != MQTTASYNC_SUCCESS)
    {
        printf("* <b>1</b>: Connection refused: Unacceptable protocol version<br>\
                * <b>2</b>: Connection refused: Identifier rejected<br>\
                * <b>3</b>: Connection refused: Server unavailable<br>\
                * <b>4</b>: Connection refused: Bad user name or password<br>\
                * <b>5</b>: Connection refused: Not authorized<br>\n");
        printf("Failed to start connect, return code %d\n", rc);

        exit(EXIT_FAILURE);
    }
    sleep(2);
    assert(MQTTASYNC_TRUE == MQTTAsync_isConnected(client));

    //MQTTAsync_destroy(&client);
    return rc;
}
