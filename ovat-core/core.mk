BASE_DIR:=$(PWD)
INCLUDE_DIRS+=$(BASE_DIR)/ovat-ctl \
              $(BASE_DIR)/ovat-netsock \
              $(BASE_DIR)/ovat-netsock/netsock \
              $(BASE_DIR)/utilities \
              $(BASE_DIR)/include \
              $(BASE_DIR)/ovat-if

CORE_DIR:=$(BASE_DIR)/ovat-core
CORE_DIRS:=$(INCLUDE_DIRS) $(CORE_DIR)

CORE_BIN:=${CORE_DIR}/ovat-core

CFLAGS+=-g -Wall -fPIC
LD_LIBRARY_PATH:=$(BASE_DIR)/stub

LFLAGS+=-L$(LD_LIBRARY_PATH) -lcannm-stub -lnm-stub -lpthread

CC:=gcc

AR:=ar -cr

CO:=$(CC) -o

CORE_SRC:=$(foreach DIR,$(CORE_DIRS),$(wildcard $(DIR)/*.c))
CORE_OBJ:=$(patsubst %.c, %.o, $(CORE_SRC))

CFLAGS+=$(foreach DIR,$(INCLUDE_DIRS),$(addprefix -I,$(DIR)))

all:$(CORE_BIN)
$(CORE_BIN):$(CORE_OBJ)
	$(CO) $@ $^ $(LFLAGS)

%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@

install:
	cp $(CORE_BIN) /usr/bin/

.PHONY:clean
clean:
	rm $(CORE_OBJ) $(CORE_BIN)
