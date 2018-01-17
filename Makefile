CC=g++
SHELL=/bin/sh

IF_PATH=./if
UTIL_PATH=./include/utils

CORE_PATH=./include/core
ADVERTISE_PATH=./include/core/advertise
BIDDING_PATH=./include/core/bidding
MONITOR_PATH=./include/core/monitor

CONTROLLER_PATH=./include/plugins/controller
EXCHANGE_PATH=./include/plugins/exchange
CRONTAB_PATH=./include/plugins/corntab

IF_OBJ:=$(patsubst %.cc,%.o, $(wildcard ${IF_PATH}/*.cc))
UTIL_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${UTIL_PATH}/*.cpp))

CORE_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${CORE_PATH}/*.cpp))
ADVERTISE_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${ADVERTISE_PATH}/*.cpp))
BIDDING_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${BIDDING_PATH}/*.cpp))
MONITOR_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${MONITOR_PATH}/*.cpp))

CONTROLLER_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${CONTROLLER_PATH}/*.cpp))
EXCHANGE_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${EXCHANGE_PATH}/*.cpp))
CRONTAB_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${CRONTAB_PATH}/*.cpp))

INCLUDES=-I. -I./include -I./include/hashtable -I${IF_PATH}/ -I${CORE_PATH}/ 
LIB_PATH=-L./lib

CPPFLAGS= -std=c++11 -O0 -g -static -fPIC -finline-functions -pipe \
        -Wreturn-type -Wtrigraphs -Wformat -Wparentheses -Wpointer-arith \
        -D_XOPEN_SOURE=500 -D_GNU_SOURCE -Wno-narrowing

LDFLAGS= -ldl -lm -lcrypto -lpthread -lssl -llog -lconn -lcluster \
		 -lzookeeper_mt -lgflags -lini_config -lz -lprotobuf -lserv -lcap -lfcgi \
         -lmysqlclient -lssl -lxml2 -lcurl -lhiredis

all: mads

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@ $(INCLUDES) $(LIB_PATH)

mads: src/ads_conf.o src/ads_hash.o src/ads_data.o src/ads_func.o src/ads_thread.o \
      src/main.o \
     $(IF_OBJ) $(UTIL_OBJ) \
     $(CORE_OBJ) $(ADVERTISE_OBJ) $(BIDDING_OBJ) $(BIDDING_OBJ) $(MONITOR_OBJ) \
     $(CONTROLLER_OBJ) $(EXCHANGE_OBJ) $(CRONTAB_OBJ) 
	$(CC) -o $@ $^ $(INCLUDES) $(LIB_PATH) $(GEOIPFLAGS) $(LDFLAGS)

clean:
	rm -rf src/*.o ${UTIL_PATH}/*.o mads \
	 	${CORE_PATH}/*.o ${ADVERTISE_PATH}/*.o ${BIDDING_PATH}/*.o ${MONITOR_PATH}/*.o \
		${CONTROLLER_PATH}/*.o ${EXCHANGE_PATH}/*.o ${CRONTAB_PATH}/*.o
