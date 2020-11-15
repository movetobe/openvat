all:
	make all -f ./stub/modules.mk
	make all -f ./ovat-core/core.mk
	make all -f ./tools/tools.mk

install:
	make install -f ./stub/modules.mk
	make install -f ./ovat-core/core.mk
	make install -f ./tools/tools.mk

.PHONY:clean
clean:
	make clean -i -f ./ovat-core/core.mk
	make clean -i -f ./tools/tools.mk
	make clean -i -f ./stub/modules.mk
