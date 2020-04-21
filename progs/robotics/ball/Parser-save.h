// Parser.h
//	We read a text file following the conventions of "inputs/format.h".
//
//	We must be sure that each triangular face follows the CCW convention
//		when seen from the "outside"

#include <iostream>

string fileName("input1.txt");     // Input file name
string inputDir("inputs");     // Path for input files
double scale=1;        // scaling of input environment
double deltaX=0;      // x-translation of input environment
double deltaY=0;      // y-translation of input environment
double deltaZ=0;      // z-translation of input environment

extern int fileProcessor(string inputfile);

/* ********************************************************************** */
// skip blanks, tabs, line breaks and comment lines,
//   leaving us at the beginning of a token (or EOF)
//   (This code is taken from core2/src/CoreIo.cpp)
int skip_comment_line (std::ifstream & in) {
  int c;

  do {
    c = in.get();
    while (c == '#') {
      do {// ignore the rest of this line
	c = in.get();
      } while (c != '\n');
      c = in.get(); // now, reach the beginning of the next line
    }
  } while (c == ' ' || c == '\t' || c == '\n');  //ignore white spaces and newlines

  if (c == EOF)
    std::cout << "unexpected end of file." << std::endl;

  in.putback(c);  // this is non-white and non-comment char!
  return c;
}//skip_comment_line

// skips '\' followed by '\n'
//   NOTE: this assumes a very special file format (e.g., our BigInt File format)
//   in which the only legitimate appearance of '\' is when it is followed
//   by '\n' immediately!
int skip_backslash_new_line (std::istream & in) {
  int c = in.get();

  while (c == '\\') {
    c = in.get();

    if (c == '\n') {
      c = in.get();
    } else { // assuming the very special file format noted above!
      cout<< "continuation line \\ must be immediately followed by new line.\n";
    }
  }//while
  return c;
}//skip_backslash_new_line

/* ********************************************************************** */

void parseConfigFile(Box* b) {
  //bool isQuad = false; // used to check whether the face is a quadrilateral or triangle
  int isQuad = 0; // used to check whether the face is a quadrilateral or triangle
  		  // isQuad=0 means a triangle
		  // isQuad=1 means a quadrilateral

  std::stringstream ss;
  ss << inputDir << "/" << fileName;  // create full file name
  std::string s = ss.str();
  cout << "input file name = " << s << endl;

  fileProcessor(s);  // this will clean the input and put in
  // output-tmp.txt

  ifstream ifs("output-tmp.txt");
  if (!ifs) {
    cerr<< "cannot open input file" << endl;
    exit(1);
  }

  // First, get to the beginning of the first token:
  skip_comment_line(ifs);

  int nPt = 0;
  int nPolyhedra = 0;
  ifs >> nPt;
  cout << "nPt=" << nPt << endl;

  vector<double> pts(nPt * 3);
  for (int i = 0; i < nPt; ++i) {
    ifs >> pts[i * 3] >> pts[i * 3 + 1] >> pts[i * 3 + 2];
  }

  while (true) {
    int numFaces;
    ifs >> numFaces;
    cout << numFaces << " faces" << endl;

    if (numFaces == 0) {
      break;
    }
    nPolyhedra++;
    for (int i = 0; i < numFaces; ++i) {
      vector<Corner*> ptVec;

      ifs >> isQuad;

      for (int j=0; j< 3; ++j){
	int pt;
	ifs >> pt;
	pt--;	// to get indexing from 0
	ptVec.push_back(new Corner(pts[pt*3]*scale+deltaX,
				   pts[pt*3+1]*scale+deltaY, pts[pt*3+2]*scale+deltaZ));
	b -> addCorner(ptVec.back());
      }
      // First triangle
	Edge* e1 = new Edge (ptVec[0], ptVec[1]);
	Edge* e2 = new Edge (ptVec[1], ptVec[2]);
	Edge* e3 = new Edge (ptVec[2], ptVec[0]);
	b -> addEdge(e1);
	b -> addEdge(e2);
	b -> addEdge(e3);

	Wall* w = new Wall (ptVec[0], ptVec[1], ptVec[2]);
	b -> addWall(w);
      // Possible Second triangle:
      if (isQuad == 1) {   // i.e., a quadrilateral
	Corner* fourthPt = new Corner(ptVec[0], ptVec[1], ptVec[2]); // use special
				// constructor of fourth corner from 3 others.
	Edge* e1 = new Edge (ptVec[2], ptVec[1]);
	Edge* e2 = new Edge (ptVec[1], fourthPt);
	Edge* e3 = new Edge (fourthPt, ptVec[2]);
	b -> addEdge(e1);
	b -> addEdge(e2);
	b -> addEdge(e3);

	Wall* w = new Wall (ptVec[1], fourthPt, ptVec[2]);
	b -> addWall(w);
      }
      // THIS LOGIC IS WRONG FOR QUAD FACES:
      /**
      for (int j = 0; j < 4; ++j) {
	int pt;
	ifs >> pt;

	// if the first pt is 0, the face is a triangle, so continue with the next 3 pts
	if (j == 0 && pt == 0) {
	  continue;
	} else if (j == 0 && pt != 0) {
	  isQuad = true;
	}
	pt -= 1;
	ptVec.push_back(new Corner(pts[pt*3]*scale+deltaX,
				   pts[pt*3+1]*scale+deltaY, pts[pt*3+2]*scale+deltaZ));
	b -> addCorner(ptVec.back());
      }

      if (isQuad) {
	// Break up rectangle into 2 triangles

	// First triangle
	Edge* e1 = new Edge (ptVec[0], ptVec[1]);
	Edge* e2 = new Edge (ptVec[1], ptVec[2]);
	Edge* e3 = new Edge (ptVec[2], ptVec[0]);
	b -> addEdge(e1);
	b -> addEdge(e2);
	b -> addEdge(e3);

	Wall* w = new Wall (ptVec[0], ptVec[1], ptVec[2]);
	b -> addWall(w);

	// Second triangle
	Edge* e4 = new Edge (ptVec[0], ptVec[2]);
	Edge* e5 = new Edge (ptVec[2], ptVec[3]);
	Edge* e6 = new Edge (ptVec[3], ptVec[0]);
	b -> addEdge(e4);
	b -> addEdge(e5);
	b -> addEdge(e6);

	Wall* w2 = new Wall (ptVec[0], ptVec[2], ptVec[3]);
	b -> addWall(w2);
      } else {
	Edge* e1 = new Edge (ptVec[0], ptVec[1]);
	Edge* e2 = new Edge (ptVec[1], ptVec[2]);
	Edge* e3 = new Edge (ptVec[2], ptVec[0]);
	b -> addEdge(e1);
	b -> addEdge(e2);
	b -> addEdge(e3);

	Wall* w = new Wall (ptVec[0], ptVec[1], ptVec[2]);
	b -> addWall(w);
      }
      **/

    }//for i
  }//while

  cout<< "nPolyhedra=" << nPolyhedra << endl;
  ifs.close();
}
