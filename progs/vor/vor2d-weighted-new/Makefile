# Designed to work on Linux/MacOS.
# Thanks to David Bommes for suggestions about how to compile on MacOS.

# Operating system dependent variables.
ifeq ($(shell uname -s), Linux)
	CXX = g++-5
	LIB_BOOST_PO = -lboost_program_options
else # uname = "Darwin" (i.e. MacOS)
	CXX = clang++
	LIB_BOOST_PO = -lboost_program_options-mt
endif

CXXFLAGS = -std=c++11 -g
TARGET = subvor
SRCS = $(wildcard *.cpp)
OBJS = ${SRCS:.cpp=.o}
DEPS = $(SRCS:.cpp=.depends)

INCLUDE=-lpng $(LIB_BOOST_PO) -L/usr/lib -lglut -lglui -lGLU -lGL

# Build commands.
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(INCLUDE) -o $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

%.depends: %.cpp
	$(CXX) -M $(CXXFLAGS) $< > $@

clean:
	rm -f *~ *.o test_* benchmarks/*.pyc benchmarks/test_* $(TARGET)

# Test commands.
paper_figures:
	make shapes
	make random_examples

shapes:
	./subvor benchmarks/shapes-1-1-1 --aeps=0.001 &
	./subvor benchmarks/shapes-2-1-1 --aeps=0.001 &
	./subvor benchmarks/shapes-4-1-1 --aeps=0.001 &
	./subvor benchmarks/shapes-4-1-1 --geps=0.001 &

random_examples:
	python benchmarks/gen_segs.py 20 -d 300
	./subvor test_segs_d_300_20 &
	./subvor test_segs_d_300_20 --geps=0.001 --grid=false &
	python benchmarks/gen_mixed.py 10 10 
	./subvor test_mixed_10_pts_10_sgs --geps=0.001 --grid=false &
	python benchmarks/gen_points.py 20 -m -l 10
	./subvor test_m_l_10_20 --geps=0.001 --grid=false &

conics:
	./subvor benchmarks/conics/ellipse --save=true &
	./subvor benchmarks/conics/parabola --save=true &
	./subvor benchmarks/conics/hyperbola --save=true &

conics_highres:
	./subvor benchmarks/conics/ellipse --geps=0.001 --save=true &
	./subvor benchmarks/conics/parabola --geps=0.001 --save=true &
	./subvor benchmarks/conics/hyperbola --geps=0.001 --save=true &

rand_w_points_10:
	python benchmarks/gen_points.py 10 -w
	./subvor test_w_l_6_10 --aeps=0.001 &

rand_w_points_10_highres:
	python benchmarks/gen_points.py 10 -w
	./subvor test_w_l_6_10 --geps=0.001 &

rand_w_segs_10:
	python benchmarks/gen_segs.py 10 -w
	./subvor test_segs_w_l_6_10 --aeps=0.001 &

rand_w_segs_10_highres:
	python benchmarks/gen_segs.py 10 -w
	./subvor test_segs_w_l_6_10 --geps=0.001 &
