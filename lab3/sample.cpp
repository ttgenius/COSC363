#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <fstream>
#include "loadTGA.h"

#define MAX_DROPS 10000000
#define GRAVITY -0.0005

using namespace std;
GLUquadric *q;
GLuint txId[2];   //Texture ids

int fire = 0;
int fly = 0;
int check = 0;
int b_move = -5;
int sp_move_y = 0;
int r_move = 0;
int cam_hgt = 150;
int NumDrops = 0;
int option = 0;
float theta = 0.0;
float angle_left_right = 0.0;
float angle_up_down = 1.0;
float lgt_pos[4] = {-30,50,-20,1};
float lgt_pos_1[4] = {30,50,20,0};
double drand48()
{	
    return (rand()%10000)/10000.0;
}
//------Waterfall System（Partical system *)------------------------------------------------------
typedef struct {
    int alive;
    float xpos, ypos;
    float xdir, ydir;
    float mass;
} Particle;

Particle water[MAX_DROPS];
void draw_waterfall(void)
{
    int i;
    glColor3f(0.0, 0.5, 1.0);
    glBegin(GL_POINTS);
    for ( i=0 ; i<NumDrops ; i++ ){
        if (water[i].alive) {
            glVertex2f(water[i].xpos, water[i].ypos);
        }
    }
    glEnd();
    glFlush();
}

void time_step(void)
{
    int i;
    for ( i=0 ; i<NumDrops ; i++ ) {
        if (water[i].alive) {           // set up an object to hit
            if (water[i].ypos + GRAVITY*water[i].mass < -0.75){ // bounce it off of the "floor"
                water[i].ydir = -water[i].ydir;
            } 
            else {            // let gravity do its thing
            water[i].ydir += GRAVITY * water[i].mass;
            }
        water[i].xpos += water[i].xdir;
        water[i].ypos += water[i].ydir;
        if (water[i].ypos < -1.0 || water[i].xpos > 1.0){
            water[i].alive = 0;}
        }
    }
}

void drop_generator(void)
{
    int i;
    float newdrops = drand48()*60;

    if (NumDrops + newdrops > MAX_DROPS){
        newdrops = MAX_DROPS - NumDrops;
    }
    
    for ( i=NumDrops ; i<NumDrops+newdrops ; i++ ) {
        water[i].alive = 1; 
        water[i].xpos = -0.8 + 0.01*drand48();
        water[i].ypos = 0.8 + 0.01*drand48();
        water[i].xdir = 0.0075 + 0.0025*drand48();
        water[i].ydir = 0.0;
        water[i].mass = 0.5 + 0.5*drand48();
    }
    NumDrops += newdrops;
}

void idle(void)
{
    glutPostRedisplay();
}

void waterfall_base(void)
{
    glPushMatrix();
        glColor4f(1.5, 0., 1., 0.);
        glTranslatef(0,35,0);        
        gluSphere(q, 5.3f, 32, 32);
        glTranslatef(0,-13,0);
        glScalef(5,25,5);
        glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
        glColor4f(1.5, 0., 1., 0.);
        glTranslatef(0.0, 35.0, 0.0);
        glRotatef(30.0, 0., 0., 1.);
        glRotatef(90.0, 0., 1., 0.);
        gluCylinder(q, 2.5, 2.5, 14.0, 20, 5);
        glTranslatef(0.0, 0.0, 14.0);
        gluDisk(q, 0.0, 2.5, 20.0, 3);
    glPopMatrix();
    
    glPushMatrix();     //waterfall
        glTranslatef(28,26,0);
        glScalef(20,20,20);
        drop_generator();
        draw_waterfall();
        time_step();
    glPopMatrix();
}
//---------Time for flying, 2nd timer for Robot-------------------------
void timer(int time)
{
	glutPostRedisplay();
    if(option == 1){
        b_move++;
    }
    else if(option == 2){
        sp_move_y++;
    }
    if (b_move == 100){
        fire = 0;
        b_move = -5;
    }
    else if (sp_move_y == 100){
        fly = 0;
    }
    else{
        glutTimerFunc(60, timer, 0);
    }
}

void robot_timer(int time)
{
	glutPostRedisplay();
	if ((theta < 30) && (check == 0)){
		theta ++;
	}
	if (theta == 30){
		check = 1;
	}
	if (check == 1){
		theta --;
	}
	if (theta == -30){
		check = 0;
	}
    fr_move ++;
	glutTimerFunc(20, robot_timer, 0);
}
//------ Keyborad funtion ----------------------------------------------
void special(int key, int x, int y)
{
	if(key == GLUT_KEY_UP){
		cam_hgt--;
	}
	else if(key == GLUT_KEY_DOWN){
		cam_hgt++;
	}
    else if(key == GLUT_KEY_RIGHT){
		angle_left_right--;
	}
	else if(key == GLUT_KEY_LEFT){
		angle_left_right++;
	}
	glutPostRedisplay();
}

void key(unsigned char key, int x, int y)
{
    if ((key == 'c') || (key == 'C')){
        fire = 1;
        option = 1;
        glutTimerFunc(20, timer, 0);
    }
    else if ((key == 's') || (key == 'S')){
        fly = 1;
        option = 2;
        glutTimerFunc(20, timer, 0);
    }
    else if ((key == 'x') || (key == 'X')){
        angle_up_down++;
    }
    else if ((key == 'z') || (key == 'Z')){    
        angle_up_down--;
    }
    glutPostRedisplay();
}
//------ Texture -------------------------------------------------------
void loadTexture(void)				
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

	glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture
    loadTGA("Wall1.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}
//------ Ground Plane --------------------------------------------------
void floor(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    glBegin(GL_QUADS);
    for(int i = -200; i < 200; i+=30)
    {
        for(int j = -200;  j < 200; j+=30)
        {
            glTexCoord2f(0.0,0.0);
            glVertex3f(i, 0.0, j);
            glTexCoord2f(0.0,1.0);
            glVertex3f(i, 0.0, j+30);
            glTexCoord2f(1.0,1.0);
            glVertex3f(i+30, 0.0, j+30);
            glTexCoord2f(1.0,0.0);
            glVertex3f(i+30, 0.0, j);
        }
    }  
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
//------ Castle Structure ----------------------------------------------
void cave(void)
{
    glPushMatrix();
        glColor4f(0.5, 0.5, 0.5, 1.0);
        glTranslatef(0,5,0);
        glScalef(5, 5, 5);
        glutSolidCube(1.1);
    glPopMatrix();
}

void wall(void)
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);
	glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 0.0);      //front
        glTexCoord2f(0.0,1.0);
        glVertex3f(0, 50.0, 0.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0, 50.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 0.0, 0.0);
            
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 80.0);      //back
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0, 50.0, 80.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0, 50.0, 80.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 0.0, 80.0);
             
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 0.0);      //left side
        glTexCoord2f(1.0,0.0);
        glVertex3f(0.0, 0.0, 80.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(0.0,50.0, 80.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0,50.0, 0.0);
        
        glTexCoord2f(0.0,0.0);
        glVertex3f(20.0, 0.0, 0.0);     //right side
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 0.0, 80.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0,50.0, 80.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(20.0,50.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[2]);
	glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex3i(0.0, 50.0, 0.0);      //Top
        glTexCoord2f(0.0,1.0);
        glVertex3i(0.0, 50.0, 80.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0, 50.0, 80.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 50.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(17,48,25);
        cave();
        glTranslatef(-14.3,0,0);
        cave();
        glTranslatef(0,0,20);
        cave();
        glTranslatef(14.3,0,0);
        cave();
        glTranslatef(0,0,20);
        cave();
        glTranslatef(-14.3,0,0);
        cave();
    glPopMatrix();
}

void castle_strut(void)
{
    glPushMatrix();
        glColor3f(10., 0., 0.);
        glTranslatef(45,50,-115);
        glRotatef(90,1,0,0);
        GLUquadric *q = gluNewQuadric();
        gluCylinder(q,10,10,50.,30.,30);
        glColor4f(1., 1., 0., 0.);
        glRotatef(180,1,0,0);
        glScalef(5, 5, 20);
        glutSolidCone(2,1,50,5);
        gluQuadricDrawStyle(q, GLU_FILL);
    glPopMatrix();
}
//------ Castle --------------------------------------------------------
void castle(void)
{
	glPushMatrix();
        glColor3f(0., 0. ,1.);
        glTranslatef(-30, 25.5, 0);
        glRotatef(0, 1, 0, 0);//two post distance
        glScalef(20, 50, 20);
        glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
        glColor3f(0., 1. ,0.);
        glTranslatef(-30, 50, 0);    //left post heave
        glRotatef(-90,1,0,0);
        glScalef(5, 5, 20);
        glutSolidCone(1.7,1,50,5);
	glPopMatrix();

	glPushMatrix();
        glColor3f(0., 1. ,0.);
        glTranslatef(30, 50, 0);    //left post cave
        glRotatef(-90,1,0,0);
        glScalef(5, 5, 20);
        glutSolidCone(1.7,1,50,5);
	glPopMatrix();
        
	glPushMatrix();
       	glColor3f(0., 0. ,1.);
        glTranslatef(30, 25.5, 0);
        glRotatef(0, 1, 0, 0); //two post distance
        glScalef(20, 50, 20);
        glutSolidCube(1);
	glPopMatrix();

    glPushMatrix();
        glColor3f(0.,0.,1.);
        glTranslatef(0,38,0);
        glRotatef(0,1,0,0);  //two post distance
        glScalef(60, 15, 20);
        glutSolidCube(1);    
    glPopMatrix();
    
    glPushMatrix();  
        glTranslatef(40,0,10);
        glRotatef(135,0,1,0);
        wall(); //right_down
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(111,0,-61);
        glRotatef(-135,0,1,0);
        wall(); //right_up
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(41, 0, -125);
        glRotatef(270,0,1,0);
        wall();
    glPopMatrix();
    
    glPushMatrix();
        castle_strut();
        glTranslatef(-90,0,-1);
        castle_strut();
    glPopMatrix();
    
    glPushMatrix();  
        glTranslatef(-25,0,-5);
        glRotatef(-135,0,1,0);
        wall(); //left_down
    glPopMatrix();
    
    glPushMatrix();  
        glTranslatef(-53,0,-118);
        glRotatef(-45,0,1,0);
        wall(); //left_up
    glPopMatrix();
}
//------ Structure of Spaceship ----------------------------------------
void spaceship(void)
{
    GLdouble eqn [4]={0.0,1.0,0.0,0.0}; 
    glPushMatrix();             //head
        glColor3f(0., 10., 100.0);
        glTranslatef(0,23,0);
        glClipPlane(GL_CLIP_PLANE0,eqn);
        glEnable(GL_CLIP_PLANE0);
        gluSphere(q, 5.3f, 32, 32);
        glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
    
    glPushMatrix();         //body
        glColor4f(0.5, 0.5, 0.5, 1.0);
        glTranslatef(0,23,0);
        glRotatef(90,1,0,0);
        GLUquadric *b = gluNewQuadric();
        gluCylinder(b,5.3,5.3,25.,30.,30);
        gluQuadricDrawStyle(q, GLU_FILL);
    glPopMatrix();
    
    glPushMatrix();             //engine
        glColor4f(1., 0., 0., 0.);
        glTranslatef(0,7,0);
        glRotatef(90,1,0,0);
        GLUquadric *q = gluNewQuadric();
        gluCylinder(q,2.5,7.5,12.,30.,30);
        gluQuadricDrawStyle(q, GLU_FILL);
    glPopMatrix();
}
//------ Build up a Robot ----------------------------------------------
void robot(int type)
{
   	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);	

    if (type == 1){
        glColor3f(1., 0.78 ,0.06);		//Head
        }
    else if (type == 0){
        glColor4f(0.2,0.2,0.2,1.0);
    }
	glPushMatrix();
        glTranslatef(0, 7.7, 0);
        glutSolidCube(1.4);
	glPopMatrix();
    
    if (type == 1){
        glColor3f(1., 0.0 ,0.0);		
    }
	glPushMatrix();             //eye (left & right)
        glTranslatef(-0.3, 8, 0.7);
        glutSolidCube(.2);
        glTranslatef(0.6, 0, 0);
        glutSolidCube(.2);
	glPopMatrix();
    
    glPushMatrix();         //mouth & noise
        glTranslatef(0, 7.6, 0.7);
        glutSolidCube(.2);
        glTranslatef(0, -0.3, 0);
        glScalef(3, .5, 1);
        glutSolidCube(.2);
	glPopMatrix();    
    
	glPushMatrix();     //Torso
        glTranslatef(0, 5.5, 0);
        glScalef(3, 3, 1.2);
        glutSolidCube(1);
	glPopMatrix();
    if (type == 1){
        glColor3f(0., 0. ,1.);
    }
	glPushMatrix();     //Right leg
        glTranslatef(0, 4.5, 0);
        glRotatef(-theta, 1, 0, 0);
        glTranslatef(-0.8, -2.5, 0);
        glScalef(1, 4, 1);
        glutSolidCube(1);
	glPopMatrix();
	
	glPushMatrix();     // Left leg
    	glTranslatef(0, 4.5, 0);
		glRotatef(theta, 1, 0, 0);
		glTranslatef(0.8, -2.5, 0);
		glScalef(1, 4, 1);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();     //Right arm
        glTranslatef(-2, 5, 0);
        glRotatef(theta, 1, 0, 0);
        glScalef(1, 4, 1);
        glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();     //Left arm
        glTranslatef(2, 5, 0);
        glRotatef(-theta, 1, 0, 0);
        glScalef(1, 4, 1);
        glutSolidCube(1);
	glPopMatrix();
	glFlush();
}
//------ Structure of Cannon -------------------------------------------
void cannon(void)
{
    glPushMatrix();
        glColor4f(0., 0., 0.5, 0.0);
        glTranslatef(6.0, 3.0, 0.0);
        glScalef(8.0, 7.0, 10.0);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(6.0, 10.0, 0.0);
        glScalef(8.0, 8.0, 8.0);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glColor4f(1.5, 0., 1., 0.);
        glTranslatef(2.0, 10.0, 0.0);
        glRotatef(-90.0, 0., 1., 0.);
        gluCylinder(q, 5.0, 5.0, 14.0, 20, 5);
        glTranslatef(0.0, 0.0, 14.0);
        gluDisk(q, 0.0, 5.0, 20, 3);
    glPopMatrix();
}
//------ fire ball -----------------------------------------------------
void bullet(void){
    glPushMatrix();
        glColor4f(1, 1., 1., 0.);
        gluSphere(q, 3.5f, 32, 32);
    glPopMatrix();
}
//------ Robot shadow & Robot Walking ----------------------------------
void robot_shadow(void)
{
    float shadowMatrix[16] = {lgt_pos[1],0,0,0,-lgt_pos[0],0,-lgt_pos[2],-1,0,0,lgt_pos[1],0,0,0,0,lgt_pos[1]};
    glDisable(GL_LIGHTING);
    glPushMatrix(); //Draw shadow object
        glMultMatrixf(shadowMatrix);
        glColor4f(0.2,0.2,0.2,1.0);//
        glTranslatef(0,0,20);
        glRotatef(90,0,1,0);
        glScalef(2,2,2);
        robot(0);   // 0 for gray shadow color
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glPushMatrix();
        glTranslatef(0,0,20);
        glRotatef(90,0,1,0);
        glScalef(2,2,2);
        robot(1);    // 1 for normal color
    glPopMatrix();
}

void robot_walking(void)
{
    glPushMatrix();
        glTranslatef(0,3,-50);
        glRotatef(-r_move-10.5,0,1,0);
        glTranslatef(0,0,-135);
        robot_shadow();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0,3,-50);
        glRotatef(-r_move-10.5*5,0,1,0);
        glTranslatef(0,0,-135);
        robot_shadow();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0,3,-50);
        glRotatef(-r_move-10.5*10,0,1,0);
        glTranslatef(0,0,-135);
        robot_shadow();
    glPopMatrix();
}
//------ Initialize ----------------------------------------------------
void initialize(void) 
{
    float grey[4] = {0.2, 0.2, 0.2, 1.}; //light’s ambient color
    float white[4] = {1.0, 1.0, 1.0, 1.}; // diffuse and spec color
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    
    q = gluNewQuadric();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
    gluQuadricDrawStyle(q, GLU_FILL);
	glClearColor (0.0, 0.0, 0.0, 0.0);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1000.0);     //Perspective projection
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
    loadTexture();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(0., 0., 1., 1.);
}
//-------------------------------------------------------------------
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (0, 20, cam_hgt, .0, 1.0, 0.0, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);       //First Light's position
    glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos_1);       //Second Light's position
    glRotatef(angle_left_right, 0.0, 1.0, 0.0);
    glRotatef(angle_up_down, 1.0, 0.0, 0.0);
    floor();
    castle();
    robot_walking();
    
    glPushMatrix();
        glTranslatef(10, 0, 5);
        glRotatef(90,0,1,0);
        cannon();
    glPopMatrix();
    
    if (fire == 1){
        glPushMatrix();
            glTranslatef(10.0, 10.0, b_move);
            bullet();
        glPopMatrix();
    }
    glPushMatrix();
        glTranslatef(0, 5, -20);
        if (fly == 1){
            glTranslatef(0, sp_move_y, 0);
        }
        glTranslatef(0, sp_move_y, 0);
        spaceship();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(30,-10,-60);
        waterfall_base();
    glPopMatrix();
    
    glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);
    glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos_1);
    glutSwapBuffers();
}
//---------------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
    glutInitWindowSize (600, 600); 
    glutInitWindowPosition (50, 50);
    glutCreateWindow ("Made by Liguo Jiao");
    initialize ();
    glutTimerFunc(20, robot_timer, 0);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    glutDisplayFunc(display); 
    glutIdleFunc(idle);
    glutMainLoop();
return 0;
}
/*void skybox() {
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
		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glPopMatrix();
	glDisable(GL_BLEND);
*/	
