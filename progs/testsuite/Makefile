# Makefile

# set the core library directory and include Make.config
CORE_DIR=../..
include ${CORE_DIR}/progs/Make.options

# define targets list
TARGETS= \
	t_bigint \
	t_bigrat \
	t_bigfloat \
	t_expr \
	t_poly \
	t_sturm \
	t_rootOf \
	kahan

# default target
all: ${TARGETS}

test: ${TARGETS}
	./t_bigint 
	./t_bigrat 
	./t_bigfloat 
	./t_expr 
	./t_poly
	./t_sturm
	./t_rootOf
	./kahan
	
# More test: go into subdirectories to do test and more test there!
moretest: 
	cd misc; make test; make moretest
	cd mpfr; make test; make moretest

#@begin targets

t_bigint: t_bigint.o

t_bigrat: t_bigrat.o

t_bigfloat: t_bigfloat.o

t_expr: t_expr.o

t_poly: t_poly.o

t_sturm: t_sturm.o

t_rootOf: t_rootOf.o

kahan: kahan.o

t_tmp: t_tmp.o

tt: tt.o

#@end targets

include ${CORE_DIR}/progs/Make.rules

#==================================================
#==================================================
