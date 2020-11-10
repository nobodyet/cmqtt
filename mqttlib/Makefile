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


SHELL = /bin/sh
.PHONY: clean, mkdir, install, uninstall, html

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
HEADERS_A = $(HEADERS)


# The names of the four different libraries to be built
MQTTLIB_C = paho-mqtt3c
MQTTLIB_CS = paho-mqtt3cs
MQTTLIB_A = paho-mqtt3a
MQTTLIB_AS = paho-mqtt3as

MQTTLIB_C_TARGET = ${MQTTLIB_C}.so
CC ?= gcc

CCFLAGS_SO += -Wno-deprecated-declarations -DOSX -I /usr/local/opt/openssl/include
LDFLAGS_C += -Wl,-install_name,lib$(MQTTLIB_C).so.${MAJOR_VERSION}
LDFLAGS_CS += -Wl,-install_name,lib$(MQTTLIB_CS).so.${MAJOR_VERSION} -L /usr/local/opt/openssl/lib

LDFLAGS_A += -Wl,-install_name,lib${MQTTLIB_A}.so.${MAJOR_VERSION}
LDFLAGS_AS += -Wl,-install_name,lib${MQTTLIB_AS}.so.${MAJOR_VERSION} -L /usr/local/opt/openssl/lib


CCFLAGS_SO = -g -fPIC $(CFLAGS) -D_GNU_SOURCE -Os -Wall -fvisibility=hidden -DPAHO_MQTT_EXPORTS=1

FLAGS_EXE = $(LDFLAGS) -I ${srcdir} ${START_GROUP} -lpthread ${GAI_LIB} ${END_GROUP} -L ${blddir}
FLAGS_EXES = $(LDFLAGS) -I ${srcdir} ${START_GROUP} -lpthread ${GAI_LIB} -lssl -lcrypto ${END_GROUP} -L ${blddir}

LDCONFIG ?= /sbin/ldconfig
LDFLAGS_C = $(LDFLAGS) -shared  -lpthread  -Wl
LDFLAGS_CS = $(LDFLAGS) -shared $(START_GROUP) -lpthread $(GAI_LIB) $(EXTRA_LIB) -lssl -lcrypto $(END_GROUP) -Wl,-init
LDFLAGS_A = $(LDFLAGS) -shared -Wl,-init,$(MQTTASYNC_INIT) $(START_GROUP) -lpthread $(GAI_LIB) $(END_GROUP)
LDFLAGS_AS = $(LDFLAGS) -shared $(START_GROUP) -lpthread $(GAI_LIB) $(EXTRA_LIB) -lssl -lcrypto $(END_GROUP) -Wl,-init

${MQTTLIB_C_TARGET}: ${SOURCE_FILES_C} ${HEADERS_C} VersionInfo.h
	${CC} ${CCFLAGS_SO} -o $@ ${SOURCE_FILES_C} ${LDFLAGS_C}
	-ln -s $(MQTTLIB_C).so lib$(MQTTLIB_C).so.${VERSION}  
	-ln -s $(MQTTLIB_C).so lib$(MQTTLIB_C).so.${MAJOR_VERSION} 
	-ln -s $(MQTTLIB_C).so lib$(MQTTLIB_C).so 


clean:
	-rm -f ${MQTTLIB_C_TARGET}
	-rm -f lib$(MQTTLIB_C).so.${VERSION} lib$(MQTTLIB_C).so.${MAJOR_VERSION} lib$(MQTTLIB_C).so 
