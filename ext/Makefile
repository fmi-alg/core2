# file: Makefile (for COREX)
#
#  Makefile for CORE Library Extensions (libcorex++.a)
#
#  	We create the Level 1, 2 and 3 versions
#	of each library file, so that users can
#	run their CORE programs in either level.
# $Id: Makefile,v 1.2 2010/08/02 21:14:47 exact Exp $


# =============================================================
# PATHS =======================================================
# =============================================================

RELATIVE_CORE_PATH=..

MODULES := linearAlgebra geometry2d geometry3d

L1_OBJS := $(foreach mod, ${MODULES}, $(mod)$(VAR)_level1.o)
L2_OBJS := $(foreach mod, ${MODULES}, $(mod)$(VAR)_level2.o)
L3_OBJS := $(foreach mod, ${MODULES}, $(mod)$(VAR)_level3.o)

# =============================================================
# VARIABLES, FLAGS ============================================
# =============================================================
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

INCLUDE=-I${CORE_PATH}/inc -I${CORE_PATH}/gmp/include \
	-I${CORE_PATH}/mpfr/include


COREX_LIB_LEVEL3=${CORE_PATH}/lib/libcorex++$(VAR)_level3.a
COREX_LIB_LEVEL2=${CORE_PATH}/lib/libcorex++$(VAR)_level2.a
COREX_LIB_LEVEL1=${CORE_PATH}/lib/libcorex++$(VAR)_level1.a
COREX_SHARED_LIB_LEVEL3=${CORE_PATH}/lib/libcorex++$(VAR)_level3.${CORE_DYLIB_EXTENSION}
COREX_SHARED_LIB_LEVEL2=${CORE_PATH}/lib/libcorex++$(VAR)_level2.${CORE_DYLIB_EXTENSION}
COREX_SHARED_LIB_LEVEL1=${CORE_PATH}/lib/libcorex++$(VAR)_level1.${CORE_DYLIB_EXTENSION}

TARGET=${COREX_LIB_LEVEL3} ${COREX_LIB_LEVEL2} ${COREX_LIB_LEVEL1}

ifeq (${LINKAGE}, shared)
  TARGET += $(COREX_SHARED_LIB_LEVEL3) $(COREX_SHARED_LIB_LEVEL2) \
            $(COREX_SHARED_LIB_LEVEL1)
endif

# =============================================================
# TARGETS =====================================================
# =============================================================

default: $(TARGET) 

graphicsTools: 
	cd graphics; ${MAKE}

# =============================================================
# RULES  ======================================================
# =============================================================


${COREX_LIB_LEVEL3}: ${L3_OBJS}
	-@rm -rf $@ 
	$(AR) $@ $?

COREX_SHARED_DEPS=-L${GMP_PREFIX}/lib -lgmp -L${MPFR_PREFIX}/lib -lmpfr -L${CORE_PATH}/lib -lcore++${VAR}

${COREX_SHARED_LIB_LEVEL3}: ${L3_OBJS}
	-@rm -rf $@ 
	$(CXX) $(COREX_SHARED_DEPS) $(CORE_LDFLAGS) -o $@ $?

${COREX_LIB_LEVEL2}: ${L2_OBJS}
	-@rm -rf $@ 
	$(AR) $@ $?

${COREX_SHARED_LIB_LEVEL2}: ${L2_OBJS}
	-@rm -rf $@ 
	$(CXX) $(COREX_SHARED_DEPS) $(CORE_LDFLAGS) -o $@ $?

${COREX_LIB_LEVEL1}: ${L1_OBJS}
	-@rm -rf $@ 
	$(AR) $@ $?

${COREX_SHARED_LIB_LEVEL1}: ${L1_OBJS}
	-@rm -rf $@ 
	$(CXX) $(COREX_SHARED_DEPS) $(CORE_LDFLAGS) -o $@ $?

%_level3.o:	%.cpp
	${CXX} $(CORE_CXXFLAGS) -DCORE_LEVEL=3 $(INCLUDE) -c $< -o $@

%_level2.o:	%.cpp
	${CXX} $(CORE_CXXFLAGS) -DCORE_LEVEL=2 $(INCLUDE) -c $< -o $@

%_level1.o:	%.cpp
	${CXX} $(CORE_CXXFLAGS) -DCORE_LEVEL=1 $(INCLUDE) -c $< -o $@

clean:
	-@test -z "*.o" || rm -f *.o
	-${MAKE} -C geom2d clean
	-${MAKE} -C geom3d clean

veryclean: clean
	-@test -z "$(COREX_LIB_LEVEL3)" || rm -rf "$(COREX_LIB_LEVEL3)"
	-@test -z "$(COREX_LIB_LEVEL2)" || rm -rf "$(COREX_LIB_LEVEL2)"
	-@test -z "$(COREX_LIB_LEVEL1)" || rm -rf "$(COREX_LIB_LEVEL1)"
	-@test -z "$(COREX_SHARED_LIB_LEVEL1)"||rm -rf "$(COREX_SHARED_LIB_LEVEL1)"
	-@test -z "$(COREX_SHARED_LIB_LEVEL2)"||rm -rf "$(COREX_SHARED_LIB_LEVEL2)"
	-@test -z "$(COREX_SHARED_LIB_LEVEL3)"||rm -rf "$(COREX_SHARED_LIB_LEVEL3)"
	-${MAKE} -C geom2d veryclean
	-${MAKE} -C geom3d veryclean

