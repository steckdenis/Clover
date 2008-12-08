TOP = .
include $(TOP)/configs/current


CL_SOURCES = \
        src/api_command.c \
        src/api_context.c \
        src/api_device.c \
        src/api_enqueue.c \
        src/api_event.c \
        src/api_flush.c \
        src/api_kernel.c \
        src/api_memory.c \
        src/api_platform.c \
        src/api_profiling.c \
        src/api_program.c \
        src/api_sampler.c \
        cpuwinsys/cpuwinsys.c


### All the core C sources

ALL_SOURCES = \
        $(CL_SOURCES)


### Object files
CL_OBJECTS = \
	$(CL_SOURCES:.c=.o)

### Include directories

INCLUDE_DIRS = \
	-I$(TOP)/include \
	-I$(GALLIUM)/include \
	-I$(GALLIUM)/src/gallium/include \
	-I$(GALLIUM)/src/gallium/auxiliary

CL_LIB = OpenCL
CL_LIB_NAME = lib$(CL_LIB).so

CL_MAJOR = 1
CL_MINOR = 0
CL_TINY = 0

GALLIUM_LIBS = \
	$(GALLIUM)/src/gallium/auxiliary/pipebuffer/libpipebuffer.a \
	$(GALLIUM)/src/gallium/auxiliary/sct/libsct.a \
	$(GALLIUM)/src/gallium/auxiliary/draw/libdraw.a \
	$(GALLIUM)/src/gallium/auxiliary/rtasm/librtasm.a \
	$(GALLIUM)/src/gallium/auxiliary/translate/libtranslate.a \
	$(GALLIUM)/src/gallium/auxiliary/cso_cache/libcso_cache.a \
	$(GALLIUM)/src/gallium/auxiliary/tgsi/libtgsi.a \
	$(GALLIUM)/src/gallium/auxiliary/util/libutil.a

.SUFFIXES : .cpp

.c.o:
	$(CC) -c $(INCLUDE_DIRS) $(CFLAGS) $< -o $@

.cpp.o:
	$(CXX) -c $(INCLUDE_DIRS) $(CXXFLAGS) $< -o $@

.S.o:
	$(CC) -c $(INCLUDE_DIRS) $(CFLAGS) $< -o $@


default: depend subdirs $(TOP)/$(LIB_DIR)/$(CL_LIB_NAME)

# Make the OpenCL library
$(TOP)/$(LIB_DIR)/$(CL_LIB_NAME): $(CL_OBJECTS) $(GALLIUM_LIBS)
	$(TOP)/bin/mklib -o $(CL_LIB) \
		-major $(CL_MAJOR) \
		-minor $(CL_MINOR) \
		-patch $(CL_TINY) \
		-install $(TOP)/$(LIB_DIR) \
		$(CL_OBJECTS) $(GALLIUM_LIBS) \
		-Wl,--whole-archive $(LIBS) -Wl,--no-whole-archive $(SYS_LIBS)

######################################################################
# Generic stuff

depend: $(ALL_SOURCES)
	@ echo "running $(MKDEP)"
	@ rm -f depend  # workaround oops on gutsy?!?
	@ touch depend
	@ $(MKDEP) $(MKDEP_OPTIONS) $(DEFINES) $(INCLUDE_DIRS) $(ALL_SOURCES) \
		> /dev/null 2>/dev/null


subdirs:

install: default
	$(INSTALL) -d $(INSTALL_DIR)/include/OpenCL
	$(INSTALL) -d $(INSTALL_DIR)/$(LIB_DIR)
	$(INSTALL) -m 644 $(TOP)/include/OpenCL/*.h $(INSTALL_DIR)/include/OpenCL
	@if [ -e $(TOP)/$(LIB_DIR)/$(CL_LIB_NAME) ]; then \
		$(INSTALL) $(TOP)/$(LIB_DIR)/libOpenCL* $(INSTALL_DIR)/$(LIB_DIR); \
	fi

# Emacs tags
tags:
	etags `find . -name \*.[ch]` $(TOP)/include/OpenCL/*.h

clean:
	-rm -f */*.o
	-rm -f */*/*.o
	-rm -f depend depend.bak

include depend

