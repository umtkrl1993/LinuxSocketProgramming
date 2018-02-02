CC=g++
CFLAGS=-c -std=c++11 -fPIC
INCLUDE_PATH=-I./include
INCLUDE=./include
VPATH=src
OBJECTS=ISocketHandler.o ISocketFactory.o
LIBNAME=libcppsocket.so
SYSTEM_LIBRARY_PATH=/usr/local/lib
SYSTEM_INCLUDE_PATH=/usr/local/include


$(LIBNAME): $(OBJECTS)
	$(CC) -shared $(OBJECTS) -o $(LIBNAME) 

$(OBJECTS): %.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $^ -o $@ 


.PHONY:install

install:
	cp $(INCLUDE)/* $(SYSTEM_INCLUDE_PATH)
	cp $(LIBNAME) $(SYSTEM_LIBRARY_PATH)
	ldconfig
