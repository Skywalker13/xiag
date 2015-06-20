ifeq (,$(wildcard ../../config.mak))
$(error "../../config.mak is not present, run configure !")
endif
include ../../config.mak

CFLAGS += -I../ -I../exml

MODNAME = texts

SRCS =  texts/texts.c \
        texts/texts_chapter.c \
        texts/texts_condition.c \
        texts/texts_engine.c \
        texts/texts_entity.c \
        texts/texts_room.c \
        texts/texts_scene.c \
        texts/texts_story.c

OBJS = $(SRCS:.c=.o)

.SUFFIXES: .c .o

all: depend $(MODNAME)

.c.o:
	$(CC) -c $(CFLAGS) $(EXTRACFLAGS) $(OPTFLAGS) -o $@ $<

texts: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -shared -Wl,-soname,$(MODNAME).so -o $(MODNAME)/$(MODNAME).so

clean:
	rm -f $(MODNAME)/*.o $(MODNAME)/*.so
	rm -f .depend

install:
	$(INSTALL) -d $(XIAG_MODULE_DIR)
	$(INSTALL) -c $(MODNAME)/$(MODNAME).so $(XIAG_MODULE_DIR)

uninstall:
	rm -rf $(XIAG_MODULE_DIR)

depend:
	$(CC) -MM $(CFLAGS) $(EXTRACFLAGS) $(SRCS) 1>.depend

.PHONY: texts
.PHONY: clean depend
.PHONY: install uninstall

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif
