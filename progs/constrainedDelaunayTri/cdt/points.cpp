/* randomly generate points*/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

char outputfile[256]="result.noff";
char	noff_keyword[]="NOFF";
void	generatePoints(char* f,int vnum)
{
	char delimiter=' ';
	FILE* fp;
	char* df=outputfile;

	if(f!=NULL)
	{
		df=f;
	}

	if
	((fp=fopen(df,"w"))==NULL)
	{
		printf("Cannot create or overwrite this file!");
		exit(-1);
	}
	else
	{
		// add a mark in the edge: writed
#ifdef	COMMENT_ON
		fprintf(fp,"# generated points randomly : %s \n",sf);
#endif
		fprintf(fp,"%s\n",noff_keyword);
		fprintf(fp,"%d\n",2);

//		srand(1);
		int	v_num=vnum;
		int	e_num=0;
		//output vertex count
#ifdef	COMMENT_ON
		fprintf(fp,"%s\n","# vertices num, face num=0, edge num");
#endif
		fprintf(fp,"%d%c%d%c%d\n",v_num,delimiter,0,delimiter,e_num);

		srand(v_num*71);
//first randomly omit some random number, (still not random)
/*		int omit_num=rand();
		for (int oi=0;oi<omit_num;oi++)
		{
			rand();
		}
*/
		int	base=rand();
		if	(base==0)	base=237;	// a prime

		double	x,y;
		//output vertex information in form of x, y value
		for (int i=0;i<v_num;i++)
		{
#ifdef	COMMENT_ON
			fprintf(fp,"%s %d\n","# vertex index: ",i);
#endif
			x=rand()/base;
			y=rand()/base;
			fprintf(fp,"%f %f\n",x,y);
		}
		
		fclose(fp);	
	}
}

int	main(int argc,char**argv)
{
	if	(argc<4)
	{
		unsigned int	vnum=atoi(argv[1]);
		generatePoints(argv[2],vnum);
	}
	else
	{
		printf("Use: ./points N [outputfile]. (Generate N random points in the plane)\n");
		printf("If no outputfile name is given, will output to %s\n",outputfile);
	}
	return 0;
}