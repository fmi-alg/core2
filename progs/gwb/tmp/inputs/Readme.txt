When building mev , on page 184, the two addhe should be exchanged because we should 
keep the vertex and add an edge first or the original vertex with an halfedge but no
edge will be lost.

When using mev, before using mef, we should focus on one face

Before your Solid is exactly a 3-D solid, all mev should be used on the same face.

For mef, he1 in the newloop,while he2 remains in the old one.
When drawing a cube after you have built a square and four edges you have to make 
sure that the edge on the upper face of the lamina should be in the new face.

Be careful when "closing" the cube because in the last step you may only have
single circle, which means that you cannot meet a duplicat point in the loop.

We can build different solids through #ifdef

For OPENGL, we have to download glut, with the command as follows in unbuntu:

sudo aptitude install freeglut3 freeglut3-dev

To install GLUT:

    sudo aptitude install freeglut3 freeglut3-dev  freeglut3-dbg
    sudo apt-get install libglui-dev libglui2c2


    This will install the OpenGL header files to /usr/include/GL/
    And the library objects to /usr/lib/libglut.a

    With all that taken care of now you just need a quick sample app to see if it worked, I googled "glut hello world" and this was the second result.

    I needed to add "#include ". Also notice that GLUT is included like "#include ". I then renamed it to helloworld.cpp and was able to compile as follows:

g++ rect.cc -o rect -lGLU -lGL -lglut -glui

The coordinates of the points and the position of the mouse arrow is inversed (x,y)vs(x,-y)


 --------------------OPENGL--------------------
1.You have to Normalize the coordinated using gluOrtho2D or glOrtho()(3D),for glOrtho(0,ww,0,wh,-z,z),-z is for translate(x,y,z) and z is for translate(x,y,-z) 

2.Multiplication by transformation matrix is from left to right.(Inverse Sequence)

3.For Matrix Mode
window:GL_PROJECTION
draw:GL_MODEL


------------------------Solid Modeling------------------------------------
1. We can draw a dodecahedron through
draw a cube first
cut though every half plane and choose another four points (They form a narrow rectangle)
So we can build a dodecahedron with first building a cube, then building 20 more faces and merge the connected ones


------------------------C++-----------------------------------------------
1.When defining a macro, it is better to put a pair of parentheses embracing everthing
or we will meet this situation:

#define fi ((1+SQRTFIVE)/2)
And when we computing 1/fi we actually get 1/4fi

2. For Vector in STL:
If we want to delete an element with the iterator,
in the loop whenever meet the element, we have to break,
or there will come up with a segmentation fault if you 
try to delete the last one.(After that, the it->end will be something else)

