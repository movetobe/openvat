BASE_DIR:=$(PWD)
STUB_DIR:=$(BASE_DIR)/stub
DIR:=$(STUB_DIR)/CanNm

INCLUDE_DIRS+=$(STUB_DIR)/Common \
			  $(STUB_DIR)/CanNm \
			  $(STUB_DIR)/Nm \
			  $(STUB_DIR)/CanIf \
			  $(BASE_DIR)/ovat-log \
              $(BASE_DIR)/utilities \

BIN:=$(STUB_DIR)/libcannm-stub.so

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
