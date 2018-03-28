//  ========================================================================
//  COSC363: Computer Graphics (2017);  University of Canterbury.
//
//  FILE NAME: Building.cpp
//  See Lab03.pdf for details
//
//  Program displays the model of an apartment block using textures loaded from
//  Apartment.tga
//  Use left/right arrow keys to rotate the scene
//  Use up/down arrow keys to move camera up/down
//  ========================================================================
 
#include <iostream>
#include <climits>
#include <math.h> 
#include <GL/freeglut.h>
#include "loadBMP.h"
using namespace std;

GLuint txId;  //Texture ID
float angle = 40;
float cam_hgt = 250;

//------Function to load a texture in bmp format  ------------------------
void loadTexture()				
{
	glGenTextures(1, &txId); 				// Create a Texture object
	glBindTexture(GL_TEXTURE_2D, txId);		//Use this texture
    loadBMP("Apartment.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

//--------draws the mesh model of the cannon----------------------------
void drawBuilding()
{
	glColor3f(0.6, 0.4, 0.4);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	  glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
	  glTexCoord2f(0,0); glVertex3f(-50, 0.0, 20.0);
	  glTexCoord2f(1,0); glVertex3f(50.0, 0.0, 20.0);
	  glTexCoord2f(1,1); glVertex3f(50.0, 200.0, 20.0);
	  glTexCoord2f(0,1); glVertex3f(-50.0, 200.0, 20.0);

	  glNormal3f(0.0, 0.0, -1.0);   //Facing -z (Back side)
	  glTexCoord2f(0.2188, 0.1); glVertex3f(50.0, 0.0, -20.0);
	  glTexCoord2f(0.7891, 0.1); glVertex3f(-50.0, 0.0, -20.0);
	  glTexCoord2f(0.7891, 0.9); glVertex3f(-50.0, 200.0, -20.0);
	  glTexCoord2f(0.2188, 0.9); glVertex3f(50.0, 200.0, -20.0);


	  glNormal3f(0.0, 1.0, 0.0);   //Facing +y (Top side)
	  glVertex3f(-50.0, 200.0, 20.0);
	  glVertex3f(50.0, 200.0, 20.0);
	  glVertex3f(50.0, 200.0, -20.0);
	  glVertex3f(-50.0, 200.0, -20.0);

	  glNormal3f(-1., 0., 0.);
	  glTexCoord2f(0.6, 0); glVertex3f(-50.0, 0.0, -20.0);   //Facing -x (Left side)
	  glTexCoord2f(0.8, 0); glVertex3f(-50.0, 0.0, 20.0);
	  glTexCoord2f(0.8, 1); glVertex3f(-50.0, 200.0, 20.0);
	  glTexCoord2f(0.6, 1); glVertex3f(-50.0, 200.0, -20.0);

	  glNormal3f(1.0, 0.0, 0.0);   //Facing +x (Right side)
	  glTexCoord2f(0.6, 0); glVertex3f(50.0, 0.0, 20.0);
	  glTexCoord2f(0.8, 0); glVertex3f(50.0, 0.0, -20.0);
	  glTexCoord2f(0.8, 1); glVertex3f(50.0, 200.0, -20.0);
	  glTexCoord2f(0.6, 1); glVertex3f(50.0, 200.0, 20.0);
	glEnd();

  glDisable(GL_TEXTURE_2D);
}

//----------draw a floor plane-------------------
void drawFloor()
{
	bool flag = false;

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for(int x = -200; x <= 200; x += 10)
	{
		for(int z = -200; z <= 200; z += 10)
		{
			if(flag) glColor3f(0.6, 0.4, 0.);
			else glColor3f(1.0, 0.8, 0.4);
			glVertex3f(x, 0, z);
			glVertex3f(x, 0, z+10);
			glVertex3f(x+10, 0, z+10);
			glVertex3f(x+10, 0, z);
			flag = !flag;
		}
	}
	glEnd();
}

//--Display: ----------------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()  
{
	float lpos[4] = {100., 100., 500., 1.0};  //light's position

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, cam_hgt, 400, 0, 100, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position

    glRotatef(angle, 0.0, 1.0, 0.0);		//rotate the whole scene

	drawFloor();

	drawBuilding();

	glFlush();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	loadTexture();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 100, 1000);  //The camera view volume  
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle--;
    else if(key == GLUT_KEY_RIGHT) angle++;
	else if(key == GLUT_KEY_UP) cam_hgt++;
	else if(key == GLUT_KEY_DOWN) cam_hgt--;

	if(cam_hgt > 400) cam_hgt = 400;
	else if(cam_hgt < 50) cam_hgt = 50;

	glutPostRedisplay();
}

//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Building");
   initialize();

   glutDisplayFunc(display);
   glutSpecialFunc(special); 
   glutMainLoop();
   return 0;
}
