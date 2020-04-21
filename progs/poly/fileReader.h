#ifndef CORE_FILE_READER_H
#define CORE_FILE_READER_H


/* file: fileReader.h
 *
 * 	The "fileReader" class is used to convert 
 * 		a sequence of "inputLines" into a sequence of "outputLines".
 * 	Special chars are those in this string:   " \t\n:#\\"
 * 		(space, tab, newline, '#', backslash)
 * 			'#' is the comment char
 * 			'\\' or backslash is the continuation char
 * 	The notion of an "inputLine" is standard -- it
 * 		means a maximal sequence of non-\n characters in
 * 		input file.  Note that a textLine may be empty.
 * 	An "uncommentedLine" is an inputLine in which we first
 * 		remove the first occurrence of '#' and rest of the line.
 * 		(If there is no occurrence of '#', then the uncommentedLine
 * 		is just the inputLine.)
 * 	An "longLine" is obtained from one or more consecutive
 * 		uncommentedLines, by a process rather similar to
 * 		uncommenting a line.  More precisely, we first remove
 * 		the first occurence of '\\' (backslash) and the rest of line,
 * 		and then append the next uncommentedLine to the result.
 * 		We repeat this process if there is more occurence of
 * 		'\\' in the joined line. When this process stops,
 *		the result is called a "longLine".
 * 	Tokens are maximal sequences of non-special characters in a longLine.
 * 		Since a longLine has NO occurences of "\n#\\" 
 * 			(newline, '#', backslash)
 * 		but may have "whiteSpaces" (i.e., space or tab),
 * 		it means that all the tokens are separated by one or
 * 		more white spaces.
 * 		Thus, a "longLine" can be viewed as a sequence of tokens.
 * 		But if it has no tokens, we say that it is empty.
 * 		Empty "longLines" are discarded.
 * 	Each "outputLine" is obtained from a non-empty longLine by replace
 * 		a single "whiteSpace" between any two tokens;
 * 		Moreover initial and terminal whiteSpaces are removed
 * 		from the "outputLine".  It follows that an "outputLine"
 * 		contains at least one token.
 *	In summary:
 *		inputLine --> uncommentedLine --> longLine --> outputLine
 *	or
 *		1Line 	  --> 2Line	      --> 3Line	   --> 4Line
 *
 *	Thus there are three transformation steps.
 *	Rather than doing these transformations by reading and writing
 *		files in 3 three successive passes, we want to do this in
 *		one pass.  What makes the code tricky to ensure that for
 *		each "xLine --> yLine" transformation, the "xLine" is nonempty.
 *
 * 	We define a class "fileReader" that convert an input text file into an
 * 	output text file which containing a sequence of outputLines.
 * 	We model our program after "std::strtok" which has state information,
 * 	and which, after the first call, will keep delivering new
 * 	tokens until end of string. 
 * 	Likewise, a "fileReader" instance will keep delivering
 *	the next "outputLine" of tokens.  
 *
 *	See fileReader.cpp for example of how to use fileReader.h.
 * 		We provide a method to create a sample input file "example.txt"
 * 		and a method to validate the result of processing "example.txt".
 *
 * 	   NOTE: "\\" is similar to "#".  Note that in our
 * 	   previous design, the "\\" token only has effect as a continuation
 * 	   token when it is the last token of a txt line.  We no longer
 * 	   require this condition.
 *
 * 	UTILITY:
 * 	The first token of an outputLine is called a "label"
 * 		if it terminates in a colon ':'. 
 * 			E.g., "label: blah blah..."
 * 			E.g., "op: blah blah..."
 *		Depending on application, we may also view this label
 *		as an operand.  We have two useful methods on outputLines:
 *
 * 			getLabel(outPutLine) returns the label
 * 			getRest(outPutLine) returns the non-label part
 *
 * 		E.g.,
 * 			getLabel("label: blah blah...") returns "label"
 * 			getRest("label: blah blah...") returns "blah blah..."
 *
 *		However, if there is no label, they behave as follows:
 * 			getLabel("blah blah...") returns empty string
 * 			getRest("blah blah...") returns "blah blah..."
 *
 * Author: Chee Yap (April 2012, Dec 2018)
 *
 * ***************************************************/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

class fileReader 
{
/* *************************************************** 
 * MEMBERS 
 *************************************************** */
    private:
        std::string COMMENT = "!";	// Frisco uses "!" but we like "#"
					// Note: can use a longer string.
	unsigned int BUFFERSIZE = 1000;
	std::ifstream ifile;
	std::ofstream ofile;

	std::string inputLine;	// from getline, but without comments
	char * lineBuffer = new char[BUFFERSIZE];	
	char *tok, *tokNext;
	int counter=0;

    public:
	std::string outputLine;	// for collecting tokens
/* *************************************************** 
 * CONSTRUCTORS 
 *************************************************** */
public:
fileReader(std::string fname, std::string c="#") {
        COMMENT = c;
    	ifile.open (fname.c_str());
  	if (! ifile.is_open()) {			// check if opened
		std::cerr << "ERROR!  file cannot open!\n";
		exit(1);
	}
  	ofile.open ("__tmp_fileReader.txt");	// diagnostic file
	ofile << "__tmp_fileReader.txt: \n"
	      << "this is a tmp file created by fileReader instance"
	      << std::endl;
	}//fileReader
/* *************************************************** 
 * HELPERS 
 *************************************************** */
private:
  // DEBUG TOOL: THIS IS TO PREVENT INFINITE LOOPS...
  //////////////////////////////////////////////////
  void check(){
    if (++counter > 200)
	exit(1);
    if (counter%20 == 0) 
    	std::cout << "counter=" << counter << std::endl;
  }
  // isContinue(tok) returns true iff tok is "\\" 
  //////////////////////////////////////////////////
  bool isContinue(char *tok){
      std::string token(tok);
	return (strcmp(tok,"\\")==0);
  }

  // trimComment(ll) will removes the first occurence of 
  // 	COMMENT and whatever follows.
  //////////////////////////////////////////////////
  void trimComment(std::string & ll){
	size_t pch = ll.find(COMMENT);
	if (pch != std::string::npos)
	    ll.erase(pch);
  }

  // nextInputLine() 
  // 	Returns false if there is no next input line,
  // 		true if there is a next input line
  // 		ALSO: exit(1) if input line overflow buffers.
  // 	If true, the next input line is read into "inputLine"
  // 		and it is uncommented.  Furthermore,
  // 		"inputLine" is copied into lineBuffer as a c-string.
  // 	CAREFUL: the uncommented line might have NO tokens!
  // 		This method has no loops, reads only one input line!
  //////////////////////////////////////////////////
  bool nextInputLine(){
	std::getline (ifile, inputLine);	// get next line from file
	if (inputLine.size()==0)
	    return false;			// input file is empty.
	trimComment(inputLine);		// remove comments

	if (inputLine.size()< BUFFERSIZE)
		strcpy (lineBuffer, inputLine.c_str());
	else{
	     std::cerr << "ERROR!  Buffer overflow!\n";
	     exit(1);
	}
	return true;	// Careful: this line might have no tokens
  }//nextInputLine

  // getFirstToken()
  //    Returns true iff when we find the first real-token of
  //       the current line.  (NOTE: real-token means non-NULL and not "\\")
  //       The rest of current line is in lineBuffer.
  //    If return false, it signals end of input file.
  //    REMARK: why not just call this "getNextToken"?  Because we need
  //    	a loop to keep reading input lines until there
  //    	is SOME token.  Establishing this existential question is
  //    	absolutely important.
  //////////////////////////////////////////////////
  bool getFirstToken(){
	while (nextInputLine()) {
		// Now, lineBuffer contains an uncommented line
		// But it might have no tokens!
	   tok = strtok (lineBuffer, " \t"); 	// whiteSpace is \t or space

	   if ((tok != NULL) && (!isContinue(tok)))
	           return true;	// found first non-\\ token!
	   // If we reached here, then tok==NULL or tok="\\"
	   // we need to read a new line!
	}//while
	return false;			// input file is empty
  }//getFirstToken

/* *************************************************** 
 * PUBLIC METHODS 
 *************************************************** */
  public:

  // cleanup -- close files and delete space
  //////////////////////////////////////////////////
  void cleanup() {
    delete [] lineBuffer;
    ifile.close();
    ofile.close();
  }

  //////////////////////////////////////////////////
  // nextOutputLine() is the MAIN method
  //    If return false, it signals end of input file.
  //    	Note that cleanup() is also done
  //    Else returns true and a non-empty sequence of tokens is stored
  //    	in "outputLine".  
  //    	We use lineBuffer (setup by getFirstToken) to
  //    	accumulate our tokens.
  //////////////////////////////////////////////////
  bool nextOutputLine(){
      	if (!getFirstToken()){
  	    cleanup();
	    return false;
	}
	outputLine.assign(tok); 	// Collect the first token

	tok = strtok (NULL, " \t"); 	// next token
	while (tok != NULL) {		// while not EOL
	    if (isContinue(tok)) {	// if \\-token, get another line
		if (!nextInputLine())
		    return true;	// if no more lines, we are DONE!
		tok = strtok (lineBuffer, " \t"); // else do next token again
					// and return to while loop
	    } else { // found next non-// token
	        outputLine.push_back(' ');	
	        outputLine.append(tok);	
	        tok = strtok (NULL, " \t"); // return to while loop
	    }
	}//while
	ofile << outputLine << std::endl;
  	return true;
  }//nextOutputLine

};//class fileReader

  //////////////////////////////////////////////////
  // string  getLabel(outputLine)
  //    	returns the first token if it is a label (i.e., 
  //    	ends with ':').
  //    	Else returns an empty string.
  // void    getRest(& outputLine)
  //    	modifies outputLine by removing the label (if any).
  //////////////////////////////////////////////////
  std::string getLabel(std::string oLine){
	char * ss = new char [oLine.size()+1];
  	strcpy (ss, oLine.c_str());	// we need to copy to ss because
  					// oLine.c_str() is const
	std::string tok = strtok(ss, " \t");
	if (tok.at(tok.size()-1) == ':')
		return tok;
	tok.clear();
	return tok;  //return empty string
  }//getLabel
  // The code for getRest() could be merged with getLabel() for efficiency!
  void getRest(std::string & oLine){
	char * ss = new char [oLine.size()+1];
  	strcpy (ss, oLine.c_str());	// we need to copy to ss because
  					// oLine.c_str() is const
	std::string tok = strtok(ss, " \t");
	if (tok.at(tok.size()-1) == ':') {
		oLine.erase(0, tok.size()+1);	//+1 because of space separator
	}
  }//getRest
  //////////////////////////////////////////////////
  // Creates a sample file for testing (default: "example.txt")
  // NOTE:
  // 	this function must be OUTSIDE the fileReader Class
  //////////////////////////////////////////////////
  void sampleFile(std::string egName="example.txt"){
      std::ofstream egFile;
      egFile.open (egName.c_str());
      egFile << "  # 0. This is a comment !!!   \n";
      egFile << "1. This should be the first outputLine from example.txt.   \n";
      egFile << "   2.  This is the second line # comment \n";
      egFile << "   		3. A line to be continued \\\n";
      egFile << " 4. into the next \\   \t \n";
      egFile << "5.   and again ! \n";
      egFile << "  # 5.5  This is a comment \n";
      egFile << " 	6. But not this line \n";
      egFile << "7.   This line is also continued \\ blah blah blab \n";
      egFile << "8. into the next line\n";
      egFile << "    9:     This should be the last line! \n";
      egFile << "   # 10. This one should not appear in the clean version \n";
      egFile << "   # 11. Nor this!\n";
      egFile.close();
  }// sample

  //////////////////////////////////////////////////
  // self-validation
  // NOTE:
  // 	this function must be OUTSIDE the fileReader Class
  //////////////////////////////////////////////////
  void validate(std::string fname="__tmp_fileReader.txt"){

    std::string s1="1. This should be the first outputLine from example.txt.";
    std::string s2="2. This is the second line";
    std::string s3="3. A line to be continued 4. into the next 5. and again !";
    std::string s4="6. But not this line";
    std::string s5="7. This line is also continued 8. into the next line";
    std::string s6="9: This should be the last line!";

    std::ifstream ifile;
    ifile.open (fname.c_str());
    std::string inputLine;
    std::getline(ifile, inputLine);	// ignore the first 2 lines 
    std::getline(ifile, inputLine);

    bool error = false;
    std::getline(ifile, inputLine);
    	if (inputLine.compare(s1) != 0) error=true;
    std::getline(ifile, inputLine);
    	if (inputLine.compare(s2) != 0) error=true;
    std::getline(ifile, inputLine);
    	if (inputLine.compare(s3) != 0) error=true;
    std::getline(ifile, inputLine);
    	if (inputLine.compare(s4) != 0) error=true;
    std::getline(ifile, inputLine);
    	if (inputLine.compare(s5) != 0) error=true;
    std::getline(ifile, inputLine);
    	if (inputLine.compare(s6) != 0) error=true;
    if (error)
    	std::cout << "ERROR: There are wrong lines!!\n";
    else
    	std::cout << "CORRECT: validated output!!\n";

    //Testing: getLabel() and getRest()
    std::string ss1="label: blah blah ...";
    std::string ss2="labe:l blah blah ...";
    std::string ss3="label blah blah ...";
	std::cout << "================> Testing getLabel(), getRest()\n";
	std::cout << "Line       = "  <<  ss1 << std::endl;
	    std::cout << "   getLabel= " << getLabel(ss1) << std::endl;
	    getRest(ss1);
	    std::cout << "   getRest = " << ss1 << std::endl;
	std::cout << "Line       = "  <<  ss2 << std::endl;
	    std::cout << "   getLabel= " << getLabel(ss2) << std::endl;
	    getRest(ss2);
	    std::cout << "   getRest = " << ss2 << std::endl;
	std::cout << "Line       = "  <<  ss3 << std::endl;
	    std::cout << "   getLabel= " << getLabel(ss3) << std::endl;
	    getRest(ss3);
	    std::cout << "   getRest = " << ss3 << std::endl;

    ifile.close();
  }// sample

#endif /* CORE_FILE_READER_H */
