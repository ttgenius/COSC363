//  ========================================================================
//  COSC363: Computer Graphics (2016);  University of Canterbury.
//
//  FILE NAME: Earth.cpp
//  See Lab03.pdf for details.
//  ========================================================================

#include <iostream>
#include <GL/freeglut.h>
#include "loadTGA.h"
using namespace std;

GLuint txId[2];
GLUquadricObj*	q;
float angle = 0;

//--------------------------------------------------------------------------------
void loadTexture()	 
{
	glGenTextures(2, txId);   //Get 2 texture IDs 
	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture name for the following OpenGL texture
	loadTGA("Earth.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture name for the following OpenGL texture
	loadTGA("Sun.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//--------------------------------------------------------------------------------

void initialise()  
{
    float black[4] = {0.0, 0.0, 0.0, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

	q =  gluNewQuadric ( );
	loadTexture();  	

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 	 //Background
	glClearDepth(1.0f);  	
	glEnable(GL_DEPTH_TEST);   	
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);	
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	gluQuadricDrawStyle (q, GLU_LINE );
	gluQuadricNormals	(q, GLU_SMOOTH );

	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture (q, GL_TRUE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 2.0, 10., 100.);
}

//============================================================
void display()  
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();  	
	gluLookAt(0., 0., 40., 0., 0., 0., 0., 1., 0.);
	
	glColor4f(1.0, 1.0, 1.0, 1.0);        //Base colour
	
	//Earth
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
	glPushMatrix();
	    glRotatef(angle, 0, 1, 0);       //rotate around the sun
	    glTranslatef(20.0, 0.0, 0.0);	 //Translate Earth along x-axis by 20 units	
	    glRotatef(angle, 0, 1, 0);       //Rotation about polar axis of the Earth
	    glRotatef(-90., 1.0, 0., 0.0);   //make the sphere axis vertical
	    gluSphere ( q, 3.0, 36, 17 );
    glPopMatrix();
    
	//Sun
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);	
	glPushMatrix();		
	    glRotatef(-90., 1.0, 0., 0.0);   //make the sphere axis vertical
	    gluSphere ( q, 4.0, 36, 17 );
    glPopMatrix();
    

	glutSwapBuffers();
}

//----------------------------------------------------------------
void timer(int value)
{
	angle ++;
	if(angle > 360) angle = 0;
	glutTimerFunc(50, timer, value);
	glutPostRedisplay();
}

//----------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (800, 400); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   initialise ();
   glutDisplayFunc(display);
   glutTimerFunc(50, timer, 0);
   glutMainLoop();
   return 0;
}
