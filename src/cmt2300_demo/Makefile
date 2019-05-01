TARGET = bin/main
CC = gcc
CFLAGS = -Wall -g

INC_DIRS = -I src/platform -I src/periph -I src/radio -I objs
INC_LIBS = -lwiringPi -lpthread
OBJS = objs/main.o \
	objs/cmt_spi3.o \
	objs/system.o \
	objs/common.o \
	objs/cmt2300a_hal.o \
	objs/cmt2300a.o \
	objs/radio.o \
	objs/radio_list.o

#$(TARGET):$(OBJS)
#    $(CC) $(OBJS)  -o $(TARGET)

#main.o: main.c
#    $(CC) $(CFLAGS) $(INCLUDE) -c src/platform/main.c

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)  $(INC_DIRS) $(INC_LIBS)
	
objs/main.o: src/platform/main.c
	$(CC) $(CFLAGS) -c src/platform/main.c -o objs/main.o  $(INC_DIRS)

objs/cmt_spi3.o: src/periph/cmt_spi3.c
	$(CC) $(CFLAGS) -c src/periph/cmt_spi3.c -o objs/cmt_spi3.o $(INC_DIRS)

objs/system.o: src/platform/system.c
	$(CC) $(CFLAGS) -c src/platform/system.c -o objs/system.o $(INC_DIRS)

objs/common.o: src/platform/common.c
	$(CC) $(CFLAGS) -c src/platform/common.c -o objs/common.o $(INC_DIRS)

objs/cmt2300a_hal.o: src/radio/cmt2300a_hal.c
	$(CC) $(CFLAGS) -c src/radio/cmt2300a_hal.c -o objs/cmt2300a_hal.o $(INC_DIRS)

objs/cmt2300a.o: src/radio/cmt2300a.c
	$(CC) $(CFLAGS) -c src/radio/cmt2300a.c -o objs/cmt2300a.o $(INC_DIRS)

objs/radio.o: src/radio/radio.c
	$(CC) $(CFLAGS) -c src/radio/radio.c -o objs/radio.o $(INC_DIRS)

objs/radio_list.o: src/radio/radio_list.c
	$(CC) $(CFLAGS) -c src/radio/radio_list.c -o objs/radio_list.o $(INC_DIRS)

clean:
	@rm -rf objs/*.o $(TARGET)
