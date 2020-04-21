# file: Makefile (for src)
#$Id: Makefile,v 1.13 2010/08/02 21:14:47 exact Exp $

# =============================================================
# PATHS ======================================================
# =============================================================

RELATIVE_CORE_PATH=..
# read in the appropriate compilation settings from the
# global Make.config.
include ${RELATIVE_CORE_PATH}/Make.config

# This is needed because in shared mode, on Mac OS X, the
# Mach system embeds the full path of the dynamic library
# into the executable being linked, so that LD_LIBRARY_PATH
# is not needed.
#
# This converts the core path from relative to absolute.
CORE_PATH=$(RELATIVE_CORE_PATH)
ifeq (${PLATFORM}, mac)
  ifeq (${LINKAGE}, shared)
    CORE_PATH=$(CURDIR)/$(RELATIVE_CORE_PATH)
  endif
endif

INCLUDE=-I${CORE_PATH}/inc -I${CORE_PATH}/mpfr/include -I${CORE_PATH}/gmp/include

# VAR indicates variants of the Core library
#	E.g., if VAR=(empty) then we compile the release version.
#	If VAR=Debug, then we compile a debugging version.

EXPROBJS=MpfrIO${VAR}.o CoreDefs${VAR}.o CoreAux${VAR}.o

ifdef USE_GMPXX
  LIBS+= -lgmpxx
else
  EXPROBJS+= gmpxx/isfuns${VAR}.o gmpxx/ismpz${VAR}.o \
	 gmpxx/ismpq${VAR}.o gmpxx/ismpf${VAR}.o
endif

# =============================================================
# TARGETS =====================================================
# =============================================================

# The following library will be built:
CORE_LIB=$(CORE_PATH)/lib/libcore++${VAR}.a
CORE_SHARED_LIB=$(CORE_PATH)/lib/libcore++${VAR}.${CORE_DYLIB_EXTENSION}

TARGET=$(CORE_LIB)

ifeq (${LINKAGE}, shared)
  TARGET+=$(CORE_SHARED_LIB)
endif

DEPS=-L${GMP_PREFIX}/lib -L${MPFR_PREFIX}/lib -lgmp -lmpfr

# =============================================================
# RULES  ======================================================
# =============================================================

all:	${TARGET}

${CORE_LIB}:	$(EXPROBJS)
	$(AR) $@ $^

${CORE_SHARED_LIB}:	$(EXPROBJS)
	$(CXX) $(DEPS) $(CORE_LDFLAGS) -o $@ $^

%${VAR}.o:	%.cpp
	${CXX} $(CORE_CXXFLAGS) $(INCLUDE) -c $< -o $@

%${VAR}.o:	%.cc
	${CXX} $(CORE_CXXFLAGS) $(INCLUDE) -c $< -o $@

# =============================================================
# HOUSE KEEPING  ==============================================
# =============================================================

clean:
	-@test -z "*${VAR}.o" || rm -rf *${VAR}.o

veryclean: clean
	-@test -z "${CORE_LIB}" || rm -rf "${CORE_LIB}"
	-@test -z "${CORE_SHARED_LIB}" || rm -rf "${CORE_SHARED_LIB}"

