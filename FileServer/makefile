CC=gcc
LIB=-L/home/umit/LinuxSocketProgramming/SocketAPI/
INCLUDE=-I/home/umit/LinuxSocketProgramming/SocketAPI/include
LIBNAME=-ltcpsocket -lpthread
SRC=file_serverWithProcess.c

Server: file_serverWithProcess.c
	$(CC) $(SRC) $(INCLUDE) $(LIB) $(LIBNAME) -o server

Client: client.c
	$(CC) client.c $(INCLUDE) $(LIB) $(LIBNAME) -o client
