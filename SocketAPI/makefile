CC=gcc
CFLAGS=-c -fPIC
VPATH=src
INCLUDE_PATH=-I./include
OBJECTS=socket.o
LIBNAME=libtcpsocket.so
SYSTEM_LIBRARY_PATH=/usr/local/lib
SYSTEM_INCLUDE_PATH=/usr/local/include
HEADER_FILE=./include/socket.h
HEADER_FILE_NAME=socket.h


$(LIBNAME): $(OBJECTS)
	$(CC) -shared $(OBJECTS) -o $(LIBNAME) 

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $^ -o $@ 



.PHONY:install

install:
	cp $(HEADER_FILE) $(SYSTEM_INCLUDE_PATH)
	cp $(LIBNAME) $(SYSTEM_LIBRARY_PATH)
	ldconfig

.PHONY:remove

remove:
	rm -rf $(SYSTEM_LIBRARY_PATH)/$(LIBNAME)
	rm -rf $(SYSTEM_INCLUDE_PATH)/$(HEADER_FILE_NAME)
	ldconfig











