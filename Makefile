CC=g++
SHELL=/bin/sh

IF_PATH=./if
CORE_PATH=./core
CONTROLLER_PATH=./src/controller
STRUCT_PATH=./src/data-struct
ADX_PATH=./src/adx

INCLUDES=-I. -I./include -I./hashtable -I${IF_PATH}/ -I${CORE_PATH}/ -I${CONTROLLER_PATH}/ -I${STRUCT_PATH}/ -I${ADX_PATH}/
LIB_PATH=-L./lib

IF_OBJ:=$(patsubst %.cc,%.o, $(wildcard ${IF_PATH}/*.cc))
CORE_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${CORE_PATH}/*.cpp))
CONTROLLER_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${CONTROLLER_PATH}/*.cpp))
STRUCT_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${STRUCT_PATH}/*.cpp))
ADX_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${ADX_PATH}/*.cpp))

CPPFLAGS=-O3 -g -static -fPIC -finline-functions -Wall -pipe \
        -Wreturn-type -Wtrigraphs -Wformat -Wparentheses -Wpointer-arith \
        -D_XOPEN_SOURE=500 -D_GNU_SOURCE

LDFLAGS= -lm -lcrypt -lpthread -lssl -llog -lconn -lcluster \
		 -lzookeeper_mt -lgflags -lini_config -lz -lprotobuf -lserv -lcap -lfcgi \
         -lmysqlclient -lcoding -lssl -lxml2 -lcurl -lhiredis

all: mads

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@ $(INCLUDES) $(LIB_PATH)

mads: src/ads_hash.o src/ads_data.o src/ads_func.o src/ads_thread.o \
     src/decrypter.o src/ads_crontab_handler.o src/main.o \
     $(IF_OBJ) $(CORE_OBJ) $(CONTROLLER_OBJ) $(STRUCT_OBJ) $(ADX_OBJ)
	$(CC) -o $@ $^ $(INCLUDES) $(LIB_PATH) $(GEOIPFLAGS) $(LDFLAGS)

clean:
	rm -rf src/*.o ${CORE_PATH}/*.o ${CONTROLLER_PATH}/*.o ${STRUCT_PATH}/*.o ${ADX_PATH}/*.o mads
