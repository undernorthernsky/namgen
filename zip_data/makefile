# variables that will be filled after including sub-makefiles 

TARGETS_TO_BUILD   =
TARGETS_TO_INSTALL =

# programs to use

LIBTOOL := libtool
CXX := g++
CC  := gcc

# include config values from configure script
include make.conf

# toggle make output
ifndef VERBOSE
	lt_silent := --silent
	echo_mkdep     = @echo "[mkdep]  $<" ;
	echo_cc        = @echo "[cc]     $@" ;
	echo_cxx       = @echo "[c++]    $@" ;
	echo_link      = @echo "[link]   $@" ;
	echo_install   = @echo "[install]   $@" ;
	echo_uninstall = @echo "[uninstall] $<" ;
endif

ifdef LT_DRYRUN
	lt_dry_run := -n
	lt_silent  :=
	echo_mkdep :=
	echo_cc    :=
	echo_cxx   :=
	echo_link  :=
endif

lt_flags := $(lt_silent) $(lt_dry_run)

# global compile/link commands
MKDEP_CXX   = $(echo_mkdep) $(CXX) $(CF_TGT) $(INC) $(DEFS) -MF"$@" -MM -MP -MT"$@" -MT"$(<:$(suffix $<)=.o)" $<
COMPILE_CXX = $(echo_cxx) $(LIBTOOL) $(lt_flags) --mode=compile --tag CXX $(CXX) $(CXXFLAGS)
LINK_CXX    = $(echo_link) $(LIBTOOL) $(lt_flags) --mode=link --tag CXX $(CXX) $(LINK_DEBUG) $(LINK_W32_EXTRA) -no-fast-install

MKDEP_CC    = $(echo_mkdep) $(CC) $(CF_TGT) $(INC) $(DEFS) -MF"$@" -MM -MP -MT"$@" -MT"$(<:$(suffix $<)=.o)" $<
COMPILE_CC  = $(echo_cc) $(LIBTOOL) $(lt_flags) --mode=compile --tag CC $(CC) $(CCFLAGS)
LINK_CC     = $(echo_link) $(LIBTOOL) $(lt_flags) --mode=link --tag CC $(CC) $(LINK_DEBUG) $(LINK_W32_EXTRA) -no-fast-install

# always silence libtool when installing... argh, what noise
INSTALL_CMD = $(echo_install) ./install
UINST_CMD   = $(echo_uninstall) $(LIBTOOL) $(lt_dry_run) --silent --mode uninstall rm -f


.PHONY: all build clean install uninstall

# common rules

%.d: %.cpp
	$(MKDEP_CXX)

%.o: %.cpp %.d
	$(COMPILE_CXX) -c -o $@ $<

%.lo: %.cpp %.d
	$(COMPILE_CXX) -c -o $(@:.lo=.o) $<

%.d: %.cxx
	$(MKDEP_CXX)

%.o: %.cxx %.d
	$(COMPILE_CXX) -c -o $@ $<

%.lo: %.cxx %.d
	$(COMPILE_CXX) -c -o $(@:.lo=.o) $<

%.d: %.c
	$(MKDEP_CC)

%.o: %.c %.d
	$(COMPILE_CC) -c -o $@ $<

%.lo: %.c %.d
	$(COMPILE_CC) -c -o $(@:.lo=.o) $<

%.d :
	@echo "Missing $@"


# jump to a dummy declared after including the sub-makefiles
# 'all' should be the first target
all: build

# when cleaning a subdir delete these:
FILES_TO_CLEAN := .libs .*.dummy *.o *.d *.la *.lo


# build/use static or shared libs?
ifdef BUILD_STATIC
	LIB_BUILD_SHARED := 
else
	# needed by libtool when creating libs; this activates creation of shared libs
	LIB_BUILD_SHARED :=
	# executables should look for shared libs in this directory
	BIN_LINK_FLAGS   := -Wl,-rpath $(DESTDIR)/lib
endif

# target-os specific stuff
ifdef WIN32
	SHARED_LIB_EXT := .dll
	EXE_EXT        := .exe
	LINK_W32_EXTRA := -no-undefined -mno-cygwin
else
	SHARED_LIB_EXT := .so
	EXE_EXT        :=
endif

# setup global flags using config values
CCFLAGS  = $(CF_TGT) $(FLAGS) $(INC) $(DEFS)
CXXFLAGS = $(CF_TGT) $(FLAGS) $(INC) $(DEFS)
#LDFLAGS  = $(LINK_DEBUG) $(LINK_W32_EXTRA)

ifdef DEBUG
	CCFLAGS    += -ggdb
	CXXFLAGS   += -ggdb
	LINK_DEBUG := -g
endif

# now include sub-project makefiles
include makefile.dirs

# compile everything
build: $(TARGETS_TO_BUILD)
