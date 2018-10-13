include ../Makefile.SUPPORTED

ifeq (,$(findstring $(OS),$(SUPPORTEDPLATFORMS)))

all:

	@echo The OS environment variable is currently set to [$(OS)]
	@echo Please set the OS environment variable to one of the following:
	@echo $(SUPPORTEDPLATFORMS)

.PHONY: all

else

include ../Makefile.$(OS)

LIBSRCFILES=../src/base.c \
	../include/baseencode.h

LIBCFILES=../src/base.c

all: ../bin/test001_testvect$(EXESUFFIX)


../bin/test001_testvect$(EXESUFFIX): test001_testvect.c $(LIBSRCFILES)

	$(CC) $(OPTIONS) -o ../bin/test001_testvect$(EXESUFFIX) test001_testvect.c $(LIBCFILES)

endif
