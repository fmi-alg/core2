/* file: fileReader.cpp
 *
 * This tests the class fileReader defined in fileReader.h.
 * Basically, the idea is to process any input text file
 * and output a sequence of tokens.  The input file consists of
 * lines with two special characters:  
 *
 * 	comment char ('#')	-- ignore rest of line
 * 	continuation char ('\')	-- ignore rest of line AND join to next line
 * 
 * After processing for '#' and '\', we remove any initial
 * or terminal white spaces (' ' or '\t').
 * Then replace any contiguous sequence of one or more
 * white spaces (' ' or '\t') by a single space (' ').
 *
 * What results is a sequence of lines, consisting of tokens
 * separated by a single white space.
 *
 * 	USAGE:
 * 		> fileReader
 *
 * 	Author: Chee Yap (April 2012, Dec 2018)
 *
 * ***************************************************/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "fileReader.h"

using namespace std;

/* *************************************************** 
 * MAIN PROGRAM
 *************************************************** */
int main (void) {
  //////////////////////////////////////////////////
  // Create sample input file
  sampleFile("example.txt");

  //////////////////////////////////////////////////
  // Create fileReader instance
  fileReader myReader("example.txt");

  //////////////////////////////////////////////////
  // Do the reading main loop:
  
  while (myReader.nextOutputLine())
      cout << "outputLine: " << myReader.outputLine << endl;

  validate();
  return 0;
}

