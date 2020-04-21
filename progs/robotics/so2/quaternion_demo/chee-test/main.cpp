/* main.cpp
 *
 *  Description:
 *  This program displays a 3D object that can be 
 *  rotated by setting the values of (w,x,y,z) of 
 *  a quaternion. First, one of the values must be 
 *  set to -1. Then the others may be adjusted freely.
 *
 *  Type the following in a command line to compile and run:
 *     > make q
 *
 *  		
 *  Author: Surin Ahn (April 2013)
 *  -- supervised by Professor Yap
 *
 ***********************************************************/

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include "Quaternion.h"

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include "glui.h"
#endif
#ifdef __linux__
#include <GL/glui.h>
#endif

using namespace std;

int main_window;
GLUI * glui_window;
GLUI_RadioGroup *radio;
int radiogroup_item_id = 0; //  Id of the selected radio button
	
// Values set by the spinners
float w_scale = 0.0;
float x_scale = 0.0;
float y_scale = 0.0;
float z_scale = 0.0;

// Define constants
const int RADIOGROUP = 1;
const int QUIT_BUTTON = 6;

void display (void) {    
	glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)  
	glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations  
  
	glTranslatef(0.0f, 0.0f, -5.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive  
		
	switch (radiogroup_item_id)
	{
		case (0): // W set to -1
			w_scale = -1.0f;
		break;
		case (1): // X set to -1
			x_scale = -1.0f;
		break;
		case (2): // Y set to -1
			y_scale = -1.0f;
		break;
		case (3): // Z set to -1
			z_scale = -1.0f;
		break;
	}	
	
	Quaternion q(w_scale, x_scale, y_scale, z_scale);	
	q.normalize();
	glMultMatrixf(q.getMatrix());
	
	glutWireTeapot(1.45f); // Render the primitive  
  
	glFlush(); // Flush the OpenGL buffers to the window  
}  
  
void reshape (int width, int height) {  
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window  
	glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed  
	glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)  
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of 		our window, and the new and far planes  
	glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly  
  
}  

void idle ( )
{
    glutSetWindow(main_window);
    glutPostRedisplay();
    sleep(0.2); // Reduce CPU usage
}

// Callback function
void control_cb( int control )
{
	switch (control)
	{	
		//  Quit Button clicked
		case QUIT_BUTTON:
		    cout << "Quit Button clicked... Exit!" << endl;

		    exit(1);
		break; 
		
		case RADIOGROUP:
		    cout << "Radio button selected: " << radiogroup_item_id << endl;
		break;
	}
}
  
int main (int argc, char **argv) {  
	glutInit(&argc, argv); // Initialize GLUT  
	glutInitDisplayMode (GLUT_SINGLE); // Set up a basic display buffer (only single buffered for now)  
	glutInitWindowSize (500, 500); // Set the width and height of the window  
	glutInitWindowPosition (100, 100); // Set the position of the window  

	main_window = glutCreateWindow ("Quaternion Tester"); // Set the title for the window
	//  Create GLUI window
	glutDisplayFunc(display); // Tell GLUT to use the method "display" for rendering  
	glutReshapeFunc(reshape); // Tell GLUT to use the method "reshape" for reshaping 
	
	glui_window = GLUI_Master.create_glui ("Options"); 

	GLUI_Master.set_glutIdleFunc (idle);
	
	//  Add the 'Set to -1' Panel to the GLUI window
	GLUI_Panel *set_panel = glui_window->add_panel ("Set to -1");

	//  Create radio button group
	GLUI_RadioGroup *radio = glui_window->add_radiogroup_to_panel (set_panel,
	    &radiogroup_item_id, RADIOGROUP, control_cb);
	
	//  Add the radio buttons to the radio group
	glui_window->add_radiobutton_to_group( radio, "W" );
	glui_window->add_radiobutton_to_group( radio, "X" );
	glui_window->add_radiobutton_to_group( radio, "Y" );
	glui_window->add_radiobutton_to_group( radio, "Z" );

	//  Add the 'Quaternion' Panel to the GLUI window
	GLUI_Panel *q_panel = glui_window->add_panel ("Quaternion");
		
	//  Add the scale spinners
	GLUI_Spinner *spinner1 = glui_window->add_spinner_to_panel (q_panel, "W", GLUI_SPINNER_FLOAT, &w_scale, 2, control_cb);
	GLUI_Spinner *spinner2 = glui_window->add_spinner_to_panel (q_panel, "X", GLUI_SPINNER_FLOAT, &x_scale, 3, control_cb);
	GLUI_Spinner *spinner3 = glui_window->add_spinner_to_panel (q_panel, "Y", GLUI_SPINNER_FLOAT, &y_scale, 4, control_cb);
	GLUI_Spinner *spinner4 = glui_window->add_spinner_to_panel (q_panel, "Z", GLUI_SPINNER_FLOAT, &z_scale, 5, control_cb);
	//  Set the limits for the spinners
	spinner1->set_float_limits ( -1.0, 1.0 );
	spinner2->set_float_limits ( -1.0, 1.0 );
	spinner3->set_float_limits ( -1.0, 1.0 );
	spinner4->set_float_limits ( -1.0, 1.0 );
  
	//  Add the Quit Button
	glui_window->add_button ("Quit", QUIT_BUTTON, control_cb);	

	glui_window->set_main_gfx_window (main_window); // Associate GLUI window with main graphics window
	glutMainLoop(); // Enter GLUT's main loop  
}  
