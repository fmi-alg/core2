// Dijkstra's algorithm:

#include <queue>
#include <list>
#include "CORE/CORE.h"
#include "CORE/geometry2d.h"
using namespace std;

const int N=100; // max number of vertices

int n; // number of vertices (n<N)
int m; // number of edges
double  d[N];
Point2d V[N];
Point2d * pV = &V[];
Point2d tmp[N];
POint2d * ptmp = &tmp[];
list<int>  E[N]; // E[i] is the pointer to a linked list of adjacent vertices

int readInput(){
	Point2d p;
	double x,y;
	//cin >> n;  // number of points
	//cin >> m;  // number of edges
	assert(n<N);
   // Read input vertices using std routine in geometry2d.h
	n = readPoints(cin, pV, N, 0);
   // Construct adjacent list of edges from Boolean matrix
	m = readPoints(cin, ptmp, N, 0); 	// kludge to read edges  
	int j, k;
	for (int i=0; i<m; i++){
		j = tmp[i].X().intValue();
		k = tmp[i].Y().intValue();
		E[j].push_back(k); E[k].push_back(j);  // check!
	}
	return n;
}//readInput

void printGraph(){
   	cout << "Vertices:=========================" << endl;
	for (int i=0; i<n; i++){
		cout << "V[" << i << "] = " << V[i] << endl;
	}
   	cout << "Edges:============================" << endl;

	list<int>::iterator it;
	for (int i=0; i<n; i++){
		it = E[i].begin();
		cout << "E[" << i << "] = (" << *it ;
		for (; it != E[i].end(); ++it)
			cout << ", " << *it ;
		cout << ")" << endl;
	}
}//printGraph

int main(){

  readInput();
  printGraph();

/*
  queue Q; // priority queue

  d[0] = 0;  Q.insert(0,d[0]);

  for (int i=1; i<N; i++) {
	d[N]=-1;  // -1 means infinity
	Q.insert(i,d[i]);
  }

  int u;
  while (Q.notempty()) {
	u = Q.deletemin();
	  
  }
*/
  return 0;
}
