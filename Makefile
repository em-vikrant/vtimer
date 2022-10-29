# Makefile to compile Timer Application

CC := gcc

APP_BIN := vtimer
APP_PATH := .
SRC_PATH := $(APP_PATH)/src
INC_PATH := $(APP_PATH)/inc
BUILD_PATH := $(APP_PATH)/build
BUILD_PATH_O := $(BUILD_PATH)/src

STD_LIB := -lm -lpthread
CFLAGS := --std=gnu99 --std=c99

clean:
	rm -fr $(BUILD_PATH_O)/*o
	rm -fr $(BUILD_PATH)/$(APP_BIN)

timer:
	$(CC) $(CFLAGS) -c $(SRC_PATH)/hw_timer.c -I$(INC_PATH) -o $(BUILD_PATH_O)/hw_timer.o $(STD_LIB)

vtimer:
	$(CC) $(CFLAGS) -c $(SRC_PATH)/vtimer.c -I$(INC_PATH) -o $(BUILD_PATH_O)/vtimer.o $(STD_LIB)

main:
	$(CC) $(CFLAGS) -c $(SRC_PATH)/main.c -I$(INC_PATH) -o $(BUILD_PATH_O)/main.o $(STD_LIB)

all: timer vtimer main
	$(CC) $(CFLAGS) $(BUILD_PATH_O)/*.o -o $(BUILD_PATH)/$(APP_BIN) $(STD_LIB)
