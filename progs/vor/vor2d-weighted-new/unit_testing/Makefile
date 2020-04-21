CXX = g++-5
CXXFLAGS = -std=c++11 -g

build:
	$(CXX) $(CXXFLAGS) -o interval-test Interval-test.cpp
	$(CXX) $(CXXFLAGS) -o bipoly-test BiPoly-test.cpp

itest:
	./interval-test

bptest:
	./bipoly-test

clean:
	rm -f *~ *-test *.o
