ifeq (,$(wildcard config.mak))
$(error "config.mak is not present, run configure !")
endif
include config.mak

all: xiag

xiag:
	$(MAKE) -C src

clean:
	$(MAKE) -C src clean

distclean: clean
	rm -f config.log
	rm -f config.mak

install:
	$(MAKE) -C src install

uninstall:
	$(MAKE) -C src uninstall

.PHONY: clean distclean
.PHONY: install uninstall
