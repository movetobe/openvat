BASE_DIR:=$(PWD)
STUB_DIR:=$(BASE_DIR)/stub
.PHONY:all
all:
	make all -f $(STUB_DIR)/CanNm/CanNm.mk
	make all -f $(STUB_DIR)/Nm/Nm.mk
	make all -f $(STUB_DIR)/BswM/BswM.mk
	make all -f $(STUB_DIR)/CanIf/CanIf.mk
	make all -f $(STUB_DIR)/CanSM/CanSM.mk
	make all -f $(STUB_DIR)/Com/Com.mk
	make all -f $(STUB_DIR)/Det/Det.mk
	make all -f $(STUB_DIR)/PduR/PduR.mk
	make all -f $(STUB_DIR)/ComM/ComM.mk
	make all -f $(STUB_DIR)/Can/Can.mk
	make all -f $(STUB_DIR)/Dem/Dem.mk
	make all -f $(STUB_DIR)/J1939Nm/J1939Nm.mk
install:
	make install -f $(STUB_DIR)/CanNm/CanNm.mk
	make install -f $(STUB_DIR)/Nm/Nm.mk
	make install -f $(STUB_DIR)/BswM/BswM.mk
	make install -f $(STUB_DIR)/CanIf/CanIf.mk
	make install -f $(STUB_DIR)/CanSM/CanSM.mk
	make install -f $(STUB_DIR)/Com/Com.mk
	make install -f $(STUB_DIR)/Det/Det.mk
	make install -f $(STUB_DIR)/PduR/PduR.mk
	make install -f $(STUB_DIR)/ComM/ComM.mk
	make install -f $(STUB_DIR)/Can/Can.mk
	make install -f $(STUB_DIR)/Dem/Dem.mk
	make install -f $(STUB_DIR)/J1939Nm/J1939Nm.mk

.PHONY:clean
clean:
	make clean -f $(STUB_DIR)/CanNm/CanNm.mk
	make clean -f $(STUB_DIR)/Nm/Nm.mk
	make clean -f $(STUB_DIR)/BswM/BswM.mk
	make clean -f $(STUB_DIR)/CanIf/CanIf.mk
	make clean -f $(STUB_DIR)/CanSM/CanSM.mk
	make clean -f $(STUB_DIR)/Com/Com.mk
	make clean -f $(STUB_DIR)/Det/Det.mk
	make clean -f $(STUB_DIR)/PduR/PduR.mk
	make clean -f $(STUB_DIR)/ComM/ComM.mk
	make clean -f $(STUB_DIR)/Can/Can.mk
	make clean -f $(STUB_DIR)/Dem/Dem.mk
	make clean -f $(STUB_DIR)/J1939Nm/J1939Nm.mk
