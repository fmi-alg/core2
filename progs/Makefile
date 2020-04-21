# Makefile for programs in ${CORE}/progs
############################################################

############################################################
# variables
############################################################

### The programs in "basicprogs" will be automatically tested
### 	on ALL platforms when you do
###	"make test" or "make time":

basicprogs=bareiss \
	benchmark \
	bounds \
	chull \
	core2paper \
	compare \
	demos \
	determinant \
	fileIO \
	fortune \
	gaussian \
	geom2d \
	generic \
	interval \
	kahan \
	pi \
	poly \
	sumOfSqrts \
	testIdent \
	testIO \
	testsuite \
	tutorial

### The programs in "moreprogs" will be automatically tested
### 	on ALL unix-type platforms (sun, sgi, cyg, gnu) when you do
###	"make moretest" or "make moretime":
###	NOTE: "moreprogs" is a superset of "basicprogs"

moreprogs= \
	chullGraham \
	complex \
	curves \
	delaunay \
	ieee \
	gaussian \
	geom3d \
	heron \
	hypergeom \
	mesh \
	nestedSqrt \
	pentagon \
	prover \
	radical \
	testFilter \
	tests

### The programs in extraprogs are not automatically testd on
### 	any platforms.  You will have to run it yourself:

extraprogs=cgal \
	determinant \
	continuedFrac \
	constants \

allprogs= ${basicprogs} ${moreprogs} ${extraprogs}

############################################################
# targets
############################################################
default: makebasic

# only make the programs but do not test them
basic makebasic:
	@for p in ${basicprogs}; do \
	  if test -d "$$p"; then \
		echo "Inside subdirectory " $$p ": =-=-=-=-=-=-=-=->>>"; \
		${MAKE} -C $$p; echo ""; fi; \
	done

# make the programs and also test them
test testbasic:
	@for p in ${basicprogs}; do \
	  if test -d "$$p"; then \
		echo "Inside subdirectory " $$p ": =-=-=-=-=-=-=-=->>>"; \
	        ${MAKE} -C $$p test; echo ""; fi; \
	done

# download the basic programs by SVN (Jan2018)
downloadBasicProgs: 
	@for p in ${basicprogs}; do \
	  if test -d "$$p"; then \
		svn update --set-depth infinity $$p; fi; \
	done
	

############################################################
# NOTE: "moreprogs" is "more" than "basicprogs" in two ways:
# 	(1) we have more programs than in basicprogs
# 	(2) the basic programs may be tested with harder inputs.
############################################################

# only make the programs but do not test them
more moreprogs makemoreprogs: makebasic
	@for p in ${moreprogs}; do \
	  if test -d "$$p"; then \
		echo "Inside subdirectory " $$p ": =-=-=-=-=-=-=-=->>>"; \
		${MAKE} -C $$p; echo ""; fi; \
	done

# make the programs and also test them
moretest: moreprogs
	@for p in ${moreprogs}; do \
	  if test -d "$$p"; then \
		echo "Inside subdirectory " $$p ": =-=-=-=-=-=-=-=->>>"; \
		${MAKE} -C $$p moretest test; echo ""; fi; \
	done
	@for p in ${basicprogs}; do \
	  if test -d "$$p"; then \
		echo "Inside subdirectory " $$p ": =-=-=-=-=-=-=-=->>>"; \
		${MAKE} -C $$p moretest; echo ""; fi; \
	done

############################################################
# Extra and all
# 	--"extra" just means extra programs,
# 		but we just do "test" on each of them
#	--"all" is combination of "moreprogs" and "extra"
############################################################

extra:
	@for p in ${extraprogs}; do \
	  if test -d "$$p"; then \
		echo "Inside subdirectory " $$p ": =-=-=-=-=-=-=-=->>>"; \
		${MAKE} -C $$p; echo ""; fi; \
	done

extratest: 
	@for p in ${extraprogs}; do \
	  if test -d "$$p"; then \
		echo "Inside subdirectory " $$p ": =-=-=-=-=-=-=-=->>>"; \
		${MAKE} -C $$p test; echo ""; fi; \
	done

all makeall:  extra moreprogs

############################################################
# Housekeeping
############################################################
clean: cleanbasic
	@for p in ${moreprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p clean; echo ""; fi; \
	done

cleanbasic: 
	@for p in ${basicprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p clean; echo ""; fi; \
	done

veryclean: verycleanbasic
	@for p in ${moreprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p veryclean; echo ""; fi; \
	done

verycleanbasic: 
	@for p in ${basicprogs}; do \
	  if test -d "$$p"; then echo $$p; ${MAKE} -C $$p veryclean; echo ""; fi; \
	done

############################################################
# End
############################################################

