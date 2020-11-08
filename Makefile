BASE_DIR=.
CORE_DIR=./ovat-core
CTL_DIR=./ovat-ctl
NET_DIR=./ovat-netsock
TOOLS_DIR=./tools
UTIL_DIR=./utilities

CORE_BIN=${CORE_DIR}/ovat-core
APPCTL_BIN=$(TOOLS_DIR)/ovat-appctl

CFLAG=-g -Wall -fPIC

LFLAG=-lpthread

CC=gcc

AR=ar -cr

CO=$(CC) -o

CORE_SRC=$(wildcard $(CORE_DIR)/*.c)
CORE_OBJ=$(patsubst %.c, %.o, $(CORE_SRC))

CTL_SRC=$(wildcard $(CTL_DIR)/*.c)
CTL_OBJ=$(patsubst %.c, %.o, $(CTL_SRC))

NET_SRC=$(wildcard $(NET_DIR)/*.c $(NET_DIR)/netsock/*.c)
NET_OBJ=$(patsubst %.c, %.o, $(NET_SRC))

TOOLS_SRC=$(wildcard $(TOOLS_DIR)/*.c)
TOOLS_OBJ=$(patsubst %.c, %.o, $(TOOLS_SRC))

UTIL_SRC=$(wildcard $(UTIL_DIR)/*.c)
UTIL_OBJ=$(patsubst %.c, %.o, $(UTIL_SRC))

CFLAG+=-I$(CORE_DIR) -I$(CTL_DIR) -I$(NET_DIR) -I$(NET_DIR)/netsock -I$(UTIL_DIR)

all:$(CORE_BIN) $(APPCTL_BIN)
$(CORE_BIN):$(CORE_OBJ)$(CTL_OBJ)$(NET_OBJ)$(UTIL_OBJ)
	$(CO) $@ $^ $(LFLAG)
$(APPCTL_BIN):$(TOOLS_OBJ)$(NET_OBJ)$(UTIL_OBJ)$(CTL_OBJ)
	$(CO) $@ $^ $(LFLAG)

$(CORE_DIR)/%.o:$(CORE_DIR)/%.c
	$(CC) $(CFLAG) -o $@ -c $^

$(CTL_DIR)/%.o:$(CTL_DIR)/%.c
	$(CC) $(CFLAG) -o $@ -c $^

$(NET_DIR)/%.o:$(NET_DIR)/%.c
	$(CC) $(CFLAG) -o $@ -c $^

$(NET_DIR)/netsock/%.o:$(NET_DIR)/netsock/%.c
	$(CC) $(CFLAG) -o $@ -c $^

$(TOOLS_DIR)/%.o:$(TOOLS_DIR)/%.c
	$(CC) $(CFLAG) -o $@ -c $^

$(UTIL_DIR)/%.o:$(UTIL_DIR)/%.c
	$(CC) $(CFLAG) -o $@ -c $^

install:
	cp $(CORE_BIN) /usr/bin/
	cp $(APPCTL_BIN) /usr/bin/

.PHONY:clean
clean:
	rm $(CORE_OBJ) $(CORE_BIN) $(CTL_OBJ) $(NET_OBJ) $(TOOLS_OBJ) $(UTIL_OBJ) $(APPCTL_BIN)