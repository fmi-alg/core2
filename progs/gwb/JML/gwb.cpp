/* file: gwb.cpp
 *
 * This is our implementation of the "Geometric Workbench" (GWB)
 * 	from Marrti Mantyla.
 *
 * Author:	Kai Cao (Supervised by Professor Yap, July 2012)
 * 		Oct 2013: improvements by Jyh-Ming Lien 
 * Since Core 2.1.
 * 
 ***************************************************/


/***************************************************
 * include files:
 ***************************************************/
#include "gwb_gui.h"
#include "Euler_Ops.h"
#include <cassert>

Euler_Ops *eo;

/***************************************************
 * CONSTANTS:
 ***************************************************/
#define SQRTWO sqrt(2)
#define SQRTFIVE sqrt(5)
#define PHI ((1+SQRTFIVE)/2)
#define SINGLE_SURFACE

/**************************************************
* PARAMETERS:
**************************************************/
int interactive=0;                  // interactive or not:
                                    // <=0 means non-interactive, >0 means interactive.

//**************************************************
// MAIN:
//*************************************************

int main(int argc,char **argv)
{
	
	/*The Euler_Ops Object*/
	eo=new Euler_Ops();
	assert(eo);

	if (argc > 1) interactive = atoi(argv[1]);	// Interactive (0) or no (>0)
	if (argc > 2) inputDir  = argv[2];		      // path for input files
	if (argc > 3) fileName = argv[3]; 		      // Input file name

	if (interactive == 0) 
	{
		   
		if (argc > 4) windowWidth = atof(argv[4]);	// windowWidth
		if (argc > 5) windowHeight= atof(argv[5]);	// windowHt
		if (argc > 6) windowPosX = atoi(argv[6]);	// window X pos
		if (argc > 7) windowPosY = atoi(argv[7]);	// window Y pos
		if (argc > 8) mode=argv[8];			// mode = face or wire
		if (argc > 9) model=argv[9];			// model = cube, trefoil, etc, etc
		/* model parameters */
		if (argc > 10) radius = atof(argv[10]);
		if (argc > 11) height = atof(argv[11]);
		if (argc > 12) nsegments = atoi(argv[12]);
		if (argc > 13) nsections = atoi(argv[13]);
		if (argc > 14) height2 = atof(argv[14]);
		if (argc > 15) radius2 = atof(argv[15]);
		/*Rotation Size*/
		if (argc > 16) rotUnitX=atoi(argv[16]);
		if (argc > 17) rotUnitY=atoi(argv[17]);
		if (argc > 18) rotUnitZ=atoi(argv[18]);
		/*model parameters*/
		if (argc > 19) nsegments2=atoi(argv[19]);
		if (argc > 20) nsections2=atoi(argv[20]);
		if (argc > 21) nparts=atoi(argv[21]);
	 	if (argc > 22) nparts2=atoi(argv[22]);
		if (argc > 23) position2=atoi(argv[23]);
		if (argc > 24) position3=atoi(argv[24]);
	 	if (argc > 25) thickness3=atoi(argv[25]);
		if (argc > 26) height3=atoi(argv[26]);
		//if (argc > 27) lightRate=atof(argv[27]); //removed
		if (argc > 28) initRotDeg=atof(argv[28]);
		if (argc > 29) initRotAxisX=atof(argv[29]);
		if (argc > 30) initRotAxisY=atof(argv[30]);
		if (argc > 31) initRotAxisZ=atof(argv[31]);


		eo->prim(model);
	}//interactive=0


	/*Read from file*/  
	string fileAdd=inputDir+"/"+fileName;
	  
	ifstream ifile;
	ifile.open(fileAdd.c_str());
	  
	/*Variable Maps*/
	map<string, double> variables;
	  
	/*Read every line*/
	string line;

	/*If we Read from file*/
	if (interactive==2)
	{
		/*The model name*/
		model=fileName;
		if (argc > 4) initRotDeg=atof(argv[4]);
		if (argc > 5) initRotAxisX=atof(argv[5]);
		if (argc > 6) initRotAxisY=atof(argv[6]);
		if (argc > 7) initRotAxisZ=atof(argv[7]);
		
		/*Display direction*/
		
	    while (ifile.good())
	    {
		    
		    /*Read a line*/
	        getline(ifile,line);
		    
		    /*If it is an empty line,go ahead*/
	        if(line.length()<=0)
				continue;

	        /*If the line begins with '#' it must be a comment*/
	        if(line[0]=='#')
	        	continue;

		    int eqPos=line.find('=');
		    if (eqPos<line.size()&&eqPos>=0)
		    {
				/*Read variables*/
	            string first=line.substr(0,eqPos);
				istringstream iss2(first);
				string var;
				iss2>>var;
				//cout<<var<<endl;

				string second=line.substr(eqPos+1);
				istringstream iss3(second);
				double num;
				iss3>>num;
				
				variables[var]=num;
	   		}//variables
		
		    else
		    {
	            /*Analyze the line*/
	            string command;
	            Vec<double> *ids= new Vec<double>();
		    
	            /*Read first word*/
	            string first;
	            istringstream iss(line);
	            iss>>first;

	            double fileScalar=variables["scalar"]==0?1:variables["scalar"];
	            
	            /*Then the first word must be command*/
	            command=first;
	            while(iss)
	            {
					 string var;
					 /*Push the number*/
		             double num;
		             iss>>var;
					 /*Parse the variable*/
					 int varPos=var.find('$');
					 if (varPos<var.size()&&varPos>=0)
					 {
						string tempVar;          // string which will contain the result

						ostringstream convert;   // stream used for the conversion

						convert <<variables[var.substr(varPos+1)];      // insert the constant value

						tempVar = convert.str(); // set 'Result' to the contents of the stream
						num=atof((var.substr(0,varPos)+tempVar).c_str());
					 }
					 else 
						num=atof(var.c_str());
				 
	                 ids->push_back(num);
	            }//while(iss)

	            /*We need to erase the last one*/
	            ids->erase(ids->end()-1);

	            /*Different commands*/
	            if (command.compare("mvfs")==0)
	            {		  
		         	(*ids)[ids->size()-1]*=fileScalar;
			 	 	(*ids)[ids->size()-2]*=fileScalar;
			  		(*ids)[ids->size()-3]*=fileScalar;
	                eo->mvfs(ids);
		    	}
	            else if(command.compare("kvfs")==0)
	            {
	                eo->kvfs(ids);
	            }
	            else if(command.compare("mev")==0)
	            {
			        (*ids)[ids->size()-1]*=fileScalar;
			        (*ids)[ids->size()-2]*=fileScalar;
			        (*ids)[ids->size()-3]*=fileScalar;
	                 eo->mev(ids);
		    	}
	            else if(command.compare("kev")==0)
	                  eo->kev(ids);
	            else if(command.compare("mef")==0)
	                  eo->mef(ids);
	            else if(command.compare("kef")==0)
	                  eo->kef(ids);
	            else if(command.compare("kemr")==0)
	                  eo->kemr(ids);
	            else if(command.compare("mekr")==0)
	                  eo->mekr(ids);
	            else if(command.compare("kfmrh")==0)
	                  eo->kfmrh(ids);
	            else if(command.compare("mfkrh")==0)
	                  eo->mfkrh(ids);
	            else if(command.compare("ringmv")==0)
	                  eo->ringmv(ids);
	            else {
	                  cout<<command<<" is not a valid command."<<endl;
	            }
			}//command

			 }//while (ifile.good())
	}//if read file
	
	//create GUI
	createGUI(argc,argv);
	
}//main


//**************************************************
//* END
//**************************************************
