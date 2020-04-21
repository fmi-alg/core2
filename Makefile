# Makefile for Core Library
# $Id: Makefile,v 1.18 2010/08/03 16:00:47 exact Exp $

#################################################################
#
# Warning: this make file will best work with GNU's gmake
#
#################################################################

#################################################################
#	Core Library Parameters
#################################################################
PACKAGE=core
VERSION=2.1.0	# official release will have x.y, and intermediate
		# releases will be x.y.z (for z=1,2,etc)
CORE_PATH:=$(shell pwd)
CVSROOT=/home/exact/cvsroot
SUB_DIRS=src ext doc lib progs

# The following is needed to make sure that CC are properly 
#  for installing gmp
-include Make.config

#################################################################
#	gmp paths  
#	mpfr paths  
#	-- Should be inherited from $GMP, $MPFR in Make.config!
#################################################################
gmp	:= ${GMP}
mpfr	:= ${MPFR}

#################################################################
#	Default make target 
#################################################################
# This default is used when you want to change the Core library
#	without changing platform or gmp.
#	This target is also used by IDE's like eclipse

default: core

#################################################################
#	Make everything in one step
#################################################################
# This makes gmp, mpfr and core library, then test and time them

overall:  gmp mpfr core \
		test time 

# Important Note: target "all" is used by eclipse IDE.
# This is the target whenever you do a "build" for Core Library Project.
all: core

gmp:	first second third testgmp

mpfr:	fourth fifth sixth testmpfr

core:	corelib corex demo

alltests:	test time moretest

#################################################################
#	Core Library Installation Steps
#################################################################

# Zeroth Step: setup default SVN download of Core (Jan2018)
zeroth svn-setup:
	svn update --set-depth immediates doc progs
	svn update --set-depth infinity ext inc src
	-${MAKE} -C progs downloadBasicProgs

# First Step: configure for gmp
first config-gmp:
	@echo "First: Configure for GMP..."
	-time ${MAKE} -C builds config-gmp 2>&1 \
			| tee ${CORE_PATH}/tmp/DIAG_CONFIG_GMP 
	@echo "See file tmp/DIAG_CONFIG_GMP for screen output"
	@echo ""

# Second Step: build gmp libraries
second build-gmp:
	@echo "Second: Build GMP Library..."
	-time ${MAKE} -C builds build-gmp 2>&1 \
			| tee ${CORE_PATH}/tmp/DIAG_BUILD_GMP
	@echo "See file tmp/DIAG_BUILD_GMP for screen output"
	@echo ""

# Third Step: install gmp libraries and include files
third install-gmp:
	@echo "Third: Install GMP Library..."
	-time ${MAKE} -C builds install-gmp 2>&1 \
			| tee ${CORE_PATH}/tmp/DIAG_INSTALL_GMP
	@echo "See file tmp/DIAG_INSTALL_GMP for screen output"
	@echo ""

# Fourth Step: configure for mpfr
fourth config-mpfr:
	@echo "Configure for MPFR..."
	-time ${MAKE} -C builds config-mpfr 2>&1 \
			| tee ${CORE_PATH}/tmp/DIAG_CONFIG_MPFR 
	@echo "See file tmp/DIAG_CONFIG_MPFR for screen output"
	@echo ""

# Fifth Step: build mpfr libraries
fifth build-mpfr:
	@echo "Build MPFR Library..."
	-time ${MAKE} -C builds build-mpfr 2>&1 \
			| tee ${CORE_PATH}/tmp/DIAG_BUILD_MPFR
	@echo "See file tmp/DIAG_BUILD_MPFR for screen output"
	@echo ""

# Sixth Step: install mpfr libraries and include files
sixth install-mpfr:
	@echo "Install MPFR Library..."
	-time ${MAKE} -C builds install-mpfr 2>&1 \
			| tee ${CORE_PATH}/tmp/DIAG_INSTALL_MPFR
	@echo "See file tmp/DIAG_INSTALL_MPFR for screen output"
	@echo ""

# Seventh Step: build Core Library, Extensions and Sample Programs
seventh: core

#################################################################
#	Test gmp
#################################################################

testgmp:
	-${MAKE} -C ${CORE_PATH}/progs/generic testgmp \
			2>&1 | tee ${CORE_PATH}/tmp/DIAG_TEST_GMP

testmpfr:
	-${MAKE} -C ${CORE_PATH}/progs/generic testmpfr \
			2>&1 | tee ${CORE_PATH}/tmp/DIAG_TEST_MPFR

#################################################################
#	Basic Core Functions
#################################################################

# This target is also useful when you need to re-compile the Core
#	Library.  Typically, you may need a "debugging version"
#	of CORE (for the debugger to work).  In this case, you set
#	VAR=Debug in Make.config, and type "make core".  This would
#	create the library file "libcoreDebug.a" or "libcoreDebug.so".

core:	corelib corex demo

# Make Core Library
corelib:
	@echo "Build Core Library..."
	-time ${MAKE} -C src 2>&1 | tee ${CORE_PATH}/tmp/DIAG_CORELIB
	@echo "See file tmp/DIAG_CORELIB for screen output"
	@echo ""

# Make Core Extension Library
corex:
	@echo "Build Core Extension Library..."
	-${MAKE} -C ext 2>&1 | tee ${CORE_PATH}/tmp/DIAG_COREX
	@echo "See file tmp/DIAG_COREX for screen output"
	@echo ""

# Make Sample Core Programs
demo:	corelib corex
	@echo "Build Sample Programs..."
	-${MAKE} -C progs 2>&1 | tee ${CORE_PATH}/tmp/DIAG_BUILD_DEMO
	@echo "See file tmp/DIAG_BUILD_DEMO for screen output"
	@echo ""

# Run Test on Sample Core Programs
eighth test:
	@echo "Testing Sample Programs..."
	-${MAKE} -C progs test 2>&1 | tee ${CORE_PATH}/tmp/DIAG_TEST_DEMO
	@echo "See file tmp/DIAG_TEST_DEMO for screen output"
	@echo "Some of the tests are self-validating, and prints"
	@echo "   either CORRECT or ERROR accordingly."
	@echo ""

# Basic Timing: just time all the tests 
ninth time:
	@echo "Timing Sample Programs..."
	-time ${MAKE} test 2>&1 | tee ${CORE_PATH}/tmp/DIAG_TIME
	@echo "See file tmp/DIAG_TIME for screen output"
	@echo ""

# Run More Test on Sample Core Programs
tenth moretest:
	@echo "More Testing of Sample Programs..."
	-${MAKE} -C progs moretest 2>&1 | tee ${CORE_PATH}/tmp/DIAG_MORETEST_DEMO
	@echo "See file tmp/DIAG_MORETEST_DEMO for screen output"
	@echo ""

# More Timing: just time all "make moretest"
eleventh moretime:
	@echo "More Timing of Sample Programs..."
	-time ${MAKE} moretest 2>&1 | tee ${CORE_PATH}/tmp/DIAG_MORETIME
	@echo "See file tmp/DIAG_MORETIME for screen output"
	@echo ""

#################################################################
#	Check Options
#################################################################
help:
	@echo "Try these targets:"
	@echo "   help -- this message"
	@echo "   opt -- currently selected options"
	@echo "   allopt -- all the possible options"
	@echo "   core -- same as choosing the 3 targets of corelib+corex+demo"
	@echo "   testgmp -- to test if you have installed gmp"
	@echo "   testmpfr -- to test if you have installed mpfr"


opt options check:
	@echo "YOUR CURRENT CORE ENVIRONMENT VARIABLES ARE: "
	@echo "  PLATFORM:	" ${PLATFORM}
	@echo "  LINKAGE:	" ${LINKAGE}
	@echo "  VAR(iant):	" ${VAR}
	@echo "  GMP_PREFIX:	" ${GMP_PREFIX}
	@echo "  MPFR_PREFIX:	" ${MPFR_PREFIX}
	@echo "  PACKAGE:	" ${PACKAGE}
	@echo "  VERSION:	" ${VERSION}
	@echo "  CORE_PATH:	" ${CORE_PATH}
	@echo "  AR(chiver):	" ${AR}
	@echo "  CC(compiler):	" ${CC}
	@echo "  CXX(compiler): " ${CXX}
	@echo "  CXXFLAGS:	" ${CXXFLAGS}
	@echo "  CORE_CXXFLAGS	" ${CORE_CXXFLAGS}
	@echo "  LDFLAGS:	" ${LDFLAGS}
	@echo "  CORE_LDFLAGS:	" ${CORE_LDFLAGS}
	@echo "  EXETYPE:	" ${EXETYPE}
	@echo "  OS:		" ${OS}

allopt alloptions:
	@echo "YOUR CURRENT CORE ENVIRONMENT VARIABLES ARE: "
	@echo "    (possible alternatives are listed in parenthesis)"
	@echo ""
	@echo "  PLATFORM:	" ${PLATFORM} "		(cyg,mac,gnu,sun,mingw,sgi)"
	@echo "  LINKAGE:	" ${LINKAGE}  "	(static,shared)"
	@echo "  VAR(iant):	" ${VAR}      "		(\"\",Debug,NoOpt)"
	@echo "  GMP_PREFIX:	" ${GMP_PREFIX} "		(..your choice..)"
	@echo "  MPFR_PREFIX:	" ${MPFR_PREFIX}
	@echo "  PACKAGE:	" ${PACKAGE} 
	@echo "  VERSION:	" ${VERSION}
	@echo "  CORE_PATH:	" ${CORE_PATH}
	@echo "  AR(chiver):	" ${AR}
	@echo "  CC(compiler):	" ${CC}
	@echo "  CXX(compiler): " ${CXX}
	@echo "  CXXFLAGS:	" ${CXXFLAGS}  "	(..compiler flags..)"
	@echo "  CORE_CXXFLAGS	" ${CORE_CXXFLAGS}
	@echo "  LDFLAGS:	" ${LDFLAGS}   "		(..load flags..)"
	@echo "  CORE_LDFLAGS:	" ${CORE_LDFLAGS}
	@echo "  EXETYPE:	" ${EXETYPE}   "		(\"\",.exe)"
	@echo "  OS:		" ${OS}        "	(automatic)"

# after you compiled core library or run it tests, you can look for
# occurrences of "ERROR" in output files under $(CORE)/tmp:
checkerror:
	-cd tmp; grep -A 1 -B 1 ERROR *
	
#################################################################
#	Cleaning Up files 
#################################################################

# clean_garbage: Remove all temp files
#
clean_garbage:
	find . -name "*.exe.stackdump" -print | xargs -i \rm -rf {}
	find . -name ii_files -print | xargs -i \rm -rf {}
	find . -name SunWS_cache -print | xargs -i \rm -rf {}
	find . -name "*\~" -print | xargs -i \rm -f {}
	find . -name "typescript" -print | xargs -i \rm -f {}

# clean: Remove object files
# This is used if you want to compile a different version of the
#	Core Library (but on the same platform)
#
clean:
	@list='$(SUB_DIRS)'; for dir in $$list; do \
	  if test -d "$$dir"; then ${MAKE} -C "$$dir" clean; fi; \
	done

# cleangmp: Clean the gmp directory only
# NOTE: to recover from this step, you need to do "make second" to
#	recompile gmp.  This is not cheap.
#
cleangmp:
	-@${MAKE} -C ${gmp} clean

# veryclean: Remove executables as well as object files.
# This is done when you need to rebuild everything (e.g., you need 
#	to recompile the library for a new platform)
# "veryclean" subsumes "clean" (although this is somewhat implicit)
#	but does not subsume "cleangmp" (which is expensive!)
# NOTE: after this step, you need to rebuild the Core Library from
#	step "make third" and subsequent.
#
vclean veryclean:  
	@list='$(SUB_DIRS)'; for dir in $$list; do \
	  if test -d "$$dir"; then ${MAKE} -C "$$dir" veryclean; fi; \
	done
	-@test -z "${CORE_PATH}/tmp/DIAG_*" || rm -f ${CORE_PATH}/tmp/DIAG_*

# superclean: clean everything (including *.o and *.a files)
#
superclean: veryclean cleangmp
	-@rm -rf ${CORE_PATH}/lib/*.{a,so}
	-@rm -rf ${CORE_PATH}/src/*.o
	-@rm -rf ${CORE_PATH}/src/gmpxx/*.o
		
#################################################################
#	Making Core Distributions
#################################################################
# commit all current changes to CVS
cvs commit:
	cvs commit

# Make CVS-based distributions (base distribution and full distribution)
base:
	(cd ..; cvs -d ${CVSROOT} co corelib; \
	 tar cFFvf - corelib | \
	 	gzip -9 > ${PACKAGE}_v${VERSION}.tgz )

# Make CVS-based incremental distributions (gmp distribution and doc distribution)
full:
	(cd ..; cvs -d ${CVSROOT} co corelib; \
	 cp ${gmp}.tar.gz corelib; \
	 tar cFFvf - corelib | \
	 	gzip -9 > ${PACKAGE}_v${VERSION}.full.tgz )


#################################################################
# END OF CORE Makefile
#################################################################
