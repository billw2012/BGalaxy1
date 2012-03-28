#  This is the GNUmakefile of the HPA Library.
#  Copyright (C)  2000    Daniel A. Atkinson  <DanAtk@aol.com>
#  Copyright (C)  2004    Ivano Primi  <ivprimi@libero.it>    
#
#  Copying and distribution of this file, with or without modification,
#  are permitted in any medium without royalty provided the copyright
#  notice and this notice are preserved.

# Remark: This makefile requires the GNU version of make, also called
# 'gmake'. To obtain 'gmake' go to   http://directory.fsf.org   and start a
# search with the word 'gmake' or 'make'.

# Check for the presence of these programs on the system where you are going
# to build the library. If you can not find the GNU C compiler (gcc) you can
# use in place of it any other ANSI-compliant C compiler supporting the 
# options -c, -D, -g, -I, -O, -o, -L and -l.
# The same is for the C++ compiler.
# Remark: 'shtool' is shipped together with the source code. So, do not
# worry about its presence !
CC= gcc
CXX= g++
AR= ./shtool arx rcs
CP= ./shtool install -c -m 644
MKDIR= ./shtool install -d
INSTALL_BIN= ./shtool install -m 755
INSTALL_LIB= ./shtool install -m 644
MKSYMLINK= ./shtool mkln -f -s
ECHO= ./shtool echo 
RM= rm -f
RMDIR= rmdir

# If you want that the compiler puts the debug symbols into the object code,
# then define the following variable to -g.
DEBUG_MACRO=
#DEBUG_MACRO=  -Wall
#DEBUG_MACRO= -g 
#DEBUG_MACRO= -g -Wall

# If you want that the compiler produces an optimized object code, then
# define the following variable to something like -O (-O3 suggested for gcc3, -O1 for gcc4).
#OPT_MACRO=
OPT_MACRO= -O

# The value of MANTSIZE_MACRO defines the precision which the functions
# of the library will perform their computations with. The default
# value is 7, which guarantees a precision higher than the extended
# double one. However, you may increase the precision by setting a larger
# value for the size of the mantissa. Besides 7, you may use 11, 15, 19,
# 23, 27 and 31.
# Remark: The HPA library defines its own numeric type, called 'xpr'.
#         2 x (XMANTISSA_SIZE + 1) will be the size (in bytes) of
#         a variable of type 'xpr'. Then, if you choose an higher value
#         for MANTSIZE_MACRO, you will obtain not also an higher precision
#         in computations but also a bigger consumption of memory and 
#         more time needed to perform your computations. So, the next value
#         must be chosen carefully, according to your needs and to the
#         power of your machine.
MANTSIZE_MACRO = -DXMANTISSA_SIZE=7
#MANTSIZE_MACRO= -DXMANTISSA_SIZE=11 
#MANTSIZE_MACRO= -DXMANTISSA_SIZE=15
#MANTSIZE_MACRO= -DXMANTISSA_SIZE=19
#MANTSIZE_MACRO= -DXMANTISSA_SIZE=23
#MANTSIZE_MACRO= -DXMANTISSA_SIZE=27
#MANTSIZE_MACRO= -DXMANTISSA_SIZE=31

# ERR_MACRO can be used to change the way the HPA library
# deals with the floating point errors. By default, whenever
# a runtime error occurs within a function of the HPA
# library, this function sets an external error indicator to
# a suitable value. This value can be looked up later
# to know what exactly went wrong. However, you could prefer
# another way to deal with runtime errors. If you want that
# a suitable message is printed onto stderr whenever a
# floating point error occurs, then define ERR_MACRO as
# XERR_WARN. If you want that the occurrence of a floating
# point error also causes the termination (via EXIT(1))
# of the program being executed, then define ERR_MACRO as XERR_EXIT.
# At last, if you prefer to disable at all the code for managing
# errors, then set ERR_MACRO to XERR_IGN (strongly discouraged).
# I suggest you to leave ERR_MACRO as XERR_DFL, so that the
# management of the errors remains the default one.  
ERR_MACRO = -DXERR_DFL
#ERR_MACRO = -DXERR_WARN
#ERR_MACRO = -DXERR_EXIT
#ERR_MACRO = -DXERR_IGN

# Root directory of the installation
PREFIX=/usr/local
#PREFIX=/opt
#PREFIX=/usr
#PREFIX=$(HOME)

# BELOW THIS LINE YOU SHOULD NOT CHANGE ANYTHING

# BINDIR defines the (sub)directory of the system where the 'hpaconf'
# program will be installed; of course, you can change
# this value but you must know what you are doing.
# Moreover, if $(PREFIX)/bin is not present in the searching path of
# the executable files (i.e. it is not present in the list 
# specified by the PATH environment variable), then you will have to
# add $(PREFIX)/bin to the list of directories of the PATH environment
# variable.
BINDIR= $(PREFIX)/bin
# INCLUDEDIR defines the (sub)directory of the system where the include files
# provided with this library will be installed; of course, you can change
# this value but you must know what you are doing.
INCLUDEDIR= $(PREFIX)/include
# LIBDIR defines the (sub)directory where the file containing the object code
# of the library will be placed. The library will be created in a static
# form. You can change the location defined by LIBDIR but you must know
# what you are doing. 
LIBDIR= $(PREFIX)/lib
# DOCDIR defines the (sub)directory where will be placed the files
# containing the text of the license under which this library is distributed
# and the files of advertisement and help. Even this location can be redefined.
DOCDIR= $(PREFIX)/doc/hpalib

VERSION = 1.7

CFLAGS = -c $(DEBUG_MACRO) $(OPT_MACRO)

CXXFLAGS = $(CFLAGS) -I./src

# Include files
HEADERS		= ./src/hpaconf.h ./src/xpre.h ./src/cxpre.h

# C Source files
SRCS		= ./src/atox.c ./src/constant.c ./src/cxaop.c ./src/cxbasic.c ./src/cxconstant.c ./src/cxconvf.c ./src/cxexp.c ./src/cxhypb.c ./src/cxidiv.c ./src/cxpow.c ./src/cxprcmp.c ./src/cxtrig.c ./src/prcxpr.c ./src/print.c ./src/prxpr.c ./src/sfmod.c ./src/shift.c ./src/xadd.c ./src/xchcof.c ./src/xdiv.c ./src/xevtch.c ./src/xexp.c ./src/xfmod.c ./src/xfrac.c ./src/xhypb.c ./src/xivhypb.c ./src/xivtrg.c ./src/xlog.c ./src/xmul.c ./src/xneg.c ./src/xprcmp.c ./src/xpwr.c ./src/xsigerr.c ./src/xsqrt.c ./src/xtodbl.c ./src/xtoflt.c ./src/xtrig.c

# C++ wrapper include files
CXXHEADERS	= ./cxxsrc/xcomplex.h ./cxxsrc/xreal.h

# C++ wrapper source files
CXXSRCS		= ./cxxsrc/read_item.cc ./cxxsrc/xcomplex.cc ./cxxsrc/xreal.cc

#Documentation repository
DOC_REP = ./doc

#Doc files
DOCS		= BUGS Changelog INSTALL NOWARRANTY README $(DOC_REP)/COPYING.LIB $(DOC_REP)/hpa.html $(DOC_REP)/hpa.pdf $(DOC_REP)/hpa.txt

#Object files
OBJECTS		= $(SRCS:.c=.o)

#C++ Object files
CXXOBJECTS	= $(CXXSRCS:.cc=.o)

# Byteord source file
BYTEORD_SRC	= byteord.c

# Byteord exec file
BYTEORD		= byteord 

#library file
LIBNAME		= libhpa.a

#C++ wrapper library file
CXXLIBNAME	= libhpaxx.a

#hpaconf is the program that, once the library will have been installed,
#will help you to compile with and link against the HPA library.
HPACONF		= hpaconf

#hpaxxconf is a symbolic link to hpaconf.
HPAXXCONF	= hpaxxconf

.PHONY :  all cxx .depend .cxxdepend install cxx_install clean clobber uninstall
all : clobber .depend $(LIBNAME) $(HPACONF)

.depend:
	$(CC) $(MANTSIZE_MACRO) $(ERR_MACRO) -DVERSION="\"$(VERSION)\"" $(BYTEORD_SRC) -o $(BYTEORD)
	./$(BYTEORD) 
	$(CC) -M $(CFLAGS) $(SRCS) > .depend

ifeq ($wildcard .depend,.depend)
include .depend
endif

$(LIBNAME) : $(OBJECTS)
	$(AR) $(LIBNAME) $^

$(HPACONF) :
	$(CC) -DVERSION="\"$(VERSION)\"" -DIPATH="\"-I$(PREFIX)/include\"" -DLPATH="\"-L$(LIBDIR)\"" $(MANTSIZE_MACRO) $(ERR_MACRO) ./hpaconf.c -o $(HPACONF)

cxx : all .cxxdepend $(CXXLIBNAME) $(HPAXXCONF)

.cxxdepend:
	$(CXX) -M $(CXXFLAGS) $(CXXSRCS) > .cxxdepend

ifeq ($wildcard .cxxdepend,.cxxdepend)
include .cxxdepend
endif

$(CXXLIBNAME) : $(CXXOBJECTS)
	$(AR) $(CXXLIBNAME) $^

$(HPAXXCONF) :
	$(MKSYMLINK) ./$(HPACONF) ./$(HPAXXCONF)

install : all
	$(MKDIR) $(BINDIR)
	$(MKDIR) $(LIBDIR)
	$(MKDIR) $(INCLUDEDIR)
	$(MKDIR) $(DOCDIR)
	$(CP) $(DOCS)    $(DOCDIR)
	$(CP) ./GNUmakefile $(DOCDIR)
	$(CP) $(HEADERS) $(INCLUDEDIR)
	$(INSTALL_LIB)   $(LIBNAME) $(LIBDIR)
	$(INSTALL_BIN)   $(HPACONF) $(BINDIR)

cxx_install : cxx
	$(MKDIR) $(BINDIR)
	$(MKDIR) $(LIBDIR)
	$(MKDIR) $(INCLUDEDIR)
	$(MKDIR) $(DOCDIR)
	$(CP) $(DOCS)    $(DOCDIR)
	$(CP) ./GNUmakefile $(DOCDIR)
	$(CP) $(HEADERS) $(INCLUDEDIR)
	$(INSTALL_LIB)   $(LIBNAME) $(LIBDIR)
	$(INSTALL_BIN)   $(HPACONF) $(BINDIR)
	$(CP) $(CXXHEADERS) $(INCLUDEDIR)
	$(INSTALL_LIB)   $(CXXLIBNAME) $(LIBDIR)
	$(INSTALL_BIN)   $(HPAXXCONF) $(BINDIR)

clean :
	$(RM) $(OBJECTS)
	$(RM) $(CXXOBJECTS)
	$(RM) $(BYTEORD)

clobber : clean
	$(RM) .depend
	$(RM) .cxxdepend
	$(RM) $(LIBNAME)
	$(RM) $(CXXLIBNAME)
	$(RM) $(HPAXXCONF)
	$(RM) $(HPACONF)

uninstall :
	$(RM)    $(DOCDIR)/*
	$(RM)    $(INCLUDEDIR)/hpaconf.h
	$(RM)    $(INCLUDEDIR)/cxpre.h
	$(RM)    $(INCLUDEDIR)/xpre.h
	$(RM)    $(INCLUDEDIR)/xreal.h
	$(RM)    $(INCLUDEDIR)/xcomplex.h
	$(RM)    $(LIBDIR)/$(LIBNAME)
	$(RM)    $(LIBDIR)/$(CXXLIBNAME)
	$(RM)    $(BINDIR)/$(HPAXXCONF)
	$(RM)    $(BINDIR)/$(HPACONF)
	$(RMDIR) $(DOCDIR)
	@$(ECHO) "I have not removed the directories $(PREFIX), $(BINDIR),"
	@$(ECHO) "$(LIBDIR), $(PREFIX)/include and $(PREFIX)/doc."
	@$(ECHO) "If you may and want to do it, do it yourself !"

# These programs are only used by developers of the HPA library
# to update the documentation. 
TXT2TEX= txt2tags --enum-title --toc -t tex
TXT2HTML= txt2tags --mask-email --enum-title --toc -t html
TXT2ASCII= txt2tags --mask-email --enum-title --toc -t txt
LATEX= latex
DVIPDF= dvipdf

doc : hpa.pdf hpa.html hpa.txt

hpa.pdf : $(DOC_REP)/hpa.t2t
	$(TXT2TEX) -i $< -o $(DOC_REP)/hpa.tex
	$(LATEX) $(DOC_REP)/hpa.tex
	$(LATEX) $(DOC_REP)/hpa.tex
	$(DVIPDF) hpa.dvi $(DOC_REP)/$@
	$(RM) ./hpa.*

hpa.html: $(DOC_REP)/hpa.t2t
	$(TXT2HTML) -i $< -o $(DOC_REP)/$@

hpa.txt: $(DOC_REP)/hpa.t2t
	$(TXT2ASCII) -i $< -o $(DOC_REP)/$@

