#
#  This file is part of TUnfold.
#
#  TUnfold is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  TUnfold is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with TUnfold.  If not, see <http://www.gnu.org/licenses/>.
#
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
TUNFOLDNEWVERSION:=V17.9
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

DICT=TUnfold$(TUNFOLDCODEVER)Dict.C
COBJC=$(SRC:%.cxx=%.o) $(DICT:%.C=%.o)
DICTINPUT=$(if $(subst 5,,$(ROOTMAJORVERSION)),$(HEADER),$(SRC))
##
## macro examples for stand-alone tests

# BINSRC:=$(subst Unfold,unfoldmain,$(MACRO))
BINSRC:=$(MACRO)

BIN:=$(BINSRC:%.C=%)

##
## figures included in the manual
#
FIGURES=$(shell ls fig/tunfold_manual_fig*.eps)

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

##
## tar file including the root source tree
##

ROOTTAR:=TUnfold_$(TUNFOLDVERSION)_for_root.tgz

## shortcuts
##
##  depend
##  clean
##  lib
##  tar
##  roottar
##  manual

make: $(BIN)

depend:  $(SRC) $(HEADER)
	makedepend -V -I. $(SRC) $(HEADER)
	makedepend -V -I. $(BINSRC) $(MACRO) -a

clean:
	rm -f $(LB) *.o *Dict.* $(BIN) $(BINSRC) $(BIN:%=%.ps) *\~ tunfold_manual.pdf tunfold_manual.dvi *.pcm testUnfold5binning.xml testUnfold*.root filter testUnfold5.ps tunfold_manual.aux tunfold_manual.log Makefile.bak dictlib.xml
	rm -rf hist tutorials doxygen *.eps testUnfold*.pdf testUnfold6.out.xml

lib: $(LB)

tar: $(VERSIONTAR)

roottar: $(ROOTTAR)

manual: tunfold_manual.pdf

## create PDF of the user's manual

tunfold_manual.dvi: tunfold_manual.tex $(FIGURES)
	latex $< -o $@
	latex $< -o $@

tunfold_manual.pdf: tunfold_manual.dvi
	dvipdf $<


# extract doxygen documentation

filter: doxygentest/filter.cxx
	g++ -g --std=c++11 $< -o$@

doxygen: clean filter
	mkdir -p doxygen
	export DOXYGEN_OUTPUT_DIRECTORY=doxygen export DOXYGEN_SOURCE_DIRECTORY=. ; doxygen doxygentest/Doxyfile

# create tar file including all files beloning to this version

$(VERSIONTAR): $(HEADER) $(SRC) $(MACRO) altercodeversion.sh Makefile README COPYING tunfold_manual.tex tunfold_manual.pdf $(FIGURES) testUnfold6binning.xml testUnfold7binning.xml tunfoldbinning.dtd
	tar cvfz $@ $+

# create root source tree

$(ROOTTAR): $(ROOTSOURCES) $(ROOTHEADERS) $(ROOTMACROS)
	tar cvfz $@ $+

$(ROOTHEADERS): hist/unfold/inc/%.h : %.h altercodeversion.sh
	mkdir -p hist/unfold/inc
	./altercodeversion.sh $< $(TUNFOLDCODEVER) > $@

$(ROOTSOURCES): hist/unfold/src/%.cxx : %$(TUNFOLDCODEVER).cxx altercodeversion.sh
	mkdir -p hist/unfold/src
	./altercodeversion.sh $< $(TUNFOLDCODEVER) > $@

$(ROOTMACROS): tutorials/unfold/%.C : %.C
	mkdir -p tutorials/unfold
	cp $< $@

# create new version
# (some manual changes are required in addition)

newversion:
	make $(VERSIONTAR)
	mkdir -p $(DIRNEW)
	cp $(VERSIONTAR) $(DIRNEW)/$(VERSIONTAR)
	cd $(DIRNEW) ; tar xvfz $(VERSIONTAR)
	rm $(DIRNEW)/$(VERSIONTAR)
	rm $(DIRNEW)/*.h
	rm $(DIRNEW)/*.cxx
	make $(HEADERNEW)
	make $(SRCNEW)

$(HEADERNEW): $(HEADER)
	mkdir -p $(DIRNEW)
	./altercodeversion.sh $(subst $(DIRNEW)/,,$@) $(TUNFOLDCODEVER) $(TUNFOLDNEWCODEVER) > $@

$(SRCNEW): $(SRC)
	mkdir -p $(DIRNEW)
	./altercodeversion.sh $(subst $(TUNFOLDNEWCODEVER),$(TUNFOLDCODEVER),$(subst $(DIRNEW)/,,$@)) $(TUNFOLDCODEVER) $(TUNFOLDNEWCODEVER) > $@

# Root cint preprocessor

dict: $(DICT)

$(DICT): $(DICTINPUT)
	rm -f dictlib.xml
	echo '<class name="TUnfold'$(TUNFOLDCODEVER)'"/>' > dictlib.xml
	# echo '<class name="TUnfoldSys'$(TUNFOLDCODEVER)'"/>' >> dictlib.xml
	# echo '<class name="TUnfoldDensity'$(TUNFOLDCODEVER)'"/>' >> dictlib.xml
	# echo '<class name="TUnfoldBinning'$(TUNFOLDCODEVER)'"/>' >> dictlib.xml
	# echo '<class name="TUnfoldBinningXML'$(TUNFOLDCODEVER)'"/>' >> dictlib.xml
	genreflex ./TUnfold.h -o $@ -s dictlib.xml
	# genreflex ./TUnfoldDensity.h ./TUnfoldBinningXML.h -o $@ -s dictlib.xml
#	$(ROOTCINT) -f $@ -s $@ -c -p $^

# library of TUnfold classes

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LB): $(COBJC)
	$(call MAKELIBRARY,$(COBJC),$(LB))

# create code to compile example macros stand-alone

# $(BINSRC): Makefile
# 	rm -f $@
# 	echo "#include <TError.h>" > $@
# 	echo "/* This code is generated automatically, do not edit */" >> $@
# 	echo "void "$(patsubst %.C,%,$(subst unfoldmain,Unfold,$@))"();" >> $@
# 	echo "int main() {" >>$@
# 	echo "gErrorIgnoreLevel=kInfo+1;" >>$@
# 	echo "gErrorAbortLevel=kError;" >>$@
# 	echo $(patsubst %.C,%,$(subst unfoldmain,Unfold,$@))"();" >>$@
# 	echo "return 0;" >> $@
# 	echo "}" >> $@

# compile macros stand-alone

%.o: %.C
	$(CXX) $(CXXFLAGS) -c $< -o $@

# $(BIN): testunfoldmain%: testunfoldmain%.C $(LB) testUnfold%.o
# 	$(CXX) $(CXXFLAGS) $< -o  $@ testUnfold$(*).o $(LDFLAGS) -l$(LIB) \
# 	$(ROOTLIBS)
$(BIN): UndersmoothDemo%: UndersmoothDemo%.C $(LB) $(DICT) UndersmoothDemo%.o
	$(CXX) $(CXXFLAGS) -o  $@ $(DICT) UndersmoothDemo$(*).o $(LDFLAGS) -l$(LIB) \
	$(ROOTLIBS)

# DO NOT DELETE

TUnfoldV17.o: TUnfold.h

testUnfold2.o: TUnfold.h
