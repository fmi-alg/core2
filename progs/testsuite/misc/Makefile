# Makefile

# set the core library directory and include Make.config
CORE_PATH=../../..
CORE_DIR=../../..
include ${CORE_DIR}/progs/Make.options

# define targets list
TARGETS= \
	t_auto_ptr \
	t_policy \
	t_infty \
	t_double_prec \
	t_mpfr_prec \
	t_realib

# default target
all: ${TARGETS}

test: ${TARGETS}
	./t_auto_ptr 
	./t_policy 
	./t_infty 
	./t_double_prec 
	./t_mpfr_prec 
	./t_realib

moretest:
	-@echo "Nothing for moretest"
#@begin targets

t_auto_ptr: t_auto_ptr.o

t_policy: t_policy.o

t_infty: t_infty.o

t_double_prec: t_double_prec.o

t_mpfr_prec: t_mpfr_prec.o

t_realib: t_realib.o

#@end targets

#include standard rules from Make.rules
include ${CORE_DIR}/progs/Make.rules

#=================================================
# END
#=================================================

