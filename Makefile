# Makefile to compile Timer Application

CC := gcc

APP_BIN := vtimer
APP_PATH := .
SRC_PATH := $(APP_PATH)/src
INC_PATH := $(APP_PATH)/inc
BUILD_PATH := $(APP_PATH)/build
BUILD_PATH_O := $(BUILD_PATH)/src
TEST_PATH := $(APP_PATH)/test
BUILD_PATH_T := $(BUILD_PATH)/test

STD_LIB := -lm -lpthread
CFLAGS := -std=gnu99
CC_GLOBAL_DEFINES := -DENABLE_TIMER_TEST

clean:
	$(info Cleaning the build directory)
	rm -fr $(BUILD_PATH_O)/*.o
	rm -fr $(BUILD_PATH)/$(APP_BIN)
	rm -fr $(BUILD_PATH)

build_create:
	$(info Creating build directory)
	mkdir -p $(BUILD_PATH)
	mkdir -p $(BUILD_PATH)/src
	mkdir -p $(BUILD_PATH)/test

timer:
	$(CC) $(CFLAGS) $(CC_GLOBAL_DEFINES) -c $(SRC_PATH)/hw_timer.c -I$(INC_PATH) -o $(BUILD_PATH_O)/hw_timer.o $(STD_LIB)

vtimer:
	$(CC) $(CFLAGS) $(CC_GLOBAL_DEFINES) -c $(SRC_PATH)/vtimer.c -I$(INC_PATH) -o $(BUILD_PATH_O)/vtimer.o $(STD_LIB)

main:
	$(CC) $(CFLAGS) $(CC_GLOBAL_DEFINES) -c $(SRC_PATH)/main.c -I$(INC_PATH) -o $(BUILD_PATH_O)/main.o $(STD_LIB)

timer_test:
	$(CC) $(CFLAGS) $(CC_GLOBAL_DEFINES) -c $(TEST_PATH)/timer_client.c -I$(INC_PATH) -o $(BUILD_PATH_T)/timer_client.o $(STD_LIB)

msg:
	$(info Compiling the application)

app: build_create msg timer vtimer timer_test main
	$(info Creating the application binary)
	$(CC) $(CFLAGS) $(CC_GLOBAL_DEFINES) $(BUILD_PATH_O)/*.o $(BUILD_PATH_T)/*.o -o $(BUILD_PATH)/$(APP_BIN) $(STD_LIB)

all: app
	$(info Compilation complete)


