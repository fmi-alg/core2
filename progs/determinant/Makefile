# Makefile for determinant
#
# Core Library, $Id: Makefile,v 1.3 2010/01/20 21:50:18 exact Exp $

include ../Make.options

#=================================================
# Define target files (put all your files here)
#=================================================
LEVEL= 3
TARGETS= determinant

all: $(TARGETS)

test: $(TARGETS)
	./determinant inputs/random_50bits_dim_5 10
	./determinant inputs/random_100bits_dim_5 10
	./determinant inputs/random_200bits_dim_5 10
	./determinant inputs/random_500bits_dim_5 10
	# ./determinant inputs/random_double_2 10
	# ./determinant inputs/random_double_5 10
t : det
	./det inputs/random_50bits_dim_5 10

determinant: determinant.o

det: det.o

createMatrices: createMatrices.o

experiment1: determinant
	./determinant inputs/random_100bits_dim2 1
	./determinant inputs/random_100bits_dim3 1
	./determinant inputs/random_100bits_dim4 1
	./determinant inputs/random_100bits_dim5 1

experiment2:
	time ./determinant inputs/random_100bits_dim2_degenerate 100000
	time ./determinant inputs/random_100bits_dim3_degenerate 10000
	time ./determinant inputs/random_100bits_dim4_degenerate 1000
	time ./determinant inputs/random_100bits_dim5_degenerate 1000

experiment3:
	time ./determinant inputs/random_50digits_dim2 1000000
	time ./determinant inputs/random_50digits_dim3 1000000
	time ./determinant inputs/random_50digits_dim4 100000
	time ./determinant inputs/random_50digits_dim5 10000

experiment4:
	time ./determinant inputs/random_50digits_dim2_degenerate 10000
	time ./determinant inputs/random_50digits_dim3_degenerate 10000
	time ./determinant inputs/random_50digits_dim4_degenerate 1000
	time ./determinant inputs/random_50digits_dim5_degenerate 1000

experiment5:
	time ./determinant inputs/random_50digits_dim3_degenerate 10000
	time ./determinant inputs/random_50digits_dim4_degenerate 10000
	time ./determinant inputs/random_50digits_dim5_degenerate 10000
	time ./determinant inputs/random_50digits_dim6_degenerate 10000

experiment6:
	(time ./determinant inputs/random_50digits_dim3_degenerate 10000) \
		2>&1 | tee outputs/time_rand_50dig_dim3_deg_ALL
	(time ./determinant inputs/random_50digits_dim4_degenerate 10000) \
		2>&1 | tee outputs/time_rand_50dig_dim4_deg_ALL
	(time ./determinant inputs/random_50digits_dim5_degenerate 10000) \
		2>&1 | tee outputs/time_rand_50dig_dim5_deg_ALL
	(time ./determinant inputs/random_50digits_dim6_degenerate 10000) \
		2>&1 | tee outputs/time_rand_50dig_dim6_deg_ALL

experiment7:
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim3_degenerate 10000 0
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim3_degenerate 10000 1
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim3_degenerate 10000 2
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim4_degenerate 10000 0
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim4_degenerate 10000 1
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim4_degenerate 10000 2
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim5_degenerate 10000 0
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim5_degenerate 10000 1
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim5_degenerate 10000 2
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim6_degenerate 10000 0
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim6_degenerate 10000 1
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim6_degenerate 10000 2

experiment8:
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim2_degenerate 10000 0
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim2_degenerate 10000 1
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim2_degenerate 10000 2
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim3_degenerate 10000 0
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim3_degenerate 10000 1
	@echo "=========================================================="
	time ./determinant inputs/random_50digits_dim3_degenerate 10000 2
	@echo "=========================================================="


matrices: createMatrices
	./createMatrices inputs/random_100bits_dim2 1 2 100 2
	./createMatrices inputs/random_100bits_dim3 1 3 100 2
	./createMatrices inputs/random_100bits_dim4 1 4 100 2
	./createMatrices inputs/random_100bits_dim5 1 5 100 2
	./createMatrices inputs/random_100bits_dim6 1 6 100 2

matrices10: createMatrices
	./createMatrices inputs/random_50digits_dim2 1 2 50 10
	./createMatrices inputs/random_50digits_dim3 1 3 50 10
	./createMatrices inputs/random_50digits_dim4 1 4 50 10
	./createMatrices inputs/random_50digits_dim5 1 5 50 10
	./createMatrices inputs/random_50digits_dim6 1 6 50 10

bigmatrices: createMatrices
	./createMatrices inputs/bigrandom_50bits_dim2  1  2 50 2
	./createMatrices inputs/bigrandom_50bits_dim4  1  4 50 2
	./createMatrices inputs/bigrandom_50bits_dim6  1  6 50 2
	./createMatrices inputs/bigrandom_50bits_dim10 1 10 50 2
	./createMatrices inputs/bigrandom_50bits_dim20 1 20 50 2
	./createMatrices inputs/bigrandom_50bits_dim50 1 50 50 2

#=================================================
# Rules
#=================================================
%: %.o
	${CXX} $(LDFLAGS) $< -lcorex++$(VAR)_level${LEVEL} $(CORE_LIB) -o $@

.cpp.o:
	${CXX} -c $(CXXFLAGS) -DCORE_LEVEL=${LEVEL} $(CORE_INC) $< -o $@

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

