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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTAsync.h"

#include "../include/globe.h"
#if !defined(_WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif

#if defined(_WRS_KERNEL)
#include <OsWrapper.h>
#endif

#define MQTT_USERNAME "MQTT_SVR_USER"
#define MQTT_PWD "MQTT_SVR_PWD"
#define ADDRESS "tcp://mqtt.eclipse.org:1883"
#define CLIENTID "ExampleClientPub"
#define TOPIC "MQTT Examples"
#define PAYLOAD "Hello World!"
#define QOS 1
#define TIMEOUT 10000L

int finished = 0;

static void *client_context;
static MQTTAsync_connectOptions client_conn_opts = MQTTAsync_connectOptions_initializer;

void myReconnctMQTT(void *contxt)
{
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_connectOptions *conn_opts = &client_conn_opts;
    int rc;
    conn_opts.keepAliveInterval = CHECK_ALIVE_TIME;
    conn_opts.cleansession = 1;

    sleep(2);
    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start connect, return code %d\n", rc);
    }
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
    printf("Disconnect failed\n");
}

void onDisconnect(void *context, MQTTAsync_successData *response)
{
    printf("Successful disconnection\n");
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
    finished = 1;
}

void onConnect(void *context, MQTTAsync_successData *response)
{

    printf("Successful connection\n");
}

int my_mqqta_sendmsg(char *topicName, MQTTAsync_message *pubmsg)
{
    MQTTAsync client = (MQTTAsync)client_context;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    int rc;

    //opts.onSuccess = onSend;
    opts.onFailure = onSendFailure;
    opts.context = client;
    if ((rc = MQTTAsync_sendMessage(client, TOPIC, pubmsg, &opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start sendMessage, return code %d\n", rc);
        return (EXIT_FAILURE);
    }
    return rc;
}

int messageArrived(void *context, char *topicName, int topicLen, MQTTAsync_message *m)
{
    /* not expecting any messages */
    return 1;
}

int init_mqtt_client()
{
    MQTTAsync client = (MQTTAsync)client_context;
    MQTTAsync_connectOptions *conn_opts = &client_conn_opts;
    int rc;

    if ((rc = MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to create client object, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    if ((rc = MQTTAsync_setCallbacks(client, client, connlost, messageArrived, NULL)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to set callback, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    conn_opts->keepAliveInterval = CHECK_ALIVE_TIME;
    conn_opts->cleansession = 1;
    conn_opts->onSuccess = onConnect;
    conn_opts->onFailure = onConnectFailure;
    conn_opts->context = client;

    strcpy(conn_opts->username, MQTT_USERNAME);
    strcpy(conn_opts->password, MQTT_PWD);

    if ((rc = MQTTAsync_connect(client, conn_opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    //MQTTAsync_destroy(&client);
    return rc;
}
