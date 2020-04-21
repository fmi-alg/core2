/* viewer.cpp*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<GL/glut.h>
#include	<math.h>

struct Vertex{
	double x;
	double y;
};

//typedef Vertex* VPointer;

struct Edge{
	int	i1;
	int	i2;
};

//typedef Edge* EPointer;

int	vnum,cenum;
Vertex* varray;
Edge* cearray;

double	minx,miny,maxx,maxy;
double	ratio;

const	char* noff_keyword="NOFF";

void	reportError(char*	str,int line,char* file)
{
	fprintf(stderr,"Error [%s:%d] %s\n",file,line,str);
	exit(-1);
}

void	reportWarning(char* str,int line,char* file)
{
	fprintf(stderr,"Warning [%s:%d] %s\n",file,line,str);
}

void	readToLineEnd(FILE * fp)
{
	int	c;
	c=fgetc(fp);
	while	(c!='\n')
		c=fgetc(fp);
}

// omit the comment in fp
void	omitComment(FILE* fp)
{
	/*
	int c;
	//omit comment
	c=fgetc(fp);
	printf("%c\n",c);
	// in case the first character of line is '#'	
	// omit comment line
	while	(c=='#')
	{
		readToLineEnd(fp);
		c=fgetc(fp);
		printf("%c\n",c);
	}
	if	(fseek(fp,-1L,SEEK_CUR)!=0)
	{	
		fclose(fp);
		reportError("Seek file error!",__LINE__,__FILE__);	//put back one character
	}
	*/
}

int	import2OFFFile(char* sf)
{
	FILE * fp;
//	int		c;

	if
	((fp=fopen(sf,"r"))==NULL)
	{
		reportError("Cannot open this file!",__LINE__,__FILE__);
	}
	else
	{
		omitComment(fp);

		// read OFF, currently only support 'OFF'
		// file name length is maximum 256
		//fscanf(fp,"%s",&c);
		char	s[256];
		fscanf(fp,"%s",s);

		if	(strcmp(s,noff_keyword)!=0 )
		{
			reportError("Not valid format(*.noff)!",__LINE__,__FILE__);
		}

		readToLineEnd(fp);
		omitComment(fp);

		int	_d;
		fscanf(fp,"%d",&_d);
		if	(_d!=2)
		{
			reportError("Not valid dimension!",__LINE__,__FILE__);
		}
		readToLineEnd(fp);
		omitComment(fp);
		// read vertices count, faces count and edges count respectively
		int	f_num;
		// read num
		fscanf(fp,"%d",&vnum);
		if	(vnum<1)
		{
			reportError("Vertex number should above zero.",__LINE__,__FILE__);
		}
		// f_num is not checked
		fscanf(fp,"%d",&f_num);
		fscanf(fp,"%d",&cenum);

		readToLineEnd(fp);
		
		omitComment(fp);

		double	x;
		double	y;

		varray=new Vertex[vnum];
		//init 
		minx=miny=HUGE_VAL;
		maxx=maxy=-HUGE_VAL;

		// read information of vertices
		for (int i=0;i<vnum;i++)
		{
			
			fscanf(fp,"%lf%lf",&x,&y);

			maxx=(maxx>x?maxx:x);
			minx=(minx<x?minx:x);

			maxy=(maxy>y?maxy:y);
			miny=(miny<y?miny:y);

			varray[i].x=x;
			varray[i].y=y;

			readToLineEnd(fp);			
			
			omitComment(fp);
		}

		//read triangle
			int	side_num;
			for	(int	f_i=0;f_i<f_num;f_i++)
			{
				//read side num
				fscanf(fp,"%d",&side_num);
				int vi;
				for	(int v_i=0;v_i<side_num;v_i++)
				{
					fscanf(fp,"%d",&vi);
				}
					readToLineEnd(fp);			
					omitComment(fp);

			}

		cearray=new Edge[cenum];

		//read constrained edge
		for (int j=0;j<cenum;j++)
		{
			int	i1,i2;

			fscanf(fp,"%d%d",&i1,&i2);
	
			if	(i1<0 || i1>=vnum || i2<0 || i2>=vnum)
				reportError("Incorrect input of edge's endpoint's index. It should not below 0 or large or equal to vertex total count.",__LINE__,__FILE__);		

			cearray[j].i1=i1;
			cearray[j].i2=i2;

			readToLineEnd(fp);
			omitComment(fp);
		
		}	

		fclose(fp);	
	}
	return(1);
}
void	display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	gluOrtho2D(minx,maxx,miny,maxy);

	//display

	Vertex	v1,v2;
	glBegin(GL_LINES);
	glLineWidth(2.0);
	glColor3f(.0,1.0,1.0);
	for	(int e_i=0;e_i<cenum;e_i++)
	{
		v1=varray[cearray[e_i].i1];
		v2=varray[cearray[e_i].i2];
		glVertex2d(v1.x,v1.y);
		glVertex2d(v2.x,v2.y);
	}
	glEnd();

	glBegin(GL_POINTS);
	glPointSize(5.0);
	glColor3f(1.0,.0,.0);
	for	(int i=0;i<vnum;i++)
		glVertex2d(varray[i].x,varray[i].y);
	glEnd();

	glFlush();
}

void	reshape(int w,int h)
{
//		glViewport(0,-hh+h/2,(GLsizei)w,hh+h/2);
		if	(ratio<h/w)
		{
			GLsizei hh=(int)w*ratio/2;
			glViewport(0, h/2-hh, w, h/2+hh);               /* Set Viewport */
		}
		else
		{
			GLsizei ww=(int)h/ratio/2;
			glViewport(w/2-ww, 0, w/2+ww, h);               /* Set Viewport */
		}

}

void	init(void)
{
	glClearColor(1.0,1.0,1.0,1.0);
	glShadeModel(GL_FLAT);

	double cx,cy;
	cx=(maxx+minx)/2;
	cy=(maxy+miny)/2;

	double s=1.2;
	maxx=cx+(maxx-cx)*s;
	minx=cx+(minx-cx)*s;
	maxy=cy+(maxy-cy)*s;
	miny=cy+(miny-cy)*s;
}

int main(int argc,char* argv[])
{
	if	(argc!=2)
	{
		fprintf(stderr,"%s\n","Usage: view inputfilename");
		fprintf(stderr,"%s\n","Input file should be an nOFF file where n=2");
		exit(-1);
	}

	import2OFFFile(argv[1]);
	ratio=(maxy-miny)/(maxx-minx);

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(250,250);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[1]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
