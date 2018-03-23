
CC=gcc
CFLAGS=-g -Wall
OBJECT_FILES=processorUtilities.o packetProcessor.o
INCLUDE_PATH=-I./include
VPATH=src
EXECUTABLE=packetProcessor

EXECUTABLE:$(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES)  -o $(EXECUTABLE)

$(OBJECT_FILES): %.o:%.c
	$(CC) -c $(INCLUDE_PATH) $^ -o $@










