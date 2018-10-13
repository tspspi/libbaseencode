include Makefile.SUPPORTED

ifeq (,$(findstring $(OS),$(SUPPORTEDPLATFORMS)))

all:

	@echo The OS environment variable is currently set to [$(OS)]
	@echo Please set the OS environment variable to one of the following:
	@echo $(SUPPORTEDPLATFORMS)

.PHONY: all

else

include Makefile.$(OS)

OBJFILES=tmp/base$(OBJSUFFIX)

all: staticlib tests

staticlib: $(OBJFILES)

	$(ARCMD) bin/libbaseencode$(SLIBSUFFIX) $(OBJFILES)

tests:

	- @$(MAKE) -C ./tests

tmp/base$(OBJSUFFIX): src/base.c include/baseencode.h

	$(CCLIB) $(OPTIONS) -c -o tmp/base$(OBJSUFFIX) src/base.c

.PHONY: tests

endif
