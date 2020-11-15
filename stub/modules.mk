BASE_DIR:=$(PWD)
STUB_DIR:=$(BASE_DIR)/stub
.PHONY:all
all:
	make all -f $(STUB_DIR)/CanNm/CanNm.mk
	make all -f $(STUB_DIR)/Nm/Nm.mk 
install:
	make install -f $(STUB_DIR)/CanNm/CanNm.mk
	make install -f $(STUB_DIR)/Nm/Nm.mk

.PHONY:clean
clean:
	make clean -f $(STUB_DIR)/CanNm/CanNm.mk
	make clean -f $(STUB_DIR)/Nm/Nm.mk
