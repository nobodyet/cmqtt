#*******************************************************************************
#  Copyright (c) 2009, 2020 IBM Corp.
#  #
#  All rights reserved. This program and the accompanying materials
#  #  are made available under the terms of the Eclipse Public License v2.0
#  and Eclipse Distribution License v1.0 which accompany this distribution.                                                     #                                                                                                                               #  The Eclipse Public License is available at
#     https://www.eclipse.org/legal/epl-2.0/
#     #  and the Eclipse Distribution License is available at                                                                         #    http://www.eclipse.org/org/documents/edl-v10.php.
#
##  Contributors:                                                                                                                #     Ian Craggs - initial API and implementation and/or initial documentation
#     Allan Stockdill-Mander - SSL updates
#     #     Andy Piper - various fixes
#     Ian Craggs - OSX build
#     #     Rainer Poisel - support for multi-core builds and cross-compilation
#*******************************************************************************/

# Note: on OS X you should install XCode and the associated command-line tools

#cc -g -fPIC  -D_GNU_SOURCE -Os -Wall -fvisibility=hidden -Ibuild -DPAHO_MQTT_EXPORTS=1 -Wno-deprecated-declarations -DOSX -I /usr/local/opt/openssl/include -o build/output/libpaho-mqtt3a.so.1.3 Base64.c Clients.c Heap.c LinkedList.c Log.c MQTTAsync.c MQTTPacket.c MQTTPacketOut.c MQTTPersistence.c MQTTPersistenceDefault.c MQTTProperties.c MQTTProtocolClient.c MQTTProtocolOut.c MQTTReasonCodes.c MQTTTime.c Messages.c OsWrapper.c SHA1.c Socket.c SocketBuffer.c StackTrace.c Thread.c Tree.c WebSocket.c utf-8.c  -shared -Wl,-init,_MQTTAsync_init  -lpthread   -Wl,-install_name,libpaho-mqtt3a.so.1
#ln -s libpaho-mqtt3a.so.1.3  build/output/libpaho-mqtt3a.so.1
#ln -s libpaho-mqtt3a.so.1 build/output/libpaho-mqtt3a.so

SHELL = /bin/sh
.PHONY: clean, mkdir, install, uninstall, html

# determine current platform
BUILD_TYPE ?= debug
ifeq ($(OS),Windows_NT)
	OSTYPE ?= $(OS)
	MACHINETYPE ?= $(PROCESSOR_ARCHITECTURE)
else
	OSTYPE ?= $(shell uname -s)
	MACHINETYPE ?= $(shell uname -m)
	build.level = $(shell date)
endif # OS
ifeq ($(OSTYPE),linux)
	OSTYPE = Linux
endif

MAJOR_VERSION = $(shell cat version.major)
MINOR_VERSION = $(shell cat version.minor)
PATCH_VERSION = $(shell cat version.patch)
VERSION = ${MAJOR_VERSION}.${MINOR_VERSION}

ifndef release.version
	release.version = $(MAJOR_VERSION).$(MINOR_VERSION).$(PATCH_VERSION) 
endif

SOURCE_FILES = $(wildcard *.c)
SOURCE_FILES_C = $(filter-out MQTTAsync.c MQTTVersion.c SSLSocket.c, $(SOURCE_FILES))
SOURCE_FILES_CS = $(filter-out MQTTAsync.c MQTTVersion.c, $(SOURCE_FILES))
SOURCE_FILES_A = $(filter-out MQTTClient.c MQTTVersion.c SSLSocket.c, $(SOURCE_FILES))
SOURCE_FILES_AS = $(filter-out MQTTClient.c MQTTVersion.c, $(SOURCE_FILES))


HEADERS = ./*.h
HEADERS_C = $(filter-out /MQTTAsync.h, $(HEADERS))
HEADERS_A = $(filter-out /MQTTAsync.h, $(HEADERS))


# The names of the four different libraries to be built
MQTTLIB_C = paho-mqtt3c
MQTTLIB_CS = paho-mqtt3cs
MQTTLIB_A = paho-mqtt3a
MQTTLIB_AS = paho-mqtt3as

MQTTLIB_C_TARGET = ${MQTTLIB_C}.so
MQTTLIB_A_TARGET = ${MQTTLIB_A}.so

CC = gcc

CCFLAGS_SO = -fPIC $(CFLAGS) -D_GNU_SOURCE -Os -Wall  -DPAHO_MQTT_EXPORTS=1
#CCFLAGS_SO = -fPIC $(CFLAGS) -D_GNU_SOURCE -Os -Wall -fvisibility=hidden -DPAHO_MQTT_EXPORTS=1

FLAGS_EXE = $(LDFLAGS) -I ${srcdir} ${START_GROUP} -lpthread ${GAI_LIB} ${END_GROUP} -L ${blddir}
FLAGS_EXES = $(LDFLAGS) -I ${srcdir} ${START_GROUP} -lpthread ${GAI_LIB} -lssl -lcrypto ${END_GROUP} -L ${blddir}


MQTTCLIENT_INIT = MQTTClient_init
MQTTASYNC_INIT = MQTTAsync_init
START_GROUP = -Wl,--start-group
END_GROUP = -Wl,--end-grou


LDFLAGS_C = $(LDFLAGS) -shared  -lpthread  -Wl,-init
LDFLAGS_CS = $(LDFLAGS) -shared $(START_GROUP) -lpthread $(GAI_LIB) $(EXTRA_LIB) -lssl -lcrypto $(END_GROUP) -Wl,-init
LDFLAGS_A = $(LDFLAGS) -shared -Wl,-init,$(MQTTASYNC_INIT) $(START_GROUP) -lpthread $(GAI_LIB) $(END_GROUP)
LDFLAGS_AS = $(LDFLAGS) -shared $(START_GROUP) -lpthread $(GAI_LIB) $(EXTRA_LIB) -lssl -lcrypto $(END_GROUP) -Wl,-init

ifeq ($(OSTYPE),Darwin)
${MQTTLIB_A_TARGET}: ${SOURCE_FILES_C} ${HEADERS_C} VersionInfo.h
##cc -g -fPIC  -D_GNU_SOURCE -Os -Wall -fvisibility=hidden   -DPAHO_MQTT_EXPORTS=1 -Wno-deprecated-declarations -DOSX  -o libpaho-mqtt3a.so.1.3 Base64.c Clients.c Heap.c LinkedList.c Log.c MQTTAsync.c MQTTPacket.c MQTTPacketOut.c MQTTPersistence.c MQTTPersistenceDefault.c MQTTProperties.c MQTTProtocolClient.c MQTTProtocolOut.c MQTTReasonCodes.c MQTTTime.c Messages.c OsWrapper.c SHA1.c Socket.c SocketBuffer.c StackTrace.c Thread.c Tree.c WebSocket.c utf-8.c  -shared -Wl,-init,_MQTTAsync_init  -lpthread   -Wl,-install_name,libpaho-mqtt3a.so.1
# cc -g -fPIC  -D_GNU_SOURCE -Os -Wall -fvisibility=hidden   -DPAHO_MQTT_EXPORTS=1 -Wno-deprecated-declarations -DOSX  -o libpaho-mqtt3a.so.1.3 Base64.c Clients.c Heap.c LinkedList.c Log.c MQTTAsync.c MQTTPacket.c MQTTPacketOut.c MQTTPersistence.c MQTTPersistenceDefault.c MQTTProperties.c MQTTProtocolClient.c MQTTProtocolOut.c MQTTReasonCodes.c MQTTTime.c Messages.c OsWrapper.c SHA1.c Socket.c SocketBuffer.c StackTrace.c Thread.c Tree.c WebSocket.c utf-8.c  -shared -Wl,-init,_MQTTAsync_init  -lpthread
	#${CC} ${CCFLAGS_SO} -o $@ ${SOURCE_FILES_C} ${LDFLAGS_A}
	@$(CC) -fPIC  -D_GNU_SOURCE -Os -Wall -fvisibility=hidden   -DPAHO_MQTT_EXPORTS=1 -Wno-deprecated-declarations -DOSX  -o $@ Base64.c Clients.c Heap.c LinkedList.c Log.c MQTTAsync.c MQTTPacket.c MQTTPacketOut.c MQTTPersistence.c MQTTPersistenceDefault.c MQTTProperties.c MQTTProtocolClient.c MQTTProtocolOut.c MQTTReasonCodes.c MQTTTime.c Messages.c OsWrapper.c SHA1.c Socket.c SocketBuffer.c StackTrace.c Thread.c Tree.c WebSocket.c utf-8.c  -shared -Wl,-init,_MQTTAsync_init  -lpthread
	-ln -s $(MQTTLIB_A).so lib$(MQTTLIB_A).so
	-ln -s $(MQTTLIB_A).so lib$(MQTTLIB_A).so.${VERSION}  
	-ln -s $(MQTTLIB_A).so lib$(MQTTLIB_A).so.${MAJOR_VERSION} 
	@echo -e "\\033[32m $@ for Macos  is OK \\033[0m"
else ifeq ($(OSTYPE),Linux)
${MQTTLIB_A_TARGET}: ${SOURCE_FILES_C} ${HEADERS_C} VersionInfo.h
	@${CC} ${CCFLAGS_SO} -o $@ ${SOURCE_FILES_C} ${LDFLAGS_A}
	-ln -s $(MQTTLIB_A).so lib$(MQTTLIB_A).so.${VERSION}  
	-ln -s $(MQTTLIB_A).so lib$(MQTTLIB_A).so.${MAJOR_VERSION} 
	-ln -s $(MQTTLIB_A).so lib$(MQTTLIB_A).so 
	@echo -e "\\033[32m ${CC} ${CCFLAGS_SO} -o $@ ${LDFLAGS_A} \\033[0m"
	@echo -e "\\033[32m $@ for Linux  is OK \\033[0m"
endif

clean:
	-rm -f ${MQTTLIB_A_TARGET}
	-rm -f lib$(MQTTLIB_A).so.${VERSION} lib$(MQTTLIB_A).so.${MAJOR_VERSION} lib$(MQTTLIB_A).so 

