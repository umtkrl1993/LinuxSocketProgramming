CC=gcc
CFLAGS=-c -fPIC
VPATH=src
INCLUDE_PATH=-I./include
OBJECTS=socket.o
LIBNAME=libtcpsocket.so


$(LIBNAME): $(OBJECTS)
	$(CC) -shared $(OBJECTS) -o $(LIBNAME) 

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $^ -o $@ 















