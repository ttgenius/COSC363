//  ========================================================================
//  COSC363: Computer Graphics (2016);  University of Canterbury.
//
//  FILE NAME: Train.cpp
//  See Lab03.pdf for details
//  ========================================================================

//#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

GLUquadric *q;    //Required for creating cylindrical objects
int theta = 20;
//-- Ground Plane --------------------------------------------------------
void floor()
{
	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glColor4f(0.7, 0.7, 0.7, 1.0);  //The floor is gray in colour
	glNormal3f(0.0, 1.0, 0.0);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	//The floor is made up of several tiny squares on a 200x200 grid. Each square has a unit size.
	glBegin(GL_QUADS);
	for(int i = -200; i < 200; i++)
	{
		for(int j = -200;  j < 200; j++)
		{
			glVertex3f(i, 0.0, j);
			glVertex3f(i, 0.0, j+1);
			glVertex3f(i+1, 0.0, j+1);
			glVertex3f(i+1, 0.0, j);
		}
	}
	glEnd();
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}

//------- Rail Track ----------------------------------------------------
// A single circular track of specified radius
void track(float radius)  
{
	float angle1,angle2, ca1,sa1, ca2,sa2;
	float x1,z1, x2,z2, x3,z3, x4,z4;  //four points of a quad
    float toRad = 3.14159265/180.0;  //Conversion from degrees to radians

    glBegin(GL_QUADS);
	for(int i = 0; i < 360; i += 5)    //5 deg intervals
	{
		angle1 = i * toRad;       //Computation of angles, cos, sin etc
		angle2 = (i+5) * toRad;
		ca1=cos(angle1); ca2=cos(angle2);
		sa1=sin(angle1); sa2=sin(angle2);
		x1=(radius-0.5)*sa1; z1=(radius-0.5)*ca1; 
		x2=(radius+0.5)*sa1; z2=(radius+0.5)*ca1;
		x3=(radius+0.5)*sa2; z3=(radius+0.5)*ca2;
		x4=(radius-0.5)*sa2; z4=(radius-0.5)*ca2;

		glNormal3f(0., 1., 0.);       //Quad 1 facing up
		glVertex3f(x1, 1.0, z1);
		glVertex3f(x2, 1.0, z2);
		glVertex3f(x3, 1.0, z3);
		glVertex3f(x4, 1.0, z4); 

		glNormal3f(-sa1, 0.0, -ca1);   //Quad 2 facing inward
		glVertex3f(x1, 0.0, z1);
		glVertex3f(x1, 1.0, z1);
		glNormal3f(-sa2 ,0.0, -ca2);
		glVertex3f(x4, 1.0, z4);
		glVertex3f(x4, 0.0, z4);

		glNormal3f(sa1, 0.0, ca1);   //Quad 3 facing outward
		glVertex3f(x2, 1.0, z2);
		glVertex3f(x2, 0.0, z2);
		glNormal3f(sa2, 0.0, ca2);
		glVertex3f(x3, 0.0, z3);
		glVertex3f(x3, 1.0, z3);
	}
	glEnd();
}

//-------- Tracks  ----------------------------------------------------
void tracks()
{
	glColor4f(0.0, 0.0, 0.3, 1.0);
	track(115.0);   //Inner track has radius 115 units
	track(125.0);   //Outer track has radius 125 units
}

//------- Base of engine, wagons (including wheels) --------------------
void base()
{
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
      gluDisk(q, 0.0, 2.0, 20, 2);
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

//-- Locomotive ------------------------------------------------
void engine()
{
    base();

    //Cab
    glColor4f(0.8, 0.8, 0.0, 1.0);
    glPushMatrix();
      glTranslatef(7.0, 8.5, 0.0);
      glScalef(6.0, 7.0, 10.0);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(6.0, 14.0, 0.0);
      glScalef(4.0, 4.0, 8.0);
      glutSolidCube(1.0);
    glPopMatrix();

    //Boiler
    glPushMatrix();
      glColor4f(0.5, 0., 0., 1.0);
      glTranslatef(4.0, 10.0, 0.0);
      glRotatef(-90.0, 0., 1., 0.);
      gluCylinder(q, 5.0, 5.0, 14.0, 20, 5);
      glTranslatef(0.0, 0.0, 14.0);
      gluDisk(q, 0.0, 5.0, 20, 3);
      glColor4f(1.0, 1.0, 0.0, 1.0);
      glTranslatef(0.0, 4.0, 0.2);
      gluDisk(q, 0.0, 1.0, 20,2);  //headlight!
    glPopMatrix();

}

//--- A rail wagon ---------------------------------------------------
void wagon()
{
    base();

    glColor4f(0.0, 1.0, 1.0, 1.0);
    glPushMatrix();
      glTranslatef(0.0, 10.0, 0.0);
      glScalef(18.0, 10.0, 10.0);
      glutSolidCube(1.0);
    glPopMatrix();
    
 
}

//---------------------------------------------------------------------
void initialize(void) 
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

	q = gluNewQuadric();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);
    
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
    gluQuadricDrawStyle(q, GLU_FILL);
	glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1000.0);   //Perspective projection
}

//-------------------------------------------------------------------
void display(void)
{
   float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f};  //light0 position (directly above the origin)
   float spot_pos[]={-10, 14, 0, 1.0};
   float spot_dir[]={-10,-10,0,0};
   
   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   gluLookAt (-80, 50, 180, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position
   
  
   

   floor();
   tracks(); 
   
   glPushMatrix();
     glRotatef(theta,0,1,0);
     glTranslatef(0,1,-120);
     engine();
     glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
     glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
   glPopMatrix();
   
   
   for (int i=1;i<5;i++){
		glPushMatrix();
			glRotatef(-10.5*i+theta,0,1,0);
			glTranslatef(0,1,-120);
			wagon();
		glPopMatrix();  
   }  
     
   glutSwapBuffers();   //Useful for animation
}

void myTimer(int value)
{
	if (theta<360){
		theta++;
	}else{
		theta=0;
	}		
	glutPostRedisplay();
	glutTimerFunc(500, myTimer, 0);
	
}
//---------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("Toy Train");
   initialize ();
   glutTimerFunc(50, myTimer, 0);
   glutDisplayFunc(display); 
   glutMainLoop();
   return 0;
}
