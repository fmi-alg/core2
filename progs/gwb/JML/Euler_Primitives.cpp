#include "Euler_Ops.h"
using namespace std;

/* Chee: new parameters */
extern double radius;		  
extern double radius2;		 
extern double height;		
extern double height2;	
extern int nsegments;
extern int nsections;
extern int nsegments2;
extern int nsections2;
extern int nparts;
extern int nparts2;
extern double position2;
extern double position3;
extern double thickness3;
extern double height3;

/*Primitives*/
Solid* Euler_Ops::prim(string name){
	return prim(name,1);
}

Solid * Euler_Ops::hemisphere(int solidNum)
{
	Solid* solid;
	double rad=radius;
	int hor=nsegments;
	int ver=nsections;
	/*Build the first two*/
	solid=mvfs(solidNum,1,1,rad,0,0);

	double horr=PI*2/hor;
	
	/*Build the polygon*/
	for (int i=2;i<=hor;i++){
		int prev=i-2>0?i-2:i-1;
		mev(solidNum,1,1,i-1,prev,prev,i,rad*cos(horr*(i-1)),rad*sin(horr*(i-1)),0);
	}

	/*face 1 up, face 2 bottom*/
	mef(solidNum,1,1,2,hor,hor-1,2);
	
	/*Build the upper hemisphere*/
	for (int i=1;i<ver;i++)
	{
		int base=i*hor;
		/*compute the layers*/
		double xyr=rad*cos(PI/2/ver*i);
		double z=rad*sin(PI/2/ver*i);
		
		/*Make all verts on this layer*/
		/*Make the first vertex*/
		mev(solidNum,1,1,(i-1)*hor+1,i*hor,i*hor,i*hor+1,xyr,0,z);
		/*Make other vertices*/
		for(int j=2;j<=hor;j++){
			int prev=(i-1)*hor+j;
			
			double x=xyr*cos(PI*2/hor*(j-1));
			double y=xyr*sin(PI*2/hor*(j-1));
			mev(solidNum,1,1,prev,prev-1,prev-1,prev+hor,x,y,z);
		}//for theta
		
		/*Make all edges*/
		/*Make the first face*/
		mef(solidNum,1,base+1,base+1-hor,base+hor,base,2+(i-1)*hor+1);
		mef(solidNum,1,base+2,base+2-hor,base+1,base+hor,2+(i-1)*hor+2);
		/*Make other vertices*/
		for(int j=3;j<=hor;j++){
			Id v1=base+j;
			Id v2=v1-hor;
			Id v3=v1-1;
			Id v4=v3-1;
			Id f2=2+(i-1)*hor+j;
			mef(solidNum,1,v1,v2,v3,v4,f2);
		}//for theta

	}//for layers
	
	/*Build the final vertex*/
	int top=hor*ver+1;
	mev(solidNum,1,1,hor*ver,hor*ver-1,hor*ver-1,hor*ver+1,0,0,rad);
	mef(solidNum,1,hor*(ver-1)+1,hor*ver,top,hor*ver,2+hor*(ver-1)+1);

	for (int i=2;i<hor;i++){
		int now=hor*(ver-1)+i;
		int prev=now-1;
		mef(solidNum,1,now,prev,top,hor*ver,2+hor*(ver-1)+i);
	}

	/*build layers*/

	return solid;
}

Solid * Euler_Ops::sphere(int solidNum)
{
	Solid* solid;
	double rad=radius;
	int hor=nsegments;
	int ver=nsections;
	/*Build the first two*/
	
	double horr=PI*2/hor;
	//double verr=PI/ver;
	
	double smallRad=rad*sin(PI/ver);
	solid=mvfs(solidNum,1,1,smallRad,0,-rad*cos(PI/ver));
	/*Build the polygon*/
	for (int i=2;i<=hor;i++){
		int prev=i-2>0?i-2:i-1;
		mev(solidNum,1,1,i-1,prev,prev,i,smallRad*cos(horr*(i-1)),smallRad*sin(horr*(i-1)),-rad*cos(PI/ver));
	}
	/*face 1 up, face 2 bottom*/
	mef(solidNum,1,1,2,hor,hor-1,2);
	
	/*Build the upper hemisphere*/
	for (int i=1;i<ver-1;i++){
		int base=i*hor;
		/*compute the layers*/
		double xyr=rad*sin(PI/ver*(i+1));
		double z=rad*sin(PI/ver*(i+1)-PI/2);
		
		/*Make all verts on this layer*/
		/*Make the first vertex*/
		mev(solidNum,1,1,(i-1)*hor+1,i*hor,i*hor,i*hor+1,xyr,0,z);
		/*Make other vertices*/
		for(int j=2;j<=hor;j++){
			int prev=(i-1)*hor+j;
			
			double x=xyr*cos(PI*2/hor*(j-1));
			double y=xyr*sin(PI*2/hor*(j-1));
			mev(solidNum,1,1,prev,prev-1,prev-1,prev+hor,x,y,z);
		}//for theta
		
		/*Make all edges*/
		/*Make the first face*/
		mef(solidNum,1,base+1,base+1-hor,base+hor,base,2+(i-1)*hor+1);
		mef(solidNum,1,base+2,base+2-hor,base+1,base+hor,2+(i-1)*hor+2);
		/*Make other vertices*/
		for(int j=3;j<=hor;j++){
			Id v1=base+j;
			Id v2=v1-hor;
			Id v3=v1-1;
			Id v4=v3-1;
			Id f2=2+(i-1)*hor+j;
			mef(solidNum,1,v1,v2,v3,v4,f2);
		}//for theta


	}//for layers
	
	/*Build the final vertex*/
	ver--;
	int top=hor*ver+1;
	mev(solidNum,1,1,hor*ver,hor*ver-1,hor*ver-1,hor*ver+1,0,0,rad);
	mef(solidNum,1,hor*(ver-1)+1,hor*ver,top,hor*ver,2+hor*(ver-1)+1);
	for (int i=2;i<hor;i++){
		int now=hor*(ver-1)+i;
		int prev=now-1;
		mef(solidNum,1,now,prev,top,hor*ver,2+hor*(ver-1)+i);
	}//for n-1 top faces
	ver++;
	int bot=top+1;
	mev(solidNum,2,2,hor,1,1,bot,0,0,-rad);
	mef(solidNum,2,bot,hor,1,2,1+hor*(ver-1)+1);
	for (int i=2;i<hor;i++){
		mef(solidNum,2,bot,i-1,i,i+1,1+hor*(ver-1)+i);
	}//for n-1 bottom faces

	/*build layers*/	
	return solid;
}


Solid * Euler_Ops::cylinder(int solidNum)
{
	Solid* solid;

	double rad=radius;
	//double height=size[1];
	int hor=nsegments;
	/*Build the first two*/
	solid=mvfs(1,1,1,rad,0,0);
	double horr=PI*2/hor;
	//double verr=PI/ver;
	
	/*Build the polygon*/
	for (int i=2;i<=hor;i++){
		int prev=i-2>0?i-2:i-1;
		mev(solidNum,1,1,i-1,prev,prev,i,rad*cos(horr*(i-1)),rad*sin(horr*(i-1)),0);
	}
	/*face 1 up, face 2 bottom*/
	mef(solidNum,1,1,2,hor,hor-1,2);
	
	/*Build the verts and faces*/
	for (int i=1;i<=nsections;i++){
		int base=i*hor;
		/*compute the layers*/
		double xyr=rad;
		double z=height/nsections*i;
		
		/*Make all verts on this layer*/
		/*Make the first vertex*/
		mev(solidNum,1,1,(i-1)*hor+1,i*hor,i*hor,i*hor+1,xyr,0,z);
		/*Make other vertices*/
		for(int j=2;j<=hor;j++){
			int prev=(i-1)*hor+j;
			
			double x=xyr*cos(PI*2/hor*(j-1));
			double y=xyr*sin(PI*2/hor*(j-1));
			mev(solidNum,1,1,prev,prev-1,prev-1,prev+hor,x,y,z);
		}//for theta
		
		/*Make all edges*/
		/*Make the first face*/
		mef(solidNum,1,base+1,base+1-hor,base+hor,base,2+(i-1)*hor+1);
		mef(solidNum,1,base+2,base+2-hor,base+1,base+hor,2+(i-1)*hor+2);
		/*Make other vertices*/
		for(int j=3;j<=hor;j++){
			Id v1=base+j;
			Id v2=v1-hor;
			Id v3=v1-1;
			Id v4=v3-1;
			Id f2=2+(i-1)*hor+j;
			mef(solidNum,1,v1,v2,v3,v4,f2);
		}//for theta


	}//for layers
	return solid;
}

Solid * Euler_Ops::cone(int solidNum)
{
	Solid *solid;

	/*Get basic parameters*/
	double rad=radius;

	int hor=nsegments;
	int ver=nsections;

	/*Build the first two*/
	solid=mvfs(solidNum,1,1,rad,0,0);
	double horr=PI*2/hor;
	//double verr=PI/ver;
	
	/*Build the polygon*/
	for (int i=2;i<=hor;i++){
		int prev=i-2>0?i-2:i-1;
		mev(solidNum,1,1,i-1,prev,prev,i,rad*cos(horr*(i-1)),rad*sin(horr*(i-1)),0);
	}
	/*face 1 up, face 2 bottom*/
	mef(solidNum,1,1,2,hor,hor-1,2);
	
	/*Build the upper hemisphere*/
	for (int i=1;i<ver;i++){
		int base=i*hor;
		/*compute the layers*/
		double xyr=rad/(ver+0.0)*(ver-i);
		double z=height/(ver+0.0)*i;
		
		/*Make all verts on this layer*/
		/*Make the first vertex*/
		mev(solidNum,1,1,(i-1)*hor+1,i*hor,i*hor,i*hor+1,xyr,0,z);
		/*Make other vertices*/
		for(int j=2;j<=hor;j++){
			int prev=(i-1)*hor+j;
			
			double x=xyr*cos(PI*2/hor*(j-1));
			double y=xyr*sin(PI*2/hor*(j-1));
			mev(solidNum,1,1,prev,prev-1,prev-1,prev+hor,x,y,z);
		}//for theta
		
		/*Make all edges*/
		/*Make the first face*/
		mef(solidNum,1,base+1,base+1-hor,base+hor,base,2+(i-1)*hor+1);
		mef(solidNum,1,base+2,base+2-hor,base+1,base+hor,2+(i-1)*hor+2);
		/*Make other vertices*/
		for(int j=3;j<=hor;j++){
			Id v1=base+j;
			Id v2=v1-hor;
			Id v3=v1-1;
			Id v4=v3-1;
			Id f2=2+(i-1)*hor+j;
			mef(solidNum,1,v1,v2,v3,v4,f2);
		}//for theta


	}//for layers
	
	/*Build the final vertex*/
	int top=hor*ver+1;
	mev(solidNum,1,1,hor*ver,hor*ver-1,hor*ver-1,hor*ver+1,0,0,height);
	mef(solidNum,1,hor*(ver-1)+1,hor*ver,top,hor*ver,2+hor*(ver-1)+1);
	for (int i=2;i<hor;i++){
		int now=hor*(ver-1)+i;
		int prev=now-1;
		mef(solidNum,1,now,prev,top,hor*ver,2+hor*(ver-1)+i);
	}


	/*build layers*/	

	return solid;
}

Solid * Euler_Ops::trefoil(int solidNum)
{
	Solid * solid;

	double rad1=radius;
	double rad2=radius2;
	
	int hor=nsegments;
	int ver=nsections;

	/*if (rad1<=rad2){
	    cout << "rad1 must be greater than rad2" << endl;
	} else {
	Build the first Point*/
	/*The first center is (0,-rad1,0)
	  The direction is (5,0,-3);
          The first vec is (0,rad2,0);
	  So the first point is (0,rad2-rad1,0)
	*/
	
	solid=mvfs(solidNum,1,1,0,rad2-rad1 ,0);
	
	/*Unit angle*/
	double verr=PI*2/ver;
	double horr=PI*2/hor;
	
	/*The first vector*/
	double firstV[]={0,rad2,0};
	/*The first direction*/
	double firstDir[]={5,0,-3};
	/*The first center*/
	double firstCen[]={0,-rad1,0};
	/*Build the polygon*/
	for (int i=2;i<=ver;i++){
		int prev=i-2>0?i-2:i-1;
		
		/*Rotating theta*/
		double theta=verr*(i-1);
		//cout<<"good before rotate"<<endl;
		double iV[]={1,20,30};
		rotate(firstDir,firstV,theta,iV);
		//cout<<"good after rotate"<<endl;
		/*cout<<iV[0]<<endl;
		cout<<iV[1]<<endl;
		cout<<iV[2]<<endl;*/

		mev(solidNum,1,1,i-1,prev,prev,i,firstCen[0]+iV[0],firstCen[1]+iV[1],firstCen[2]+iV[2]);
		//cout<<"good after mev"<<endl;
	}
	/*face 1 up, face 2 bottom*/
	mef(solidNum,1,1,2,ver,ver-1,2);
	
	
	
	/*Build the upper hemisphere*/
	for (int i=1;i<hor;i++){
		
		/*The first direction*/
		double iDir[]={dirX(i*horr),dirY(i*horr),dirZ(i*horr)};
		/*The first center*/
		double iCen[]={rad1*modelX(i*horr),rad1*modelY(i*horr),rad1*modelZ(i*horr)};
		/*The first vector*/
		double iFirstV[]={-iDir[1],iDir[0],0};
		/*normIFistV*/
		double iNormFirstV[]={1,2,3};
		norm(iFirstV,iNormFirstV,rad2);
		int base=i*ver;
		
		/*Make all horts on this layer*/
		/*Make the first hortex*/
		mev(solidNum,1,1,(i-1)*ver+1,i*ver,i*ver,i*ver+1,iCen[0]+iNormFirstV[0],iCen[1]+iNormFirstV[1],iCen[2]+iNormFirstV[2]);
		/*Make other hortices*/
		for(int j=2;j<=ver;j++){
			
			int prev=(i-1)*ver+j;

			/*Rotating theta*/
			double theta=verr*(j-1);
			
			double jV[]={1,20,30};
			rotate(iDir,iNormFirstV,theta,jV);				
			
			mev(solidNum,1,1,prev,prev-1,prev-1,prev+ver,iCen[0]+jV[0],iCen[1]+jV[1],iCen[2]+jV[2]);
		}//for theta
		
		/*Make all edges*/
		/*Make the first face*/
		mef(solidNum,1,base+1,base+1-ver,base+ver,base,2+(i-1)*ver+1);
		mef(solidNum,1,base+2,base+2-ver,base+1,base+ver,2+(i-1)*ver+2);
		/*Make other hortices*/
		for(int j=3;j<=ver;j++){
			Id v1=base+j;
			Id v2=v1-ver;
			Id v3=v1-1;
			Id v4=v3-1;
			Id f2=2+(i-1)*ver+j;
			mef(solidNum,1,v1,v2,v3,v4,f2);
		}//for theta


	}//for layers
	/*Build the final hortex*/
	/*int top=ver*hor+1;
	mev(1,1,1,ver*hor,ver*hor-1,ver*hor-1,ver*hor+1,0,0,rad1);
	mef(1,1,ver*(hor-1)+1,ver*hor,top,ver*hor,2+ver*(hor-1)+1);
	for (int i=2;i<ver;i++){
		int now=ver*(hor-1)+i;
		int prev=now-1;
		mef(1,1,now,prev,top,ver*hor,2+ver*(hor-1)+i);
	}*/

	/*Add the face*/
	kfmrh(solidNum,2,1);
	mekr(solidNum,2,1,2,ver*(hor-1)+1,ver*hor);
	
  		/*Make the edge faces*/
	for (int i=2;i<=ver;i++){
	    mef(solidNum,2,ver*(hor-1)+i,ver*(hor-1)+i-1,i,i+1>ver?1:i+1,ver*(hor-1)+i+1);
	}

	//}//rad1>rad2

	return solid;
}

Solid * Euler_Ops::disk(int solidNum)
{
	Solid * solid;
	double rad=radius;
	//double height=size[1];
	int hor=nsegments;
	int ver=nsections;
	/*Build the first two*/
	solid=mvfs(solidNum,1,1,rad,0,0);
	double horr=PI*2/hor;
	//double verr=PI/ver;
	
	/*Build the polygon*/
	for (int i=2;i<=hor;i++){
		int prev=i-2>0?i-2:i-1;
		mev(solidNum,1,1,i-1,prev,prev,i,rad*cos(horr*(i-1)),rad*sin(horr*(i-1)),0);
	}
	/*face 1 up, face 2 bottom*/
	mef(solidNum,1,1,2,hor,hor-1,2);
	
	/*Build the upper hemisphere*/
	for (int i=1;i<=ver;i++){
		int base=i*hor;
		/*compute the layers*/
		double xyr=rad+height/2*sin(PI/ver*i);
		double z=height/ver*i;
		
		/*Make all verts on this layer*/
		/*Make the first vertex*/
		mev(solidNum,1,1,(i-1)*hor+1,i*hor,i*hor,i*hor+1,xyr,0,z);
		/*Make other vertices*/
		for(int j=2;j<=hor;j++){
			int prev=(i-1)*hor+j;
			
			double x=xyr*cos(PI*2/hor*(j-1));
			double y=xyr*sin(PI*2/hor*(j-1));
			mev(solidNum,1,1,prev,prev-1,prev-1,prev+hor,x,y,z);
		}//for theta
		
		/*Make all edges*/
		/*Make the first face*/
		mef(solidNum,1,base+1,base+1-hor,base+hor,base,2+(i-1)*hor+1);
		mef(solidNum,1,base+2,base+2-hor,base+1,base+hor,2+(i-1)*hor+2);
		/*Make other vertices*/
		for(int j=3;j<=hor;j++){
			Id v1=base+j;
			Id v2=v1-hor;
			Id v3=v1-1;
			Id v4=v3-1;
			Id f2=2+(i-1)*hor+j;
			mef(solidNum,1,v1,v2,v3,v4,f2);
		}//for theta


	}//for layers

	return solid;
	/*build layers*/	
}

Solid * Euler_Ops::torus(int solidNum)
{
	Solid * solid;

	double rad1=radius;
	double rad2=radius2;
	int hor=nsegments;
	int ver=nsections;
	if (rad1>rad2){
		/*Build the first two*/
	solid=mvfs(solidNum,1,1,rad1+rad2,0,0);
	double horr=PI*2/hor;
	//double verr=PI*2/ver;
	
	/*Build the polygon*/
	for (int i=2;i<=hor;i++){
		int prev=i-2>0?i-2:i-1;
		mev(solidNum,1,1,i-1,prev,prev,i,(rad1+rad2)*cos(horr*(i-1)),(rad1+rad2)*sin(horr*(i-1)),0);
	}
	/*face 1 up, face 2 bottom*/
	mef(solidNum,1,1,2,hor,hor-1,2);
	
	
	/*Build the upper hemisphere*/
	for (int i=1;i<ver;i++){
		int base=i*hor;
		/*compute the layers*/
		double xyr=rad1+rad2*cos(2.0*PI/ver*i);
		double z=rad2*sin(2.0*PI/ver*i);
		
		/*Make all verts on this layer*/
		/*Make the first vertex*/
		mev(solidNum,1,1,(i-1)*hor+1,i*hor,i*hor,i*hor+1,xyr,0,z);
		/*Make other vertices*/
		for(int j=2;j<=hor;j++){
			int prev=(i-1)*hor+j;
			
			double x=xyr*cos(PI*2/hor*(j-1));
			double y=xyr*sin(PI*2/hor*(j-1));
			mev(solidNum,1,1,prev,prev-1,prev-1,prev+hor,x,y,z);
		}//for theta
		
		/*Make all edges*/
		/*Make the first face*/
		mef(solidNum,1,base+1,base+1-hor,base+hor,base,2+(i-1)*hor+1);
		mef(solidNum,1,base+2,base+2-hor,base+1,base+hor,2+(i-1)*hor+2);
		/*Make other vertices*/
		for(int j=3;j<=hor;j++){
			Id v1=base+j;
			Id v2=v1-hor;
			Id v3=v1-1;
			Id v4=v3-1;
			Id f2=2+(i-1)*hor+j;
			mef(solidNum,1,v1,v2,v3,v4,f2);
		}//for theta


	}//for layers
	/*Build the final vertex*/
	/*int top=hor*ver+1;
	mev(1,1,1,hor*ver,hor*ver-1,hor*ver-1,hor*ver+1,0,0,rad1);
	mef(1,1,hor*(ver-1)+1,hor*ver,top,hor*ver,2+hor*(ver-1)+1);
	for (int i=2;i<hor;i++){
		int now=hor*(ver-1)+i;
		int prev=now-1;
		mef(1,1,now,prev,top,hor*ver,2+hor*(ver-1)+i);
	}*/

	/*Add the face*/
	kfmrh(solidNum,2,1);
	mekr(solidNum,2,1,2,hor*(ver-1)+1,hor*ver);
	
  		/*Make the edge faces*/
	for (int i=2;i<=hor;i++){
	    mef(solidNum,2,hor*(ver-1)+i,hor*(ver-1)+i-1,i,i+1>hor?1:i+1,hor*(ver-1)+i+1);
	}

	}//rad1>rad2

	else {
		double topZ=sqrt(rad2*rad2-rad1*rad1);
		double theta=PI-acos(rad1/rad2);

	/*Build the first two*/
	
	double horr=PI*2/hor;
	double verr=theta*2/ver;
	
	double smallRad=rad1+rad2*cos(theta-verr);
	double smallZ=-rad2*sin(theta-verr);
	solid=mvfs(solidNum,1,1,smallRad,0,smallZ);
	/*Build the polygon*/
	for (int i=2;i<=hor;i++){
		int prev=i-2>0?i-2:i-1;
		mev(solidNum,1,1,i-1,prev,prev,i,smallRad*cos(horr*(i-1)),smallRad*sin(horr*(i-1)),smallZ);
	}
	/*face 1 up, face 2 bottom*/
	mef(solidNum,1,1,2,hor,hor-1,2);
	
	/*Build the upper hemisphere*/
	for (int i=1;i<ver-1;i++){
		int base=i*hor;
		/*compute the layers*/
		double xyr=rad1+rad2*cos(theta-verr*(i+1));
		double z=-rad2*sin(theta-verr*(i+1));
		
		/*Make all verts on this layer*/
		/*Make the first vertex*/
		mev(solidNum,1,1,(i-1)*hor+1,i*hor,i*hor,i*hor+1,xyr,0,z);
		/*Make other vertices*/
		for(int j=2;j<=hor;j++){
			int prev=(i-1)*hor+j;
			
			double x=xyr*cos(PI*2/hor*(j-1));
			double y=xyr*sin(PI*2/hor*(j-1));
			mev(solidNum,1,1,prev,prev-1,prev-1,prev+hor,x,y,z);
		}//for theta
		
		/*Make all edges*/
		/*Make the first face*/
		mef(solidNum,1,base+1,base+1-hor,base+hor,base,2+(i-1)*hor+1);
		mef(solidNum,1,base+2,base+2-hor,base+1,base+hor,2+(i-1)*hor+2);
		/*Make other vertices*/
		for(int j=3;j<=hor;j++){
			Id v1=base+j;
			Id v2=v1-hor;
			Id v3=v1-1;
			Id v4=v3-1;
			Id f2=2+(i-1)*hor+j;
			mef(solidNum,1,v1,v2,v3,v4,f2);
		}//for theta


	}//for layers
	
	/*Build the final vertex*/
	ver--;
	int top=hor*ver+1;
	mev(solidNum,1,1,hor*ver,hor*ver-1,hor*ver-1,hor*ver+1,0,0,topZ);
	mef(solidNum,1,hor*(ver-1)+1,hor*ver,top,hor*ver,2+hor*(ver-1)+1);
	for (int i=2;i<hor;i++){
		int now=hor*(ver-1)+i;
		int prev=now-1;
		mef(solidNum,1,now,prev,top,hor*ver,2+hor*(ver-1)+i);
	}//for n-1 top faces
	ver++;
	int bot=top+1;
	mev(solidNum,2,2,hor,1,1,bot,0,0,-topZ);
	mef(solidNum,2,bot,hor,1,2,1+hor*(ver-1)+1);
	for (int i=2;i<hor;i++){
		mef(solidNum,2,bot,i-1,i,i+1,1+hor*(ver-1)+i);
	}//for n-1 bottom faces
	
	
	}//if rad1<=rad2
	

	/*build layers*/	

	return solid;
}

Solid * Euler_Ops::bullet(int solidNum)
{
	Solid * solid;
	double rad=radius;
	int hor=nsegments;
	int ver=nsections;
	/*Build the first two*/
	solid=mvfs(1,1,1,rad,0,0);
	double horr=PI*2/hor;
	//double verr=PI/ver;
	
	/*Build the polygon*/
	for (int i=2;i<=hor;i++){
		int prev=i-2>0?i-2:i-1;
		mev(solidNum,1,1,i-1,prev,prev,i,rad*cos(horr*(i-1)),rad*sin(horr*(i-1)),0);
	}
	//cout<<"Polygon done!"<<endl;
	/*face 1 up, face 2 bottom*/
	mef(solidNum,1,1,2,hor,hor-1,2);
	
	/*Build the upper hemisphere*/
	for (int i=1;i<=ver;i++){
		int base=i*hor;
		/*compute the layers*/
		double xyr=rad*cos(PI/2/ver*(i-1));
		double z=height+rad*sin(PI/2/ver*(i-1));
		
		/*Make all verts on this layer*/
		/*Make the first vertex*/
		mev(solidNum,1,1,(i-1)*hor+1,i*hor,i*hor,i*hor+1,xyr,0,z);
		/*Make other vertices*/
		for(int j=2;j<=hor;j++){
			int prev=(i-1)*hor+j;
			double x=0;
			double y=0;
			if (i==1){
				x=rad*cos(PI*2/hor*(j-1));
				y=rad*sin(PI*2/hor*(j-1));
				z=height;
			}
			else{
				x=xyr*cos(PI*2/hor*(j-1));
				y=xyr*sin(PI*2/hor*(j-1));
			}
			mev(solidNum,1,1,prev,prev-1,prev-1,prev+hor,x,y,z);
		}//for theta
		
		/*Make all edges*/
		/*Make the first face*/
		mef(solidNum,1,base+1,base+1-hor,base+hor,base,2+(i-1)*hor+1);
		mef(solidNum,1,base+2,base+2-hor,base+1,base+hor,2+(i-1)*hor+2);
		/*Make other vertices*/
		for(int j=3;j<=hor;j++){
			Id v1=base+j;
			Id v2=v1-hor;
			Id v3=v1-1;
			Id v4=v3-1;
			Id f2=2+(i-1)*hor+j;
			mef(solidNum,1,v1,v2,v3,v4,f2);
		}//for theta


	}//for layers
	//cout<<"layers done"<<endl;
	
	/*Build the final vertex*/
	/*int top=hor*ver+1;
	mev(1,1,1,hor*ver,hor*ver-1,hor*ver-1,hor*ver+1,0,0,rad);
	cout<<"final vertex done"<<endl;
	mef(1,1,hor*(ver-1)+1,hor*ver,top,hor*ver,2+hor*(ver-1)+1);
	for (int i=2;i<hor;i++){
		int now=hor*(ver-1)+i;
		int prev=now-1;
		mef(1,1,now,prev,top,hor*ver,2+hor*(ver-1)+i);
	}*/


	/*build layers*/	

	return solid;	
}

/*Primitives*/
Solid* Euler_Ops::prim(string name, int solidNum)
{

	Solid* solid;

	if (name=="hemisphere")
	{
		return hemisphere(solidNum);
	}// if a hemisphere

	else if (name.compare("sphere")==0){
		return sphere(solidNum);
	}// if a sphere

	/*build a cylinder*/
	else if (name.compare("cylinder")==0){
		return cylinder(solidNum);
	}//if a cylinder

	else if (name.compare("bullet")==0){
		return bullet(solidNum);
	}// if a bullet

	else if (name.compare("cone")==0){
		return cone(solidNum);
	}//if cone

	if (name.compare("trefoil")==0){
		return trefoil(solidNum);
	}//trefoil

	/*disk*/
	if (name.compare("disk")==0){
		return disk(solidNum);
	}// if a disk
	
	/*Torus*/
	if (name.compare("torus")==0){
		return torus(solidNum);
	}// if a torus

	if (name.compare("chain")==0){
		double rad1=radius;
		double rad2=radius2;
		int hor=nsegments;
		int ver=nsections;
		if (rad1>rad2){
		
		for (int s=1;s<=nparts;s++){
		double movX=(s-1)*rad1*1.2;
			/*Build the first two*/
		solid=mvfs(s,1,1,movX+rad1+rad2,0,0);
		double horr=PI*2/hor;
		//double verr=PI*2/ver;
		
		/*Build the polygon*/
		for (int i=2;i<=hor;i++){
			int prev=i-2>0?i-2:i-1;
			/*minus y plus z*/
			double y=(rad1+rad2)*sin(horr*(i-1));
			double z=0;
			if (s%2==1)
				mev(s,1,1,i-1,prev,prev,i,movX+(rad1+rad2)*cos(horr*(i-1)),y,z);
			else
				mev(s,1,1,i-1,prev,prev,i,movX+(rad1+rad2)*cos(horr*(i-1)),-z,y);
		}
		/*face 1 up, face 2 bottom*/
		mef(s,1,1,2,hor,hor-1,2);
		
		
		/*Build the upper hemisphere*/
		for (int i=1;i<ver;i++){
			int base=i*hor;
			/*compute the layers*/
			double xyr=rad1+rad2*cos(2.0*PI/ver*i);
			double z=rad2*sin(2.0*PI/ver*i);
			
			/*Make all verts on this layer*/
			/*Make the first vertex*/
			if (s%2==1)
				mev(s,1,1,(i-1)*hor+1,i*hor,i*hor,i*hor+1,movX+xyr,0,z);
			else
				mev(s,1,1,(i-1)*hor+1,i*hor,i*hor,i*hor+1,movX+xyr,-z,0);
			/*Make other vertices*/
			for(int j=2;j<=hor;j++){
				int prev=(i-1)*hor+j;
				
				double x=xyr*cos(PI*2/hor*(j-1));
				double y=xyr*sin(PI*2/hor*(j-1));
			if (s%2==1)
				mev(s,1,1,prev,prev-1,prev-1,prev+hor,x+movX,y,z);
			else
				mev(s,1,1,prev,prev-1,prev-1,prev+hor,x+movX,-z,y);
			}//for theta
			
			/*Make all edges*/
			/*Make the first face*/
			mef(s,1,base+1,base+1-hor,base+hor,base,2+(i-1)*hor+1);
			mef(s,1,base+2,base+2-hor,base+1,base+hor,2+(i-1)*hor+2);
			/*Make other vertices*/
			for(int j=3;j<=hor;j++){
				Id v1=base+j;
				Id v2=v1-hor;
				Id v3=v1-1;
				Id v4=v3-1;
				Id f2=2+(i-1)*hor+j;
				mef(s,1,v1,v2,v3,v4,f2);
			}//for theta


		}//for layers
		/*Build the final vertex*/
		/*int top=hor*ver+1;
		mev(1,1,1,hor*ver,hor*ver-1,hor*ver-1,hor*ver+1,0,0,rad1);
		mef(1,1,hor*(ver-1)+1,hor*ver,top,hor*ver,2+hor*(ver-1)+1);
		for (int i=2;i<hor;i++){
			int now=hor*(ver-1)+i;
			int prev=now-1;
			mef(1,1,now,prev,top,hor*ver,2+hor*(ver-1)+i);
		}*/

		/*Add the face*/
		kfmrh(s,2,1);
		mekr(s,2,1,2,hor*(ver-1)+1,hor*ver);
		
      		/*Make the edge faces*/
		for (int i=2;i<=hor;i++){
		    mef(s,2,hor*(ver-1)+i,hor*(ver-1)+i-1,i,i+1>hor?1:i+1,hor*(ver-1)+i+1);
		}
		
		}//for nparts : # of rings

		}//rad1>rad2

		else {
			cout<<"Wrong! Radius1 should be bigger than Radius2!"<<endl;
		
		}//if rad1<=rad2
		

		/*build layers*/	
	}// if a chain
	
	if (name.compare("table")==0)
	{
	    // Chee: THESE PARAMETERS ARE NOT INTUITIVE!!!
	    //         PLEASE MAKE SOME COMMENTS IN CODE!
		/*Table Radius*/
		double rad=radius;
		/*Table face height*/
		//double height=size[1];
		int hor = nsegments;
		
		/*Leg Position and Leg numbers*/
		double legPos=position2;//This is the distance from the center/radius
		int legNum=nparts2;
		
		/*Leg Radius*/
		double legRad=radius2;
		double legHor=nsegments2;
		double legHeight=height2;
		/*Build the first two*/
		solid=mvfs(solidNum,1,1,rad,0,0);
		double horr=PI*2/hor;
		//double verr=PI/ver;
		
		/*Build the polygon*/
		for (int i=2;i<=hor;i++){
			int prev=i-2>0?i-2:i-1;
			mev(solidNum,1,1,i-1,prev,prev,i,rad*cos(horr*(i-1)),rad*sin(horr*(i-1)),0);
		}
		/*face 1 up, face 2 bottom*/
		mef(solidNum,1,1,2,hor,hor-1,2);
		
		/*Build the verts and faces*/
		for (int i=1;i<2;i++){
			int base=i*hor;
			/*compute the layers*/
			double xyr=rad;
			double z=height;
			
			/*Make all verts on this layer*/
			/*Make the first vertex*/
			mev(solidNum,1,1,(i-1)*hor+1,i*hor,i*hor,i*hor+1,xyr,0,z);
			/*Make other vertices*/
			for(int j=2;j<=hor;j++){
				int prev=(i-1)*hor+j;
				
				double x=xyr*cos(PI*2/hor*(j-1));
				double y=xyr*sin(PI*2/hor*(j-1));
				mev(solidNum,1,1,prev,prev-1,prev-1,prev+hor,x,y,z);
			}//for theta
			
			/*Make all edges*/
			/*Make the first face*/
			mef(solidNum,1,base+1,base+1-hor,base+hor,base,2+(i-1)*hor+1);
			mef(solidNum,1,base+2,base+2-hor,base+1,base+hor,2+(i-1)*hor+2);
			/*Make other vertices*/
			for(int j=3;j<=hor;j++){
				Id v1=base+j;
				Id v2=v1-hor;
				Id v3=v1-1;
				Id v4=v3-1;
				Id f2=2+(i-1)*hor+j;
				mef(solidNum,1,v1,v2,v3,v4,f2);
			}//for theta


		}//for layers


		/**********************Now we have a cylinder********************************/
		/*Build legs*/
		for (int i=0;i<legNum;i++){
			int vertNum=hor*2+i*legHor*2;
			int faceNum=2+hor+i*(legHor+1);
			
			/*Get the leg center*/
			double centerX=legPos*cos(2*PI/legNum*i);
			double centerY=legPos*sin(2*PI/legNum*i);
			
			/*The first point*/
			double x1=centerX+legRad*cos(2*PI/legNum*i);
			double y1=centerY+legRad*sin(2*PI/legNum*i);
			double x2=centerX+legRad*cos(2*PI/legNum*i+2*PI/legHor);
			double y2=centerY+legRad*sin(2*PI/legNum*i+2*PI/legHor);
			/*The first & second point*/
			mev(solidNum,2,2,1,2,2,vertNum+1,x1,y1,0);
			mev(solidNum,2,2,vertNum+1,1,1,vertNum+2,x2,y2,0);
			
			/*The other n-2 points*/
			for (int j=3;j<=legHor;j++){
				double xj=centerX+legRad*cos(2*PI/legNum*i+2*PI/legHor*(j-1));
				double yj=centerY+legRad*sin(2*PI/legNum*i+2*PI/legHor*(j-1));
				mev(solidNum,2,2,vertNum+j-1,vertNum+j-2,vertNum+j-2,vertNum+j,xj,yj,0);
			}//n-2 points

			/*Close the cycle*/
			mef(solidNum,2,vertNum+1,vertNum+2,vertNum+legHor,vertNum+legHor-1,faceNum+1);
			/*Kemr: kill the edge*/
			kemr(solidNum,2,1,vertNum+1);
			
			/******************Draw the Leg******************/
			/*Draw edges*/
			for (int j=1;j<=legHor;j++){
				double xj=centerX+legRad*cos(2*PI/legNum*i+2*PI/legHor*(j-1));
				double yj=centerY+legRad*sin(2*PI/legNum*i+2*PI/legHor*(j-1));
				mev(solidNum,faceNum+1,faceNum+1,vertNum+j,vertNum+((j+1)>legHor?1:(j+1)),vertNum+((j+1)>legHor?1:(j+1)),vertNum+legHor+j,xj,yj,-legHeight);
			}//add n edge & vertices
			
			/*Draw the final edges*/
			for (int j=1;j<=legHor;j++){
				mef(solidNum,faceNum+1,vertNum+legHor+j,vertNum+j,vertNum+legHor+((j+1)>legHor?1:(j+1)),vertNum+((j+1)>legHor?(legHor+2):(j+1)),faceNum+1+j);
			}//add n edge & faces

  		}//for legs	
	}//table

	if (name.compare("chair")==0){
		/*Table Radius*/
		double rad=radius;
		/*Table face height*/
		//double height=size[1];
		int hor = nsegments;
		
		/*Leg Position and Leg numbers*/
		double legPos=position2;
		int legNum=nparts2;
		
		/*Leg Radius*/
		double legRad=radius2;
		double legHor=nsegments2;
		double legHeight=height2;
		/*Build the first two*/
		solid=mvfs(1,1,1,rad,0,0);
		double horr=PI*2/hor;
		//double verr=PI/ver;
		
		/*Build the polygon*/
		for (int i=2;i<=hor;i++){
			int prev=i-2>0?i-2:i-1;
			mev(solidNum,1,1,i-1,prev,prev,i,rad*cos(horr*(i-1)),rad*sin(horr*(i-1)),0);
		}
		/*face 1 up, face 2 bottom*/
		mef(solidNum,1,1,2,hor,hor-1,2);
		
		/*Build the verts and faces*/
		for (int i=1;i<2;i++){
			int base=i*hor;
			/*compute the layers*/
			double xyr=rad;
			double z=height;
			
			/*Make all verts on this layer*/
			/*Make the first vertex*/
			mev(solidNum,1,1,(i-1)*hor+1,i*hor,i*hor,i*hor+1,xyr,0,z);
			/*Make other vertices*/
			for(int j=2;j<=hor;j++){
				int prev=(i-1)*hor+j;
				
				double x=xyr*cos(PI*2/hor*(j-1));
				double y=xyr*sin(PI*2/hor*(j-1));
				mev(solidNum,1,1,prev,prev-1,prev-1,prev+hor,x,y,z);
			}//for theta
			
			/*Make all edges*/
			/*Make the first face*/
			mef(solidNum,1,base+1,base+1-hor,base+hor,base,2+(i-1)*hor+1);
			mef(solidNum,1,base+2,base+2-hor,base+1,base+hor,2+(i-1)*hor+2);
			/*Make other vertices*/
			for(int j=3;j<=hor;j++){
				Id v1=base+j;
				Id v2=v1-hor;
				Id v3=v1-1;
				Id v4=v3-1;
				Id f2=2+(i-1)*hor+j;
				mef(solidNum,1,v1,v2,v3,v4,f2);
			}//for theta


		}//for layers


		/**********************Now we have a cylinder********************************/
		/*Build legs*/
		for (int i=0;i<legNum;i++){
			int vertNum=hor*2+i*legHor*2;
			int faceNum=2+hor+i*(legHor+1);
			
			/*Get the leg center*/
			double centerX=legPos*cos(2*PI/legNum*i);
			double centerY=legPos*sin(2*PI/legNum*i);
			
			/*The first point*/
			double x1=centerX+legRad*cos(2*PI/legNum*i);
			double y1=centerY+legRad*sin(2*PI/legNum*i);
			double x2=centerX+legRad*cos(2*PI/legNum*i+2*PI/legHor);
			double y2=centerY+legRad*sin(2*PI/legNum*i+2*PI/legHor);
			/*The first & second point*/
			mev(solidNum,2,2,1,2,2,vertNum+1,x1,y1,0);
			mev(solidNum,2,2,vertNum+1,1,1,vertNum+2,x2,y2,0);
			
			/*The other n-2 points*/
			for (int j=3;j<=legHor;j++){
				double xj=centerX+legRad*cos(2*PI/legNum*i+2*PI/legHor*(j-1));
				double yj=centerY+legRad*sin(2*PI/legNum*i+2*PI/legHor*(j-1));
				mev(solidNum,2,2,vertNum+j-1,vertNum+j-2,vertNum+j-2,vertNum+j,xj,yj,0);
			}//n-2 points

			/*Close the cycle*/
			mef(solidNum,2,vertNum+1,vertNum+2,vertNum+legHor,vertNum+legHor-1,faceNum+1);
			/*Kemr: kill the edge*/
			kemr(solidNum,2,1,vertNum+1);
			
			/******************Draw the Leg******************/
			/*Draw edges*/
			for (int j=1;j<=legHor;j++){
				double xj=centerX+legRad*cos(2*PI/legNum*i+2*PI/legHor*(j-1));
				double yj=centerY+legRad*sin(2*PI/legNum*i+2*PI/legHor*(j-1));
				mev(solidNum,faceNum+1,faceNum+1,vertNum+j,vertNum+((j+1)>legHor?1:(j+1)),vertNum+((j+1)>legHor?1:(j+1)),vertNum+legHor+j,xj,yj,-legHeight);
			}//add n edge & vertices
			
			/*Draw the final edges*/
			for (int j=1;j<=legHor;j++){
				mef(solidNum,faceNum+1,vertNum+legHor+j,vertNum+j,vertNum+legHor+((j+1)>legHor?1:(j+1)),vertNum+((j+1)>legHor?(legHor+2):(j+1)),faceNum+1+j);
			}//add n edge & faces

  		}//for legs	


		/**************************After finish bottom and the legs, build the back*******************************/
		int vertNum=hor*2+legNum*legHor*2;
		int faceNum=2+hor+legNum*(legHor+1);
		
		/*Get parameter information*/
		double backRad=position3;
		//double backWid=size[9];
		double backThi=thickness3;
		double backHei=height3;
		
		/*Build the back*/
		double b1[]={backRad,0,height};
		double b2[]={backRad*cos(2*PI/hor),backRad*sin(2*PI/hor),height};
		double v[]={b1[1]-b2[1],b2[0]-b1[0],0};
		double normV[]={1,2,3};
		/*Normalize the vector*/
		norm(v,normV,backThi);
		double b3[]={b2[0]+normV[0],b2[1]+normV[1],b2[2]+normV[2]};
		double b4[]={b1[0]+normV[0],b1[1]+normV[1],b1[2]+normV[2]};
		mev(solidNum,1,1,hor+1,2*hor,2*hor,vertNum+1,b1[0],b1[1],b1[2]);
		mev(solidNum,1,1,vertNum+1,hor+1,hor+1,vertNum+2,b4[0],b4[1],b4[2]);
		mev(solidNum,1,1,vertNum+2,vertNum+1,vertNum+1,vertNum+3,b3[0],b3[1],b3[2]);
		mev(solidNum,1,1,vertNum+3,vertNum+2,vertNum+2,vertNum+4,b2[0],b2[1],b2[2]);
		
		/*Close the cycle*/
		mef(solidNum,1,vertNum+1,vertNum+2,vertNum+4,vertNum+3,faceNum+1);
		/*Remove the edge*/
		kemr(solidNum,1,hor+1,vertNum+1);
		
		/*Build the back*/
		/*Four vertical lines*/
		mev(solidNum,faceNum+1,faceNum+1,vertNum+1,vertNum+2,vertNum+2,vertNum+5,b1[0],b1[1],b1[2]+backHei);
		mev(solidNum,faceNum+1,faceNum+1,vertNum+2,vertNum+3,vertNum+3,vertNum+6,b4[0],b4[1],b4[2]+backHei);
		mev(solidNum,faceNum+1,faceNum+1,vertNum+3,vertNum+4,vertNum+4,vertNum+7,b3[0],b3[1],b3[2]+backHei);
		mev(solidNum,faceNum+1,faceNum+1,vertNum+4,vertNum+1,vertNum+1,vertNum+8,b2[0],b2[1],b2[2]+backHei);

		/*Finish the faces*/
		mef(solidNum,faceNum+1,vertNum+5,vertNum+1,vertNum+6,vertNum+2,faceNum+2);
		mef(solidNum,faceNum+1,vertNum+6,vertNum+2,vertNum+7,vertNum+3,faceNum+3);
		mef(solidNum,faceNum+1,vertNum+7,vertNum+3,vertNum+8,vertNum+4,faceNum+4);
		mef(solidNum,faceNum+1,vertNum+8,vertNum+4,vertNum+5,vertNum+6,faceNum+5);
	}//chair

	return solid;
}