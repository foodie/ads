CC=g++
SHELL=/bin/sh

IF_PATH=./if
CORE_PATH=./include/core

INCLUDES=-I. -I./include -I./include/hashtable -I${IF_PATH}/ -I${CORE_PATH}/
LIB_PATH=-L./lib

IF_OBJ:=$(patsubst %.cc,%.o, $(wildcard ${IF_PATH}/*.cc))
CORE_OBJ:=$(patsubst %.cpp,%.o, $(wildcard ${CORE_PATH}/*.cpp))

CPPFLAGS=-O3 -g -static -fPIC -finline-functions -Wall -pipe \
        -Wreturn-type -Wtrigraphs -Wformat -Wparentheses -Wpointer-arith \
        -D_XOPEN_SOURE=500 -D_GNU_SOURCE

LDFLAGS= -lm -lcrypt -lpthread -lssl -llog -lconn -lcluster \
		 -lzookeeper_mt -lgflags -lini_config -lz -lprotobuf -lserv -lcap -lfcgi \
         -lmysqlclient -lssl -lxml2 -lcurl -lhiredis

all: mads

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@ $(INCLUDES) $(LIB_PATH)

mads: src/ads_hash.o src/ads_data.o src/ads_func.o src/ads_thread.o \
      src/main.o \
     $(IF_OBJ) $(CORE_OBJ) 
	$(CC) -o $@ $^ $(INCLUDES) $(LIB_PATH) $(GEOIPFLAGS) $(LDFLAGS)

clean:
	rm -rf src/*.o ${CORE_PATH}/*.o mads
