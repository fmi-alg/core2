# Makefile

# set the core library directory and include Make.config
CORE_DIR=../..
CORE_PATH=../../..
include ${CORE_DIR}/Make.options

# define targets list
TARGETS= \
	t_root \
	t_prec_round \
	t_get_str

# default target
all: ${TARGETS}

test: ${TARGETS}
	./t_root
	./t_get_str
	./t_prec_round

moretest:
	-@echo "Nothing for moretest"
	

#@begin targets

t_root: t_root.o

t_prec_round: t_prec_round.o

t_get_str: t_get_str.o

#@end targets

#include standard rules from Make.rules
include ${CORE_DIR}/Make.rules

#=================================================
# END
#=================================================

