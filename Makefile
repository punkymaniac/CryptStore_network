
PATH_DAEMON = ./daemon/
PATH_CLIENT = ./client/

ifneq ($(DEBUG), true)
	DEBUG = false
endif
ifneq ($(PROF), true)
	PROF = false
endif

all:
	@DEBUG=$(DEBUG) PROF=$(PROF) make -C $(PATH_DAEMON)
	@DEBUG=$(DEBUG) PROF=$(PROF) make -C $(PATH_CLIENT)

help:
	@echo "Option variable:"
	@echo " DEBUG=true\t: debug compilation"
	@echo " PROF=true\t: profiling compilation"

clean:
	@DEBUG=$(DEBUG) PROF=$(PROF) make clean -C $(PATH_DAEMON)
	@DEBUG=$(DEBUG) PROF=$(PROF) make clean -C $(PATH_CLIENT)

mrproper:
	@DEBUG=$(DEBUG) PROF=$(PROF) make mrproper -C $(PATH_DAEMON)
	@DEBUG=$(DEBUG) PROF=$(PROF) make mrproper -C $(PATH_CLIENT)

re:
	@DEBUG=$(DEBUG) PROF=$(PROF) make re -C $(PATH_DAEMON)
	@DEBUG=$(DEBUG) PROF=$(PROF) make re -C $(PATH_CLIENT)

install:
	@DEBUG=$(DEBUG) PROF=$(PROF) make install -C $(PATH_DAEMON)
	@DEBUG=$(DEBUG) PROF=$(PROF) make install -C $(PATH_CLIENT)

uninstall:
	@DEBUG=$(DEBUG) PROF=$(PROF) make uninstall -C $(PATH_DAEMON)
	@DEBUG=$(DEBUG) PROF=$(PROF) make uninstall -C $(PATH_CLIENT)
