BASE_DIR:=.
INCLUDE_DIRS+=$(BASE_DIR)/ovat-ctl \
              $(BASE_DIR)/ovat-netsock \
              $(BASE_DIR)/ovat-netsock/netsock \
              $(BASE_DIR)/utilities

CORE_DIR:=$(BASE_DIR)/ovat-core
CORE_DIRS:=$(INCLUDE_DIRS) $(CORE_DIR)
TOOLS_DIR:=$(BASE_DIR)/tools
TOOLS_DIRS:=$(INCLUDE_DIRS) $(TOOLS_DIR)

CORE_BIN:=${CORE_DIR}/ovat-core
APPCTL_BIN:=$(TOOLS_DIR)/ovat-appctl

CFLAGS+=-g -Wall -fPIC

LFLAGS+=-lpthread

CC:=gcc

AR:=ar -cr

CO:=$(CC) -o

CORE_SRC:=$(foreach DIR,$(CORE_DIRS),$(wildcard $(DIR)/*.c))
CORE_OBJ:=$(patsubst %.c, %.o, $(CORE_SRC))

TOOLS_SRC:=$(foreach DIR,$(TOOLS_DIRS),$(wildcard $(DIR)/*.c))
TOOLS_OBJ:=$(patsubst %.c, %.o, $(TOOLS_SRC))

CFLAGS+=$(foreach DIR,$(INCLUDE_DIRS),$(addprefix -I,$(DIR)))

all:$(CORE_BIN) $(APPCTL_BIN)
$(CORE_BIN):$(CORE_OBJ)
	$(CO) $@ $^ $(LFLAGS)
$(APPCTL_BIN):$(TOOLS_OBJ)
	$(CO) $@ $^ $(LFLAGS)

%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@

install:
	cp $(CORE_BIN) /usr/bin/
	cp $(APPCTL_BIN) /usr/bin/

.PHONY:clean
clean:
	rm $(CORE_OBJ) $(CORE_BIN) $(TOOLS_OBJ) $(APPCTL_BIN)
