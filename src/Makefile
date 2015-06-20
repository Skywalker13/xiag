ifeq (,$(wildcard ../config.mak))
$(error "../config.mak is not present, run configure !")
endif
include ../config.mak

CFLAGS += -Isrc -Iexml -I. \
          -DXIAG_MODULE_DIR=\"$(XIAG_MODULE_DIR)\" \
          -DXIAG_DATA_DIR=\"$(XIAG_DATA_DIR)\"

APPNAME = xiag

SRCS =  action.c \
	chapter.c \
	condition.c \
	engine.c \
	entity.c \
	interface.c \
	inventory.c \
	logs.c \
	module.c \
	result.c \
	room.c \
	scene.c \
	story.c \
	utils.c \
	xiag.c \
	xiag_module.c \
	xml_utils.c \
	exml/exmllexer.c \
	exml/exmlparser.c \
	interfaces/terminal.c

OBJS = $(SRCS:.c=.o)

.SUFFIXES: .c .o

all: depend $(APPNAME)

.c.o:
	$(CC) -c $(CFLAGS) $(EXTRACFLAGS) $(OPTFLAGS) -o $@ $<

xiag: $(OBJS) modules
	$(CC) $(OBJS) $(LDFLAGS) $(OPTLDFLAGS) $(EXTRALIBS) -o $(APPNAME)

modules:
	$(MAKE) -C modules

clean:
	$(MAKE) -C modules clean
	rm -f *.o exml/*.o interfaces/*.o $(APPNAME)
	rm -f .depend

install:
	$(MAKE) -C modules install
	$(INSTALL) -d $(bindir)
	$(INSTALL) $(APPNAME) $(bindir)
	$(STRIP) $(INSTALLSTRIP) $(bindir)/$(APPNAME)

	$(INSTALL) -d $(XIAG_DATA_DIR)/story
	$(INSTALL) ../data/story/*.xml $(XIAG_DATA_DIR)/story

uninstall:
	$(MAKE) -C modules uninstall
	rm -f $(bindir)/$(APPNAME)
	rm -rf $(XIAG_DATA_DIR)

depend:
	$(CC) -MM $(CFLAGS) $(EXTRACFLAGS) $(SRCS) 1>.depend

.PHONY: xiag modules
.PHONY: clean depend
.PHONY: install uninstall

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif
