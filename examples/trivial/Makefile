TOP = ../..
include $(TOP)/configs/current


LIBS = -L$(TOP)/$(LIB_DIR) -l$(CL_LIB) $(APP_LIB_DEPS)

SOURCES = \
	basic.c

PROGS = $(SOURCES:%.c=%)

INCLUDES = -I. -I$(TOP)/include -I../samples


##### RULES #####

.SUFFIXES:
.SUFFIXES: .c

.c:
	$(APP_CC) $(INCLUDES) $(CFLAGS) $(LDFLAGS) $< $(LIBS) -o $@

.c.o:
	$(APP_CC) -c $(INCLUDES) $(CFLAGS) $(DEFINES) $< -o $@

.S.o:
	$(APP_CC) -c $(INCLUDES) $(CFLAGS) $(DEFINES)  $< -o $@


##### TARGETS #####

default: $(PROGS)

clean:
	-rm -f $(PROGS)
	-rm -f *.o

# Emacs tags
tags:
	etags `find . -name \*.[ch]` `find ../include`
