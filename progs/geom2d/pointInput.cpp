/***********************************************
 * file: pointInput.ccp
 *
 * purpose:  A flexible facility input of points:
 * 		-- to allow comment '#' (rest of line omitted)
 * 		-- to load a sequence of points flexibly
 * 		-- somewhat variable point format
 * usage:  The main method is
 * 
 * 	     int readPoints(istream is, Point2d* S, int MaxN, int N = 0)
 *
 *		-- this reads a sequence of points from
 *		   from "is" into an array "S".
 *		-- If N > 0, then at most max(MaxN, N) will be read.
 *		-- If N = 0, then the first number in istream "is" must
 *		   be a number N' and at most max(MaxN, N') will be read.
 *		-- The stream is a sequence of n points in the form
 *		   	( x1 , y1 ) ( x2 , y2 ) ... ( xn , yn )
 *		   where all the spaces can be replaced by any white
 *		   space, including no space.  Note that this
 *		   sequence might be preceded by an initial number N'.
 *		-- the commas and parentheses can also be replaced by
 *		   an non-empty white spaces.  E.g.,
 *		   	x1 y1 x2 y2 ... xn yn 
 *		   or even
 *		   	(x1 y1) x2 , y2 ... xn , yn 
 *	     	-- If the comment character '#' is encountered, the
 *	     	   rest of line is ignored.
 *
 * 	     This method is now in geometry2d.h, and a
 * 	     similar facility for geometry3d will be added.
 *
 * author: Chee Yap (yap@cs.nyu.edu)
 * date:   Sep 28, 2003
 *
 * Since CORE Library Version 1.6
 * $Id: pointInput.cpp,v 1.1 2006/03/07 04:51:24 exact Exp $
 **********************************************/

#include "CORE/CORE.h"
#include "CORE/geometry2d.h"

using namespace std;

#define MaxN 100
int main() {
	Point2d S[MaxN];
	int n = readPoints(cin, S, MaxN, 0);
	 	// N is the number of points to read
	 	// if N=0, then input file must specify
	 	// the number of points to read
	cout << "n= " << n << endl;
	for (int i=0; i<n; i++)
		cout << i << ": " << S[i] << endl;

	return 0;
}

/*
// PROPOSED NEW version of operator>>(..).
// It uses the auxilliary functions:
// 	startNum(char),
// 	getToChar(istream, char),
// 	getToNum(istream, char).

// startNum(c): check if char c can be the start of a literal number
  // i.e., if c occurs in string '01234567890+-.'
bool XstartNum(char c) {
  switch(c) {
    case '0': ;  case '1': ;  case '2': ;  case '3': ;  case '4': ;  
    case '5': ;  case '6': ;  case '7': ;  case '8': ;  case '9': ; 
    case '+': ;  case '-': ; 
    case '.': return true; break;
  }
  return false;
}//startNum(c)

// getToChar(istream, mark):
// 	-- moves input cursor to the next occurrence of mark
// 	-- in any case, it will stop if it gets to the beginning
// 	   of the next number (indicated by startNum()).
bool XgetToChar( std::istream& in, char mark) {
  char c;
  char buf[256];
  bool got_mark = false;
  do {
	in.get(c); 
	if (!isspace(c)) {
	     if (c == mark) {
		got_mark = true; // this is what we were looking for
	     } else if (c == '#') {	// rest of line is discarded as comment
		in.getline(buf,256);
	     } else if (XstartNum(c)) {
		     //IOErrorFlag = 2;	// I/O Error 2:  did not find
					// mark before next number
		in.putback(c);		// time to stop looking!
		return in;		// Usually not considered error
	     } else {
		     //IOErrorFlag = 1;	// I/O Error 1: 
		return in;		// Unexpected character
	     }
	}
  } while (!(got_mark || in.eof()));
  return (got_mark);
}//getToChar

// getToNum(istream, mark, strict):
// 	-- moves input cursor to the beginning of the next number. 
bool XgetToNum( std::istream& in, char mark, bool strict=false) {
	// default value for strict is false
  // NOTES: We ignore spaces, comment lines
  // and accept at most one copy of the "mark" character.
  // [It must be exactly one copy when strict=true]
  // If so, we return true.
  // Return false if reach eof with no number, or
  // saw 2 "mark" characters, or saw ANY other unexpected chars.
  char c;
  char buf[256];
  bool got_mark = false;
  bool got_num = false;

  do {
	in.get(c); 
	if (!isspace(c)) {
	     if (c == mark) {
		if (got_mark) {
			//IOErrorFlag = 2;	// I/O Error 2:
			return in; 		// Second mark found
		} else got_mark = true; // this is what we were looking for
	     } else if (XstartNum(c)) {	// we may get this before finding mark
		in.putback(c);	// but we accept this (not considered error)
		got_num = true;
	     } else if (c == '#') {	// rest of line is discarded as comment
		in.getline(buf,256);
	     } else {
		     //IOErrorFlag = 1;	// I/O Error 1: 
		return in;		// Unexpected character
	     }
	}
  } while (!(got_num || in.eof()));
  return (got_num && (got_mark || !strict));
}
// Reads an input point which has the format
// 	( nnn , nnn )
// where the white spaces, "(", "," and ")" are all optional.
std::istream& readPt(std::istream& in, Point2d& p)
{
	// NOTE: No other non-white chars may be used.  Whenever we see #, 
	// the rest of line is omitted, and it counts as white space.
	// E.g., "[ nnn : nnn ]" is no good but "( nnn, nnn) #xxx" is OK.
  double x, y;  

  // if (!(XgetToNum(in, '(') || IOErrorFlag || in.eof())) 
  if (!(XgetToNum(in, '(') || in.eof())) 
	return in;		// No first value
  else
  	in >> x;
  // if (!(XgetToNum(in, ',') ||  IOErrorFlag ||in.eof())) 
  if (!(XgetToNum(in, ',') || in.eof())) 
	return in;		// no second value
  else
	in >> y;
  XgetToChar(in, ')');	// This terminates if the start of the next number
  p = Point2d(x, y);	// 	occurs before ')'.
  return in;
} // operator>>

// reads a sequence of points from input stream iS into Point2d array pA.
int XreadPoints(std::istream & iS, Point2d *pA, int MaxN, int N=0){
  // DEFAULT VALUES: MaxN=1000, N=0
  // The input stream constains a sequence of 2K+1 numbers of the form
  //
  //     [NN]   ( x1 ,  y1 )  ( x2 , y2 )  ... ( xK , yK )
  //
  // The i-th point is (xi, yi). 
  //    (0) NN is optional if N is given as argument (then N is set to NN)
  //    (1) Any of the "(", "," and ")" are optional
  //    (2) Newlines, extra white spaces, '#' are all ignored.
  //    (3) Also, everything after '#' is discarded.
  // If N > MaxN, nothing is read and 0 is returned.  
  // Returns the number of points actually read, i.e, min(K, N).
  
  int i;
  // IOErrorFlag = 0;		// initially no error
  if (N == 0) {			// read NN from input
     if (XgetToNum(iS, ' '))
  	iS >> N;
     else
  	return 0;
  }
  if (N > MaxN) return 0;
  for (i = 0; i < N; i++){
	  if (iS.eof()) break;
	  // if (!IOErrorFlag) {
	  if (true) {
	  	// iS >> pA[i];
		readPt(iS, pA[i]);
	  } else {
		i = 0;
	        break;
	  }
  }
  return i;  // number of points read
}

int main() {
	int N = 0;
	Point2d S[20];
	 int n = XreadPoints(cin, S, 20, N);
	 	// N is the number of points to read
	 	// if N=0, then input file must specify
	 	// the number of points to read
	cout << "n= " << n << endl;
	 for (int i=0; i<n; i++)
		cout << i+1 << ": " << S[i] << endl;

	return 0;
}

************************************************** */
