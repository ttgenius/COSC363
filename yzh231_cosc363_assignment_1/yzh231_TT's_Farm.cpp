//  ====================================================================
//  COSC363: Assignment 1 TT's Farm
//  Date:06/04/2017
//  Author: Melody Zhu 
//  ID: 97245310 
//  ====================================================================
 
#include <iostream>
#include <fstream>
#include <cmath> 
#include <GL/freeglut.h>
#include "loadBMP.h"
#include "loadTGA.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"

#define	MAX_PARTICLES 1000

using namespace std;

//--Globals ------------------------------------------------------------
GLuint txId[9];  //Texture ID
GLUquadric *q;
const int N = 25;  // Total number of vertices on the base curve
int cam_hgt = 90; //Camera height
float angle = 10.0;  //Rotation angle for viewing
float angle2=0;     //robot's self rotating angle
float t=0;          //time for robot_walking
float a=105;       // walking r parameter
float theta = 20.0;  //walking,leg,arm,and tails'rotating angle
float theta2 = 35;  //watering angle
int direction=1;
float jump=0;
float pig_angle=0;  //pig's self rotating angle
float tree_angle = 7.2;
float lpos[4] = {20,90,10,1};    //light0's position
float spot_pos[]={-10, 14, 0, 1.0}; //light1's position (spot light)
float spot_dir[]={-10,-10,0,0};     //spot light direction
float  eye_x = 2,  eye_y = 20,  eye_z = 100;    //Initial camera position
float look_x = 2, look_y = 20, look_z = 80;    //"Look-at" point along -z direction
int step = 0;		//camera motion
float s=1;         //camera motion factor
float d=2;
float cam_angle=0;
int move_mode = 1; //objects move in default
int cam_mode=0;  //default is general

//points on the sweep surface 'Fountain Base'
float vx[N] = {0,4,3.8,3.5,3,2.5,2,1.5,1,0.9,0.9,1,1.2,1.5,2,2.5,3,3.5,4,4.5,5,5.2,5.5,6,6.5};
float vy[N] = {0,0,0.2,0.4,0.5,0.7,0.8,0.9,1.1,1.5,2.5,3,3.2,3.5,3.75,3.8,3.83,3.89,3.9,3.95,4,4.2,4.5,5,6};
float vz[N] = {0};

//For particles
float slow_facor=3000.;			// slow down particles' speed
float y_base=1.;		  // min y speed
float fade_base=0.0005;   //min fade speed

//--------------Structure For Particle---------------------------------
typedef struct						
{
	float life;					// Particle's lifespan, max is 1
	float fade;					// fade speed
	float x;					// x position
	float y;					// y position
	float z;					// z position
	float xdir;					// speed in x direction
	float ydir;					// speed in y direction
	float zdir;					// speed in z direction
}Particles;					


Particles particle[MAX_PARTICLES];	// Particle Array hoding particles' info

/* initialize particles */
void init_particle(){
 for (int i=0;i<MAX_PARTICLES;i++)				
	{
		particle[i].life=1.0;					
		particle[i].fade=float(rand()%100)/1000 + fade_base; // random Fade Speed
		particle[i].xdir=float((rand()%50)-25.0);	
		particle[i].ydir=float((rand()%80)-25.0)+ y_base;		
		particle[i].zdir=float((rand()%50)-25.0);
		
	}
}

/* update particles' postitions and draw*/
void update_position(){   
	glColor3f(0, 0.5, 1);
	for (int i = 0;i < MAX_PARTICLES; i++)		
	{
		if (particle[i].life > 0)
		{
			glTranslatef(particle[i].x,particle[i].y,particle[i].z);
			gluSphere(q,0.01,4,4);  //particle object
			glTranslatef(-particle[i].x,-particle[i].y,-particle[i].z);
			
			particle[i].x += particle[i].xdir/slow_facor;	
			particle[i].y += particle[i].ydir/slow_facor;	
			particle[i].z += particle[i].zdir/slow_facor;	
			particle[i].life -= particle[i].fade;		// reduce particles' life by a random fade speed
			if (particle[i].y<0 || particle[i].y>1|| particle[i].life==0) //if Particle is out of the domain or dead
			{
				particle[i].life=1.0;	// particle reborn
				particle[i].fade=float(rand()%100)/1000.0 + fade_base;
				particle[i].x=0.0;	//re-position the particle
				particle[i].y=0.1;					
				particle[i].z=0.0;					
				particle[i].xdir=float((rand()%60)-29.0);	
				particle[i].ydir=float((rand()%100)-29.0) + y_base;	
				particle[i].zdir=float((rand()%60)-29.0);				
	
			}
		}
	}
}

//-----------Load Textures----------------------------------------------
void loadTexture(void)				
{
	glGenTextures(9, txId); 	// Create texture ids
	
	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadBMP("ground.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadBMP("roof0.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture
    loadBMP("wall.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
    
    glBindTexture(GL_TEXTURE_2D, txId[3]);  //Use this texture
    loadTGA("organic_bk.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, txId[4]);  //Use this texture
    loadTGA("organic_rt.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, txId[5]);  //Use this texture
    loadTGA("organic_ft.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
	glBindTexture(GL_TEXTURE_2D, txId[6]);  //Use this texture
    loadTGA("organic_lf.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
	glBindTexture(GL_TEXTURE_2D, txId[7]);  //Use this texture
    loadTGA("organic_up.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
	glBindTexture(GL_TEXTURE_2D, txId[9]);  //Use this texture
    loadBMP("VaseTexture.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

//--Draws a grid of lines on the floor plane -------------------------------
void drawFloor()
{
	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);

    //glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBegin(GL_QUADS);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
    for(int i = -256; i < 256; i+=16)
    {
        for(int j = -256;  j < 256; j += 16)
        {
            glTexCoord2f(0.0,0.0);
            glVertex3f(i, -0.1, j); //-0.1 so that the shadow can be seen clearly
            glTexCoord2f(0.0,1.0);
            glVertex3f(i, -0.1, j+16);
            glTexCoord2f(1.0,1.0);
            glVertex3f(i+16, -0.1, j+16);
            glTexCoord2f(1.0,0.0);
            glVertex3f(i+16, -0.1, j);
        }
    }  
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}

//--Draws models constructed using GLUT objects ------------------
void drawRobot(int color)
{   
	if (color == 1){                     //1 is colorful
        glColor3f(1., 0.78 ,0.06);		//Head
    }
    else if (color == 0){              //0 is shawdow color
        glColor4f(0.2,0.2,0.2,1.0);
    }
    if (color == 1){
	glColor3f(1.,0.76, 0.06);
	}		
	glPushMatrix();						//Head
	  glTranslatef(0, 7.7, 0);
	  glutSolidSphere(1,20,20);
	glPopMatrix();
	
	 if (color == 1){
	 glColor3f(0,0,0);
	}		      
    glPushMatrix();                  //eye      
        glTranslatef(0.4, 8, 0.7);
        glutSolidSphere(0.2,20,20);
	glPopMatrix();
	      
    glPushMatrix();                  //eye       
        glTranslatef(-0.4, 8, 0.7);
        glutSolidSphere(0.2,20,20);
	glPopMatrix();    
	
		
	glPushMatrix();                 //mouth
       glTranslatef(0, 7.55, 0.7);
        glScalef(3, 0.4, 1.8);
        glutSolidSphere(0.2,20,20);
	glPopMatrix();    
	
    if (color == 1){				
	glColor3f(1, 1, 0);}
	glPushMatrix();
	  glTranslatef(0, 5.5, 0.75); 
   	  gluDisk(q, 0.0, 1.0, 20,2);
   glPopMatrix();    
   	
   	  
   	if (color == 1){				
	glColor3f(0.8, 1, 1);
	}
				  
   	glPushMatrix();					//Torso
	  glTranslatef(0, 5.5, 0);
	  glScalef(3, 3, 1.4);
	  glutSolidCube(1);
	  
   	glPopMatrix();;
	  
	glPushMatrix();					//Right leg
		glTranslatef(-0.8,4,0);
		glRotatef(-theta, 1, 0, 0);
		glTranslatef(0.8, -4, 0);
	    glTranslatef(-0.8, 2.2, 0);
	    glScalef(1, 4.4, 1);
	    glutSolidCube(1);
	glPopMatrix();

         
	glPushMatrix(); 				//Left leg  
		glTranslatef(-0.8, 4, 0);
		glRotatef(theta, 1, 0, 0);
		glTranslatef(0.8, -4, 0);
		glTranslatef(0.8, 2.2, 0);
	    glScalef(1, 4.4, 1);
		glutSolidCube(1);
	glPopMatrix();

									
	glPushMatrix();					//Right arm
		glTranslatef(-2, 6.5, 0);
		glRotatef(theta, 1, 0, 0);
		glTranslatef(2, -6.5, 0);
		glTranslatef(-2, 5, 0);
		glScalef(1, 4, 1);
		glutSolidCube(1);
	glPopMatrix();

			
	glPushMatrix();					//Left arm
		glTranslatef(2,6.5,0);
		glRotatef(-theta, 1, 0, 0);
		glTranslatef(-2, -6.5, 0);
		glTranslatef(2, 5, 0);
		glScalef(1, 4, 1);
		glutSolidCube(1);
	glPopMatrix();
	
}
	
void drawRobotShadow(){
	// Light0 source position lpos
    float shadowMat[16] = {lpos[1],0,0,0,-lpos[0],0,-lpos[2],-1,0,0,lpos[1],0,0,0,0,lpos[1]};
   
    //int b=40;

	// Draw object
	glEnable(GL_LIGHTING);
    glPushMatrix();
        	glTranslatef(a*cos(t)/(1+sin(t)*sin(t)),0,a*sin(t)*cos(t)/(1+sin(t)*sin(t)));  // walk in a Lemniscate curve
        	glTranslatef(0,0,15);
        	glRotatef(angle2,0,1,0);
        	glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
        	drawRobot(1);
     glPopMatrix();    	      
   
    
     //Draw shadow
    glDisable(GL_LIGHTING);

    glPushMatrix();
        glMultMatrixf(shadowMat);
        	glTranslatef(a*cos(t)/(1+sin(t)*sin(t)),0,a*sin(t)*cos(t)/(1+sin(t)*sin(t)));
        	glTranslatef(0,0,15);
			glRotatef(angle2,0,1,0);
        drawRobot(0);
    glPopMatrix();
}
    

void drawTree()
{
	
    glPushMatrix();
      glColor4f(0.5, 0., 0., 1.0); //base
      glTranslatef(0,2,0);
      glScalef(1,4,1);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();               //green 1    
	  glColor4f(0., 1., 0., 1.0);
      glTranslatef(0,4,0);
      glRotatef(-90,1,0,0);
      glutSolidCone(3,2,20,20);
    glPopMatrix(); 
    
   glPushMatrix();               //green 2   
	  glColor4f(0., 1., 0., 1.0);
      glTranslatef(0,6,0);
      glRotatef(-90,1,0,0);
      glutSolidCone(3,2,20,20);
    glPopMatrix(); 

 
}


void drawPig()
{
	glColor3f(1, 0.7529, 0.7961);
	glPushMatrix();                          //body
	 glColor3f(1, 0.7529, 0.7961);
      glTranslatef(30.0, 4, 0.0);
      glRotatef(-90.0, 0., 1., 0.);
      gluCylinder(q, 2.0, 2.0, 4, 20, 5);
      glTranslatef(0.0, 0.0, 4);
      gluDisk(q, 0.0, 2.0, 20, 4);
      glTranslatef(0.0, 0.0, -4);
      gluDisk(q, 0.0, 2.0, 20, 4);
    glPopMatrix();
    
    
    glPushMatrix();
     glTranslatef(25, 4, 0.0);                  //head
     glutSolidSphere(1.5,20,20);
    glPopMatrix(); 
    
    glPushMatrix();
      glPushMatrix();                          //legs
	  glTranslatef(27, 1, 0.5);
      glScalef(0.5,2,0.5);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
      glPushMatrix();                          //legs
	  glColor3f(1, 0.7529, 0.7961);
	  glTranslatef(27, 1, -0.5);
      glScalef(0.5,2,0.5);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
      glPushMatrix();                          //legs
	  glTranslatef(29.5, 1, 0.5);
      glScalef(0.5,2,0.5);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
      glPushMatrix();                          //legs
	  glTranslatef(29.5, 1, -0.5);
      glScalef(0.5,2,0.5);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
      glPushMatrix();                          //tail
	  glTranslatef(30, 4, 0);
	  glRotatef(theta,0,1,0);
	  glRotatef(-90,0,0,1);
      glScalef(0.5,3,0.5);
      glutSolidCube(1);
    glPopMatrix();
    
}


void drawSheep(){
	glColor3f(1, 1, 1);
	
	glPushMatrix();                          //body
	  glTranslatef(-26, 4, 0.0);
      glRotatef(-90.0, 0., 1., 0.);
      gluCylinder(q, 2.0, 2.0, 4, 20, 5);
      glTranslatef(0.0, 0.0, 4);
      gluDisk(q, 0.0, 2.0, 20, 4);
      glTranslatef(0.0, 0.0, -4);
      gluDisk(q, 0.0, 2.0, 20, 4);
    glPopMatrix();
    

    
    glPushMatrix();
     glTranslatef(-25, 5, 0.0);
     glutSolidCube(2);                         //head                   
	glPopMatrix();
	
    
    glPushMatrix();
      glPushMatrix();                          //legs
	  glTranslatef(-27, 1, 0.5);
      glScalef(0.5,2,0.5);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
      glPushMatrix();                          //legs
	  glTranslatef(-27, 1, -0.5);
      glScalef(0.5,2,0.5);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
      glPushMatrix();                          //leg
	  glTranslatef(-29.5, 1, 0.5);
      glScalef(0.5,2,0.5);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
      glPushMatrix();                          //leg
	  glTranslatef(-29.5, 1, -0.5);
      glScalef(0.5,2,0.5);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
      glPushMatrix();                          //tail
	  glTranslatef(-30, 4, 0);
	  glRotatef(theta,0,1,0);
	  glRotatef(-90,0,0,1);
      glScalef(0.5,3,0.5);
      glutSolidCube(1);
    glPopMatrix();
 

}


//------------Compute normal for sweep surface--------------------------
void normal(float x1, float y1, float z1, 
            float x2, float y2, float z2,
		      float x3, float y3, float z3 )
{
	  float nx, ny, nz;
	  nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
	  ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
	  nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}

/* Fountain, a custom-built sweep surface */
void drawFountain(){
	float wx[N], wy[N], wz[N]; 
	float angStep = -10.0*3.1415926/180.0;  //Rotate in 10 deg steps (converted to radians)
	glColor4f (1.0, 0.75, 0.5, 1.0); 
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[9]);
	for (int j =0;j< 36;j++){
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < N; i++)
		{
			wx[i] = vx[i] * cos(angStep) +  vz[i] * sin(angStep);
			wy[i] = vy[i];
			wz[i] = -vx[i] * sin(angStep) + vz[i] * cos(angStep);
			if(i > 0) normal(wx[i-1], wy[i-1], wz[i-1], 
							vx[i-1], vy[i-1], vz[i-1],
							vx[i], vy[i], vz[i]);
			glTexCoord2f((float)j/36.0,(float)i/N); glVertex3f(vx[i], vy[i], vz[i]);
			if(i > 0) normal(wx[i-1], wy[i-1], wz[i-1],
							vx[i], vy[i], vz[i],
							wx[i], wy[i], wz[i] );
			glTexCoord2f((float)(j+1)/36.0,(float)i/N);glVertex3f(wx[i], wy[i], wz[i]);
		}
        glEnd();		
 
		for (int i = 0; i<N; i++){
			vx[i] = wx[i];
			vy[i] = wy[i];
			vz[i] = wz[i];
		}
	}
	glDisable(GL_TEXTURE_2D);
}	

	
//Cottage, a custom_buil model using coordinates and polygon definitions
void drawRoof(){

	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);
	glColor3f(1, 1, 1);
	
	glBegin(GL_TRIANGLES);
	
	glNormal3f(0, 1, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-50, 30, 20);
	glTexCoord2f(5, 0);
	glVertex3f(50, 30, 20);
	glTexCoord2f(2.5, 5);
	glVertex3f(0, 60, 0);

	glNormal3f(1, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(50, 30, 20);
	glTexCoord2f(5, 0);
	glVertex3f(50, 30, -20);
	glTexCoord2f(2.5, 5);
	glVertex3f(0, 60, 0);
	
	glNormal3f(0, 1, -1);
	glTexCoord2f(0, 0);
	glVertex3f(50, 30, -20);
	glTexCoord2f(5, 0);
	glVertex3f(-50, 30, -20);
	glTexCoord2f(2.5, 5);
	glVertex3f(0, 60, 0);

	glNormal3f(-1, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-50, 30, -20);
	glTexCoord2f(5, 0);
	glVertex3f(-50, 30, 20);
	glTexCoord2f(2.5, 5);
	glVertex3f(0, 60, 0);
	
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawWalls(){
	
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[2]);
	glColor3f(1, 1, 1);
	
	glBegin(GL_QUADS);
	
	glNormal3f(0, 0, 1);                  //front side left of the door
	glTexCoord2f(0, 0);
	glVertex3f(-50, 0, 20);
	glTexCoord2f(1, 0);
	glVertex3f(-5, 0, 20);
	glTexCoord2f(1, 1);
	glVertex3f(-5, 30, 20);
	glTexCoord2f(0, 1);
	glVertex3f(-50, 30, 20);
	
	glNormal3f(0, 0, 1);                 //above the door
	glTexCoord2f(0, 0);
	glVertex3f(-5, 15, 20);
	glTexCoord2f(0.22, 0);
	glVertex3f(5, 15, 20);
	glTexCoord2f(0.22, 0.5);
	glVertex3f(5, 30, 20);
	glTexCoord2f(0, 0.5);
	glVertex3f(-5, 30, 20);
	
	glNormal3f(0, 0, 1);                  //right of the door
	glTexCoord2f(0, 0);
	glVertex3f(5, 0, 20);
	glTexCoord2f(1, 0);
	glVertex3f(50, 0, 20);
	glTexCoord2f(1, 1);
	glVertex3f(50, 30, 20);
	glTexCoord2f(0, 1);
	glVertex3f(5, 30, 20);

	glNormal3f(1, 0, 0);                   //right side
	glTexCoord2f(0, 0);
	glVertex3f(50, 0, 20);
	glTexCoord2f(1, 0);
	glVertex3f(50, 0, -20);
	glTexCoord2f(1, 1);
	glVertex3f(50, 30, -20);
	glTexCoord2f(0, 1);
	glVertex3f(50, 30, 20);
	
	glNormal3f(0, 0, -1);                  //backside left of the door
	glTexCoord2f(0, 0);
	glVertex3f(-50, 0, -20);
	glTexCoord2f(1, 0);
	glVertex3f(-5, 0, -20);
	glTexCoord2f(1, 1);
	glVertex3f(-5, 30, -20);
	glTexCoord2f(0, 1);
	glVertex3f(-50, 30, -20);
	
	glNormal3f(0, 0, -1);                 //above the door
	glTexCoord2f(0, 0);
	glVertex3f(-5, 15, -20);
	glTexCoord2f(1, 0);
	glVertex3f(5, 15, -20);
	glTexCoord2f(1, 1);
	glVertex3f(5, 30, -20);
	glTexCoord2f(0, 1);
	glVertex3f(-5, 30, -20);
	
	glNormal3f(0, 0, -1);                  //right of the door
	glTexCoord2f(0, 0);
	glVertex3f(5, 0, -20);
	glTexCoord2f(1, 0);
	glVertex3f(50, 0, -20);
	glTexCoord2f(1, 1);
	glVertex3f(50, 30, -20);
	glTexCoord2f(0, 1);
	glVertex3f(5, 30, -20);
	
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-50, 0, 20);
	glTexCoord2f(1, 0);
	glVertex3f(-50, 0, -20);
	glTexCoord2f(1, 1);
	glVertex3f(-50, 30, -20);
	glTexCoord2f(0, 1);
	glVertex3f(-50, 30, 20);
	
	glEnd();
	glDisable(GL_TEXTURE_2D);
}	

void drawHouse(){
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		drawRoof();
		drawWalls();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void skybox() {
	glEnable(GL_BLEND);
	glColor4f(1.0, 1.0, 1.0, 0.9);
	glPushMatrix();
		float white[4] = {1.0, 1.0, 1.0, 1.0};
		float black[4] = {0.0, 0.0, 0.0, 1.0};
		glMaterialfv(GL_FRONT, GL_SPECULAR, black);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glEnable(GL_TEXTURE_2D);
		// Back
		glBindTexture(GL_TEXTURE_2D, txId[3]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f(-256.0, -256.0, -256.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f( 256.0, -256.0, -256.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f( 256.0,  256.0, -256.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(-256.0,  256.0, -256.0);
		glEnd();
		// Right
		glBindTexture(GL_TEXTURE_2D, txId[4]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f(256.0, -256.0, -256.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f(256.0, -256.0,  256.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(256.0,  256.0,  256.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(256.0,  256.0, -256.0);
		glEnd();
		// Front
		glBindTexture(GL_TEXTURE_2D, txId[5]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f( 256.0, -256.0, 256.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f(-256.0, -256.0, 256.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(-256.0,  256.0, 256.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f( 256.0,  256.0, 256.0);
		glEnd();
		// Left
		glBindTexture(GL_TEXTURE_2D, txId[6]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f(-256.0, -256.0,  256.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f(-256.0, -256.0, -256.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(-256.0,  256.0, -256.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(-256.0,  256.0,  256.0);
		glEnd();
		// Top
		glBindTexture(GL_TEXTURE_2D, txId[7]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);		glVertex3f( 256.0,  256.0, -256.0);
			glTexCoord2f(1.0, 0.0);		glVertex3f( 256.0,  256.0,  256.0);
			glTexCoord2f(1.0, 1.0);		glVertex3f(-256.0,  256.0,  256.0);
			glTexCoord2f(0.0, 1.0);		glVertex3f(-256.0,  256.0, -256.0);
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glPopMatrix();
	glDisable(GL_BLEND);
	
}

//--Display: ---------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.

void display()  
{   
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glMatrixMode(GL_MODELVIEW);								
	glLoadIdentity();
	
	// first-person view
	if (cam_mode==1){
		    glTranslatef(0,-10,0); 
            glRotatef(-angle2,0,-1,0);
            glTranslatef(0,0,-15);
        	glTranslatef(-a*cos(t)/(1+sin(t)*sin(t)),0,-a*sin(t)*cos(t)/(1+sin(t)*sin(t))); 
    } 
       	    	
	else if(cam_mode == 0){
	//General Camera
	float dir_x = -sin(cam_angle), dir_y = 0,  dir_z = -cos(cam_angle);

	eye_x += step * s * dir_x;
	eye_z += step * s * dir_z;
	step = 0;
     
    if (eye_x >=230){
		eye_x = 230;
	}	
	else if(eye_x <=-230){
		eye_x = -230;	
	}
	if (eye_z >= 230){
		eye_z = 230;
	}	
	else if (eye_z <=-230){
		eye_z = -230;
	}
				 
    look_x = eye_x+d*dir_x;
	look_y = eye_y+d*dir_y;
	look_z = eye_z+d*dir_z; 

   
   
    gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z, 0, 1, 0);  	
   }      
	

	
	glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position


	glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
    drawFloor();
    skybox();
    
    drawRobotShadow(); //draw robot and its shadow
    
    glEnable(GL_LIGHTING);			//Enable lighting when drawing the model
     for (int i=0; i<=50;i++){
	    glPushMatrix();
	        glRotatef(-tree_angle*i,0,1,0);   // draw the surrounding trees
			glTranslatef(10,0,-150);
            drawTree();
  
        glPopMatrix(); 
     }
     
     for (int i=0; i<=3;i++){               //pinky pigs
		 for (int j=0;j<=3;j++){
			glPushMatrix();
			glTranslatef(10*i,0,10*j);
			glTranslatef(30,4,0);
			glRotatef(-pig_angle,0,1,0);
			glTranslatef(-30,-4,0);
            drawPig();
            glPopMatrix();
         }
	}
	 
    for (int i=0; i<=3;i++){                //whitie sheeps  
		 for (int j=0;j<=3;j++){
			glPushMatrix();
			glTranslatef(-10*i,jump,10*j);
            drawSheep();
            glPopMatrix();
      }
    }
   
    glPushMatrix();
		glTranslatef(58,5,-75); // push water particles to the centre of fountain
        glScalef(20,10,20);
        update_position();
        init_particle();
    glPopMatrix();
    
    glPushMatrix();              //fountain
      glTranslatef(58,0,-75);
      drawFountain();
    glPopMatrix(); 
    

    glPushMatrix();              //cottage
      glTranslatef(0,0,-100);
	  drawHouse();
	glPopMatrix(); 
	
	glutSwapBuffers(); 

}

//------------ Special key event callback ------------------------------
void special(int key, int x, int y)
{
	step = 0;
	if(key == GLUT_KEY_LEFT){
		cam_angle += 0.1;   //in radians
	}else if(key == GLUT_KEY_RIGHT){
		 cam_angle -= 0.1;
	}else if(key == GLUT_KEY_DOWN){
		step = -2;
	}else if(key == GLUT_KEY_UP){
		step = 2;
	}
	
	glutPostRedisplay();
}
//------------  keyboard event callback ---------------------------------
void key(unsigned char key, int x, int y)
{
	
	if(key == 'p'|| key == 'P'){
		move_mode = 0;
	}
    if(key == 's'|| key == 'S'){
		move_mode = 1;
	}
	if(key == 'F'|| key =='f'){
		cam_mode = 1;
	}	
	if(key == 'G' || key == 'g'){
		cam_mode = 0;
	}			
    glutPostRedisplay();
}



//-----Timers-----------------------------------------------------------
void main_timer(int value)
{   
   if (move_mode==1){	
	if (theta==20){
		direction=-1;
	}
	if (theta==-20){
		direction=1;
	}
	theta+=direction;
	
	t+=0.01;
	angle2-=3*(atan(sin(t))-atan(sin(t-0.4))); //self-rotating
   }
	glutTimerFunc(20,main_timer, 0);
	glutPostRedisplay();
	
}


void animal_timer(int value)
{
  if (move_mode == 1){ 
    if (pig_angle<360){
		pig_angle++;
	}else{
		pig_angle=0;
	}		
	
	if (jump==100){
		direction=-1;
	}
	if (jump==0){
		direction=1;
	}
	jump+= direction;
  }
    glutTimerFunc(100,animal_timer, 0);
	glutPostRedisplay();
   
	
}	

//------- Initialize OpenGL parameters ---------------------------------
void initialize()
{
	float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    q = gluNewQuadric();
    loadTexture();
	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    
	glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);  //specular reflection
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	
    
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
    gluQuadricDrawStyle(q, GLU_FILL);
    glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 1000.0);   //Camera Frustum

}


//------- Main: Initialize glut window and register call backs ---------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Humanoid");
   initialize();
   glutTimerFunc(20, main_timer, 0);
   glutTimerFunc(80,animal_timer, 0);
   glutDisplayFunc(display);
   glutSpecialFunc(special); 
   glutKeyboardFunc(key);
   glutMainLoop();
   return 0;
}
