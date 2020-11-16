BASE_DIR:=$(PWD)

INCLUDE_DIRS+=$(BASE_DIR)/ovat-ctl \
			  $(BASE_DIR)/ovat-netsock \
              $(BASE_DIR)/ovat-netsock/netsock \
              $(BASE_DIR)/utilities \
              $(BASE_DIR)/ovat-log

TOOLS_DIR:=$(BASE_DIR)/tools
TOOLS_DIRS:=$(INCLUDE_DIRS) $(TOOLS_DIR)

APPCTL_BIN:=$(TOOLS_DIR)/ovat-appctl

CFLAGS+=-g -Wall -fPIC

LFLAGS+=-lpthread

CC:=gcc

AR:=ar -cr

CO:=$(CC) -o

TOOLS_SRC:=$(foreach DIR,$(TOOLS_DIRS),$(wildcard $(DIR)/*.c))
TOOLS_OBJ:=$(patsubst %.c, %.o, $(TOOLS_SRC))

CFLAGS+=$(foreach DIR,$(INCLUDE_DIRS),$(addprefix -I,$(DIR)))

all:$(APPCTL_BIN)
$(APPCTL_BIN):$(TOOLS_OBJ)
	$(CO) $@ $^ $(LFLAGS)

%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@

install:
	cp $(APPCTL_BIN) /usr/bin/

.PHONY:clean
clean:
	rm $(TOOLS_OBJ) $(APPCTL_BIN)
