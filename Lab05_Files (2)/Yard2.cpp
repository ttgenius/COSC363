//  ========================================================================
//  COSC363: Computer Graphics (2016);  University of Canterbury.
//
//  FILE NAME: Yard.cpp
//  See Lab05.pdf for details.
//  ========================================================================
 
#include <iostream>
#include <cmath> 
#include <GL/glut.h>
#include "loadTGA.h"
using namespace std;

GLuint txId[2];   //Texture ids
float  eye_x = 12,  eye_y = 1,  eye_z = 12;    //Initial camera position
float look_x = 12, look_y = 1, look_z = 10;    //"Look-at" point along -z direction
float  theta = 0;                              //Look angle
int step = 0;								   //camera motion

//--------------------------------------------------------------------------------
void loadTexture()				
{
	glGenTextures(2, txId); 	// Create 2 texture ids

	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("Wall.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("Floor.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

//--------------------------------------------------------------------------------
void special(int key, int x, int y)
{
	step = 0;
	if(key == GLUT_KEY_LEFT) theta += 0.1;   //in radians
	else if(key == GLUT_KEY_RIGHT) theta -= 0.1;
	else if(key == GLUT_KEY_DOWN) step = -1;
	else if(key == GLUT_KEY_UP) step = 1;

	glutPostRedisplay();
}

//--------------------------------------------------------------------------------

void initialise()
{ 
//	loadTexture();		
//	glEnable(GL_TEXTURE_2D);

	glClearColor(0., 1., 1., 1.);  //Background colour  = cyan
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();
	gluPerspective(45., 1., 1., 100.);
}

//--------------------------------------------------------------------------------

void walls()
{
	glColor3f(0.8, 0.7, 0.3);

 	glBegin(GL_QUADS);

	////////////////////// BACK WALL ///////////////////////

		glVertex3f(-15, 2, -15);
		glVertex3f(-15, 0, -15);
		glVertex3f(15, 0, -15);
		glVertex3f(15, 2, -15);

	////////////////////// FRONT WALL ///////////////////////

		glVertex3f(-15, 2, 15);
		glVertex3f(-15, 0, 15);
		glVertex3f(15, 0, 15);
		glVertex3f(15, 2, 15);

	////////////////////// LEFT WALL ///////////////////////

		glVertex3f(-15, 2, -15);
		glVertex3f(-15, 0, -15);
		glVertex3f(-15, 0, 15);
		glVertex3f(-15, 2, 15);


	////////////////////// RIGHT WALL ///////////////////////

		glVertex3f(15, 2, -15);
		glVertex3f(15, 0, -15);
		glVertex3f(15, 0, 15);
		glVertex3f(15, 2, 15);

	glEnd();
}
//--------------------------------------------------------------------------------

void floor()
{
	glColor3d(0.6, 0.25, 0.25);

	glBegin(GL_QUADS);
		glVertex3f(-15, 0, -15);
		glVertex3f(-15, 0, 15);
		glVertex3f(15, 0, 15);
		glVertex3f(15, 0, -15);
	glEnd();
}

//--------------------------------------------------------------------------------

void display() 
{
	float dir_x = -sin(theta), dir_y = 0,  dir_z = -cos(theta);

	//eye_x =
	//eye_z =

	//look_x = 
	//look_y = 
	//look_z = 

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glMatrixMode(GL_MODELVIEW);								
	glLoadIdentity();
	gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z,   0, 1, 0);	
	
	walls();
	floor();

	glutSwapBuffers();									
}

//--------------------------------------------------------------------------------

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("The Yard");
   initialise();

   glutDisplayFunc(display); 
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
