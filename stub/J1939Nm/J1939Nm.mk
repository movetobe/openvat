BASE_DIR:=$(PWD)
STUB_DIR:=$(BASE_DIR)/stub
DIR:=$(STUB_DIR)/J1939Nm

INCLUDE_DIRS+=$(STUB_DIR)/Common \
			  $(STUB_DIR)/J1939Nm \
			  $(STUB_DIR)/J1939Rm \
			  $(STUB_DIR)/Nm \
			  $(BASE_DIR)/ovat-log \
              $(BASE_DIR)/utilities \

BIN:=$(STUB_DIR)/libj1939nm-stub.so

SHARE:=--share

CFLAGS+=-g -fPIC

LFLAGS+=$(SHARE)

CC:=gcc -o

AR:=ar -cr

CO:=$(CC)

CFLAGS+=$(foreach D,$(INCLUDE_DIRS),$(addprefix -I,$(D)))
SRC:=$(wildcard $(DIR)/*.c)
OBJ:=$(patsubst %.c, %.o, $(SRC))

all:$(BIN)
$(BIN):$(OBJ)
	$(CO) $@ $^ $(LFLAGS)

$(DIR)/%.o:$(DIR)/%.c
	$(CC) $@ -c $< $(CFLAGS)

install:
	cp -rf $(BIN) /usr/lib/

.PHONY:clean
clean:
	rm -rf $(OBJ) $(BIN)
