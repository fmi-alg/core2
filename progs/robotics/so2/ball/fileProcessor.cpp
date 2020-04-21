/* file: fileProcessor.cpp
 *
 * 	Usage:
 * 		> fileProcessor inputFileName outputFileName
 *
 * 	This will read the contents of "inputFileName",
 * 	viewing it as a sequence of lines,
 * 	and each line is viewed as a sequence of tokens.
 *	By definition, tokens are non-whitespaces that are separated
 *	by one or more white-spaces.   A white-space is either a space or a tab.
 *
 *	There are two special tokens:
 *		"#" is the comment line token
 *		"\\" is the continuation line token
 *
 * 	Then we will "clean up" these lines by:
 *
 * 		-- eliminating initial white-spaces
 * 		-- eliminating terminal white-spaces
 * 		-- replacing multiple white-spaces to one space
 * 		-- eliminating the "#" token and rest of line
 * 			CAREFUL:
 * 			This "#" char must be separated by white-spaces from other tokens!
 * 			E.g., "abc# xyz" and "abc #xyz" and "abc#xyz" do not count.
 * 		-- removing the continuation-line token "\\" PROVIDED it is the
 * 			last token in the current line, and join
 * 			the following line to the current line (this is recursive).
 * 			CAREFUL:
 * 			The "\\" char must be separated by white-spaces from other token!
 * 			E.g., "abc\\" does not count because "\\" is not separated.
 * 			E.g., "abc \\ xyz" doesn't count because "\\" is not the last token
 * 	
 * 	The result is written to the "outputFileName".
 *
 * 	KNOWN BUG:
 * 		If the last token is not "\\", we will output an empty last line.
 * 		Otherwise, we do not output any empty last line.
 *
 * 		The BUFFERSIZE is currently fixed.  Should be ease
 * 		to automatically increase its size if necessary.
 *
 * REFERENCES:
 *
 * 	Author: Chee Yap (April 2012)
 *
 * ***************************************************/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

// GLOBAL VARIABLES

unsigned int BUFFERSIZE = 200;	
char * lineBuffer = new char[BUFFERSIZE];	

ifstream ifile;

int counter=0;	// for debugging

/* *************************************************** 
 * HELPERS 
 *************************************************** */

// DEBUG TOOL: THIS IS TO PREVENT INFINITE LOOPS...
void check(){
    if (++counter > 200) exit(1);
    cout << "counter=" << counter << endl;
}

// check if token is "\\"
//
int isContinue(char *tok){
	string token(tok);
	return strcmp(tok,"\\");
}
// check if token is "#"
//
int isComment(char *tok){
	string token(tok);
	return strcmp(tok,"#");
}

// fill the lineBuffer
//     THIS NEVER WORKED UNLESS INLINED... WHY?
int getLineBuffer(){
    string line;
    getline (ifile,line);		// NOTICE that getline() is a global method!
    if (line.size()< BUFFERSIZE)
      	strcpy (lineBuffer, line.c_str());	// need to copy 
    else{
        cerr << "ERROR!  Buffer overflow!\n";	// should be easy to extend
	return 1;
    }
    return 0;
}//getnextLine


/* *************************************************** 
 * MAIN PROGRAM
 *************************************************** */

int fileProcessor(string infileName){
  //////////////////////////////////////////////////
  //files for input and output
  //////////////////////////////////////////////////
    ifstream ifile;
    ifile.open (infileName.c_str());

    ofstream ofile;
    ofile.open ("output-tmp.txt");	// temporary output

  //////////////////////////////////////////////////
  // Processing input file line-by-line
  //////////////////////////////////////////////////
  
  string line;			// for getline
  char *tok, *tokNext;
  bool emptyLine = true;	// do not output \n for an empty line...

  if (ifile.is_open())			// check if opened
  {
    while ( ifile.good() )		// while it is good
    {
	//getLineBuffer();
	getline (ifile,line);		// NOTICE that getline() is a global method!
	if (line.size()< BUFFERSIZE)
	      	strcpy (lineBuffer, line.c_str());	// need to copy 
	else{
	        cerr << "ERROR!  Buffer overflow!\n";	// should be easy to extend
		return 1;
	}
	
	tok = strtok (lineBuffer, " \t"); 	// whitespaces is \t or space
	emptyLine = true;
	while (tok != NULL) {
	  tokNext = strtok(NULL, " \t");
	  if ((isContinue(tok)==0)  && (tokNext == NULL)) {	// if tok is "\\",
	      do {
		//getLineBuffer ();		// then join to the next line
		getline (ifile,line);		// NOTE: getline() is a global method!
		if (line.size()< BUFFERSIZE)
	      	   strcpy (lineBuffer, line.c_str());	// need to copy 
		else{
	           cerr << "ERROR!  Buffer overflow!\n"; // should be easy to extend
		   return 1;
		}
		tok = strtok(lineBuffer, " \t");// get new tok
	      } while (tok == NULL);		// assert(We will find a non-null token)
	      continue; 			// do not break!
	  }// if "\\"
	  if (isComment(tok)==0)
	      break;
	  ofile << string(tok);
	  emptyLine = false;		// we have output SOME token!
	  if (tokNext != NULL)		// add space only between two tokens
	      ofile << " ";
	  tok = tokNext;
	}// while tok not NULL
	if (!emptyLine)
	    ofile << "\n";
    }//while good
    ifile.close();
  }//if
  else cout << "Unable to open file input file"; 	// error if cannot open
  ofile.close();

  return 0;
}

