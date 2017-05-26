#include <iostream>
#include <fstream>
#include <cmath> 
#include <GL/freeglut.h>
#include <math.h>
#include <climits>
#include "loadTGA.h"
#include "loadBMP.h"
using namespace std;
GLuint txId[8];   //Texture ids
int cam_hgt = 10;
float angle = 10.0;  //Rotation angle for viewing
float theta = 20.0;
int direction=1;
GLUquadric *q; 

//--Draws a grid of lines on the floor plane -------------------------------
void drawFloor()
{
	glColor3f(0., 0.5, 0.);			//Floor colour
	for(float i = -50; i <= 50; i ++)
	{
		glBegin(GL_LINES);			//A set of grid lines on the xz-plane
			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);
			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);
		glEnd();
	}
}
 
//--Draws a character model constructed using GLUT objects ------------------
void drawRobot()
{
	glColor3f(1.,0.76, 0.06);		//Head
	glPushMatrix();
	  glTranslatef(0, 7.7, 0);
	  glutSolidSphere(1,20,20);
	glPopMatrix();
	
	
	glColor3f(0,0,0);         
    glPushMatrix();                  //eyes       
        glTranslatef(0.4, 8, 0.7);
        glutSolidSphere(0.2,20,20);
	glPopMatrix();
	
	glColor3f(0,0,0);         
    glPushMatrix();                  //eyes       
        glTranslatef(-0.4, 8, 0.7);
        glutSolidSphere(0.2,20,20);
	glPopMatrix();
	
	
	
	glPushMatrix();                 //mouth
       glTranslatef(0, 7.55, 0.7);
        glScalef(3, 0.4, 1.8);
        glutSolidSphere(0.2,20,20);
	glPopMatrix();    
	
	
	glColor3f(1., 0., 0.);			//Torso
	glPushMatrix();
	  glTranslatef(0, 5.5, 0);
	  glScalef(3, 3, 1.4);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Right leg
	glPushMatrix();
		glTranslatef(-0.8,4,0);
		glRotatef(-theta, 1, 0, 0);
		glTranslatef(0.8, -4, 0);
	    glTranslatef(-0.8, 2.2, 0);
	    glScalef(1, 4.4, 1);
	    glutSolidCube(1);
	glPopMatrix();

    glColor3f(0., 0., 1.);         //Left leg   
	glPushMatrix();
		glTranslatef(-0.8, 4, 0);
		glRotatef(theta, 1, 0, 0);
		glTranslatef(0.8, -4, 0);
		glTranslatef(0.8, 2.2, 0);
	    glScalef(1, 4.4, 1);
		glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Right arm
	glPushMatrix();
		glTranslatef(-2, 6.5, 0);
		glRotatef(theta, 1, 0, 0);
		glTranslatef(2, -6.5, 0);
		glTranslatef(-2, 5, 0);
		glScalef(1, 4, 1);
		glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//Left arm
	glPushMatrix();
		glTranslatef(2,6.5,0);
		glRotatef(-theta, 1, 0, 0);
		glTranslatef(-2, -6.5, 0);
		glTranslatef(2, 5, 0);
		glScalef(1, 4, 1);
		glutSolidCube(1);
	glPopMatrix();
	
	glFlush();
}

void drawPig(){
	
    glColor4f(0.2, 0.2, 0.2, 1.0);   //The base is nothing but a scaled cube!
    glPushMatrix();
      glTranslatef(0.0, 4.0, 0.0);
      glScalef(20.0, 2.0, 10.0);     //Size 20x10 units, thickness 2 units.
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();					//Connector between wagons
      glTranslatef(11.0, 4.0, 0.0);
      glutSolidCube(2.0);
    glPopMatrix();

    //Wheels
    glColor4f(0.5, 0., 0., 1.0);
    glPushMatrix();
      glTranslatef(-8.0, 2.0, 5.1);
      gluCylinder(q, 0.0, 2.0, 20, 2);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(8.0, 2.0, 5.1);
      gluDisk(q, 0.0, 2.0, 20, 2);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-8.0, 2.0, -5.1);
      glRotatef(180.0, 0., 1., 0.);
      gluDisk(q, 0.0, 2.0, 20, 2);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(8.0, 2.0, -5.1);
      glRotatef(180.0, 0., 1., 0.);
      gluDisk(q, 0.0, 2.0, 20, 2);
    glPopMatrix();
}
/*void skybox() {
	glEnable(GL_BLEND);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPushMatrix();
		//float white[4] = {1.0, 1.0, 1.0, 1.0};
		//float black[4] = {0.0, 0.0, 0.0, 1.0};
		//glMaterialfv(GL_FRONT, GL_SPECULAR, black);
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glEnable(GL_TEXTURE_2D);
		// Back
		glBindTexture(GL_TEXTURE_2D, txId[3]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f(-200.0, -200.0, -200.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f( 200.0, -200.0, -200.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f( 200.0,  200.0, -200.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(-200.0,  200.0, -200.0);
		glEnd();
		// Right
		glBindTexture(GL_TEXTURE_2D, txId[5]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f(200.0, -200.0, -200.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f(200.0, -200.0,  200.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(200.0,  200.0,  200.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(200.0,  200.0, -200.0);
		glEnd();
		// Front
		glBindTexture(GL_TEXTURE_2D, txId[2]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f( 200.0, -200.0, 200.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f(-200.0, -200.0, 200.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(-200.0,  200.0, 200.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f( 200.0,  200.0, 200.0);
		glEnd();
		// Left
		glBindTexture(GL_TEXTURE_2D, txId[4]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f(-200.0, -200.0,  200.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f(-200.0, -200.0, -200.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(-200.0,  200.0, -200.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(-200.0,  200.0,  200.0);
		glEnd();
		// Top
		glBindTexture(GL_TEXTURE_2D, txId[6]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f( 200.0,  200.0, -200.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f( 200.0,  200.0,  200.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(-200.0,  200.0,  200.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(-200.0,  200.0, -200.0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		//glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glPopMatrix();
	glDisable(GL_BLEND);
}*/
//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()  
{
	float lpos[4] = {10., 10., 10., 1.0};  //light's position

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, cam_hgt, 10, 0, 4, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position

    glRotatef(angle, 0.0, 1.0, 0.0);  //Rotate the scene about the y-axis

	glEnable(GL_LIGHTING);	       //Enable lighting when drawing the model.
    drawFloor();

	glEnable(GL_LIGHTING);	       //Enable lighting when drawing the model
	drawRobot();
    drawPig()
    //skybox();
	glFlush();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 1000.0);   //Camera Frustum
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle--;
    else if(key == GLUT_KEY_RIGHT) angle++;
    glutPostRedisplay();
}


//-----timer------------------------------------------------------------

void myTimer(int value)
{
	if (theta==20){
		direction=-1;
	}
	if (theta==-20){
		direction=1;
		}
	theta+=direction;
	glutPostRedisplay();
	glutTimerFunc(10, myTimer, 0);
	
}
//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Humanoid");
   initialize();
   
   glutTimerFunc(10, myTimer, 0); 
   glutDisplayFunc(display);
   glutSpecialFunc(special); 
   glutMainLoop();
   return 0;
}
