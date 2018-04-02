
CC=gcc
CFLAGS=-g -Wall
OBJECT_FILES=processorUtilities.o packetProcessorParallel.o aes256.o
INCLUDE_PATH=-I./include
VPATH=src
EXECUTABLE=packetProcessorParallel

EXECUTABLE:$(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) -lpthread  -o $(EXECUTABLE)

$(OBJECT_FILES): %.o:%.c
	$(CC) -c $(INCLUDE_PATH) $^ -o $@










