SQ=smooth_quadtree_test

build:
	g++ -std=c++11 -g smooth_quadtree_test.cpp -o $(SQ)
test:
	./$(SQ)
valgrind:
	valgrind ./$(SQ) -v --leak-check=full --track-origins=yes
clean:
	rm -f *~ *.o $(SQ)
