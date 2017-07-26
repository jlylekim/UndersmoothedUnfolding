# Modified by Junhyung Lyle Kim and Mikael Kuusela
# starting from Makefile in TUnfold package (version 17.6) by Stefan Schmitt

ROOTCONFIG   := $(ROOTSYS)/bin/root-config
ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags)
ROOTLDFLAGS  := $(shell $(ROOTCONFIG) --ldflags)
ROOTLIBS     := $(shell $(ROOTCONFIG) --libs)
ROOTGLIBS    := $(shell $(ROOTCONFIG) --glibs)
ROOTVERSION  := $(shell $(ROOTCONFIG) --version)
ROOTMAJORVERSION := $(word 1,$(subst ., ,$(ROOTVERSION)))
ROOTCINT=$(ROOTSYS)/bin/rootcint

CXXFLAGS=-isystem $(shell $(ROOTCONFIG) --incdir) -I$(ROOTSYS)/htmldoc -I. -O2 -g -Wall -Wshadow -W -Woverloaded-virtual -fPIC $(ROOTCFLAGS)
LDFLAGS=$(ROOTLDFLAGS) -L. -Wl,-rpath .
CXX=$(shell $(ROOTCONFIG) --cxx)

OSTYPE=$(shell uname)
ROOTLIBS     := -L$(shell $(ROOTCONFIG) --libdir) -lXMLParser $(ROOTLIBS)

## version number Vx.y (e.g. x=17 y=0)
## code version Vx (e.g. x=17)
## header files TUnfold*.h
## source files TUnfold*.cxx
## macro files testUnfold*.C
##
## classes have the code version in the class name
##  e.g. TUnfoldV17
##
## in the header file, #define statements are added
## such that the user can use the conventional class names
##  e.g. #define TUnfold TUnfoldV17
##
## when exported to root, the code version is removed from the class name

TUNFOLDVERSION :=$(shell grep TUnfold_VERSION TUnfold.h)
TUNFOLDVERSION :=$(filter-out \#define TUnfold_VERSION,$(TUNFOLDVERSION))
TUNFOLDVERSION :=$(shell echo $(TUNFOLDVERSION))
TUNFOLDCODEVER :=$(subst ., ,$(TUNFOLDVERSION))
TUNFOLDCODEVER :=$(word 1,$(TUNFOLDCODEVER))
HEADER=$(shell ls TUnfold*.h | grep -v Dict)
SRC=$(shell ls TUnfold*$(TUNFOLDCODEVER).cxx)
MACRO=$(shell ls UndersmoothDemo*.C)

## this is for changing the code version
TUNFOLDNEWVERSION:=V17.6
TUNFOLDNEWCODEVER :=$(subst ., ,$(TUNFOLDNEWVERSION))
TUNFOLDNEWCODEVER :=$(word 1,$(TUNFOLDNEWCODEVER))
DIRNEW:=../TUnfold_$(TUNFOLDNEWVERSION)
HEADERNEW:=$(subst TUnfold,$(DIRNEW)/TUnfold,$(HEADER))
SRCNEW:=$(subst $(TUNFOLDCODEVER),$(TUNFOLDNEWCODEVER),$(SRC))
SRCNEW:=$(subst TUnfold,$(DIRNEW)/TUnfold,$(SRCNEW))
##
## library for stand-alone test

LIB=unfold

ifeq ($(OSTYPE),Linux)
	MAKELIBRARY = $(CXX) $(CXXFLAGS) -shared -o $(2) $(1)
	LB=lib$(LIB).so
#	LB0=lib$(LIB)0.so
else
	MAKELIBRARY = rm $(2) ; ar rv $(2) $(1) ; ranlib $(2)
	LB=lib$(LIB).a
#	LB0=lib$(LIB)0.a
endif

COBJC=$(SRC:%.cxx=%.o)
DICT=TUnfold$(TUNFOLDCODEVER)Dict.C
DICTINPUT=$(if $(subst 5,,$(ROOTMAJORVERSION)),$(HEADER),$(SRC))
##
## macro examples for stand-alone tests

BINSRC:=$(MACRO)

BIN:=$(BINSRC:%.C=%)

##
## tar file including all files
##

VERSIONTAR:=TUnfold_$(TUNFOLDVERSION).tgz

##
## source tree for root
## (with version number from class names)
##
ROOTSOURCES=$(subst TUnfold,hist/unfold/src/TUnfold,$(SRC:%$(TUNFOLDCODEVER).cxx=%.cxx))
ROOTHEADERS= $(subst TUnfold,hist/unfold/inc/TUnfold,$(HEADER))


## shortcuts
##
##  clean
##  lib

make: $(BIN)

clean:
	rm -f $(LB) *.o *Dict.* *.pcm $(BIN:%=%.pdf)

lib: $(LB)


# create root source tree
$(ROOTHEADERS): hist/unfold/inc/%.h : %.h altercodeversion.sh
	mkdir -p hist/unfold/inc
	./altercodeversion.sh $< $(TUNFOLDCODEVER) > $@

$(ROOTSOURCES): hist/unfold/src/%.cxx : %$(TUNFOLDCODEVER).cxx altercodeversion.sh
	mkdir -p hist/unfold/src
	./altercodeversion.sh $< $(TUNFOLDCODEVER) > $@


# create new version
# (some manual changes are required in addition)

newversion: copyoldversion $(HEADERNEW) $(SRCNEW)

copyoldversion:
	make $(VERSIONTAR)
	mkdir -p $(DIRNEW)
	cp $(VERSIONTAR) $(DIRNEW)/$(VERSIONTAR)
	cd $(DIRNEW) ; tar xvfz $(VERSIONTAR)
	rm $(DIRNEW)/$(VERSIONTAR)
	mkdir -p $(DIRNEW)/tmpheader
	rm $(DIRNEW)/*.h
	rm $(DIRNEW)/*.cxx

$(HEADERNEW): $(HEADER)
	mkdir -p $(DIRNEW)
	./altercodeversion.sh $(subst $(DIRNEW)/,,$@) $(TUNFOLDCODEVER) $(TUNFOLDNEWCODEVER) > $@

$(SRCNEW): $(SRC)
	mkdir -p $(DIRNEW)
	./altercodeversion.sh $(subst $(TUNFOLDNEWCODEVER),$(TUNFOLDCODEVER),$(subst $(DIRNEW)/,,$@)) $(TUNFOLDCODEVER) $(TUNFOLDNEWCODEVER) > $@

# Root cint preprocessor

dict: $(DICT)

$(DICT): $(DICTINPUT)
	$(ROOTCINT) -f $@ -c -p $^

# library of TUnfold classes

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LB): $(COBJC)
	$(call MAKELIBRARY,$(COBJC),$(LB))


# compile macros stand-alone
%.o: %.C
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN): UndersmoothDemo%: UndersmoothDemo%.C $(LB) $(DICT) UndersmoothDemo%.o
	$(CXX) $(CXXFLAGS) -o  $@ $(DICT) UndersmoothDemo$(*).o $(LDFLAGS) -l$(LIB) \
	$(ROOTLIBS)


# DO NOT DELETE

TUnfoldV17.o: TUnfold.h

UndersmoothDemo.o: TUnfold.h
