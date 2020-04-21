# Makefile for hyperGeom 
#
# (template Makefile for simple targets)
#
# Core Library, $Id: Makefile,v 1.1 2006/03/07 04:54:38 exact Exp $

CORE_PATH=../../..
include ../../Make.options

PREC 	= 500

#=================================================
# Define target files (put all your files here)
#=================================================
TARGETS= timehyper timepi timelog

all: $(TARGETS)

test:
	./timepi $(PREC)

test-timepi:
	echo "bits\ton the fly\tprecomputed" > output/timepi.out
	./timepi 100 >> output/timepi.out
	./timepi 1000 >> output/timepi.out
	./timepi 3000 >> output/timepi.out
	./timepi 5000 >> output/timepi.out
	./timepi 7000 >> output/timepi.out
	./timepi 9000 >> output/timepi.out
	./timepi 10000 >> output/timepi.out
	./timepi 20000 >> output/timepi.out

test-timeexp:
	echo "timing for computing exp(1)" > output/timeexp.out
	echo "bits\ttime" >> output/timeexp.out
	./timehyper exp 1 100 2000 200 >> output/timeexp.out

test-timelog:
	echo "timing for computing log(1.3)" > output/timelog.out
	echo "bits\ttime" >> output/timelog.out
	./timelog 1.3 100 600 100 >> output/timelog.out

timelog: timelog.o

timehyper: timehyper.o

timepi: timepi.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) $(CORE_INC) -I../ $< -o $@

#=================================================
# Clean object files
#=================================================
clean:
	-@test -z "*.o" || rm -f *.o

#=================================================
# Remove executable files
#=================================================
EXEPROGS=$(TARGETS:=$(EXETYPE))

veryclean: clean
	-@test -z "$(EXEPROGS)" || rm -f $(EXEPROGS)

rcs:
	-@ci -l -m"make rcs" *.cpp *.h Makefile README
