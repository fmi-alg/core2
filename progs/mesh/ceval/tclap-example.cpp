/* **************************************************
 * 
 * From
 * 	http://tclap.sourceforge.net/manual.html
 * 
 * Preliminary:
 * 	CLAP stands for "Command Line Argument Processing"
 * 	TCLAP by Michael Smoot is the templated version of an earlier CLAP library.
 * 
 * Overview:
 * The main class in TCLAP is is the CmdLine (command line) class.
 * This class parses the command line passed to it according to the arguments
 * that it contains. 
 * 
 * Arguments are separate objects that are added to the CmdLine object one at a time.
 * The six argument classes are:
 *
 * 	ValueArg, UnlabeledValueArg, SwitchArg,
 * 	MultiSwitchArg, MultiArg and UnlabeledMultiArg.
 *
 * These classes are templatized, which means they can be defined to parse a value of
 * any type. Once you add the arguments to the CmdLine object,
 * it parses the command line and assigns the data it finds to
 * the specific argument objects it contains.
 * 
 * Your program accesses the values parsed by calls to the getValue()
 * methods of the argument objects.
 * 
 * Example:
 * 	The following program takes a name argument (-n NAME)
 * 	and prints back the NAME.  If the switch (-r) is given,
 * 	the name is printed in reverse, EMAN.
 * 
 * 	% test1 -n mike
 * 	My name is: mike
 * 
 * 	% test1 -n mike -r
 * 	My name (spelled backwards) is: ekim
 * 
 *	% test1 -r -n mike
 *	My name (spelled backwards) is: ekim
 * 
 *	% test1 -r
 *	PARSE ERROR:
 *              One or more required arguments missing!
 * 
 *	Brief USAGE:
 *	   test1  [-r] -n <string> [--] [-v] [-h]
 * 
 *	For complete USAGE and HELP type:
 * 	  test1 --help
 * 
 *	% test1 --help
 * 
 *	USAGE:
 * 
 *	   test1  [-r] -n <string> [--] [-v] [-h]
 * 
 *	 Where:
 *	 
 *	    -r,  --reverse
 *	      Print name backwards
 *	 
 *	    -n <string>  --name <string>
 *	      (required)  (value required)  Name to print
 *	 
 *	    --,  --ignore_rest
 *	      Ignores the rest of the labeled arguments following this flag.
 *	 
 *	    -v,  --version
 *	      Displays version information and exits.
 *	 
 *	    -h,  --help
 *	      Displays usage information and exits.
 *	 
 *	    Command description message
 * 
 * **************************************************/

#include <string>
#include <iostream>
#include <algorithm>
#include <tclap/CmdLine.h>

int main(int argc, char** argv)
{
	// Wrap everything in a try block.  Do this every time, 
	// because exceptions will be thrown for problems.
	try {  

	// Define the command line object, and insert a message
	// that describes the program. The "Command description message" 
	// is printed last in the help text. The second argument is the 
	// delimiter (usually space) and the last one is the version number. 
	// The CmdLine object parses the argv array based on the Arg objects
	// that it contains. 

	TCLAP::CmdLine cmd("Command description message", ' ', "0.9");

	// Define a value argument and add it to the command line.
	// A value arg defines a flag and a type of value that it expects,
	// such as "-n Bishop".

	TCLAP::ValueArg<std::string> nameArg("n","name","Name to print",true,"homer","string");

	// Add the argument nameArg to the CmdLine object. The CmdLine object
	// uses this Arg to parse the command line.

	cmd.add( nameArg );

	// Define a switch and add it to the command line.
	// A switch arg is a boolean argument and only defines a flag that
	// indicates true or false.  In this example the SwitchArg adds itself
	// to the CmdLine object as part of the constructor.  This eliminates
	// the need to call the cmd.add() method.  All args have support in
	// their constructors to add themselves directly to the CmdLine object.
	// It doesn't matter which idiom you choose, they accomplish the same thing.

	TCLAP::SwitchArg reverseSwitch("r","reverse","Print name backwards", cmd, false);

	// Parse the argv array.
	cmd.parse( argc, argv );

	// Get the value parsed by each arg. 

	std::string name = nameArg.getValue();
	bool reverseName = reverseSwitch.getValue();

	// Do what you intend. 
	if ( reverseName )
	{
		std::reverse(name.begin(),name.end());
		std::cout << "My name (spelled backwards) is: " << name << std::endl;
	}
	else
		std::cout << "My name is: " << name << std::endl;


	} catch (TCLAP::ArgException &e)  // catch any exceptions
	{ std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }
}

/* **************************************************
 * **************************************************/
