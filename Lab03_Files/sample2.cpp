//  ========================================================================
//  AUTHOR:     Carl Kenny
//  FILE NAME:  csk29.cpp
//  DATE:       02/04/2015
//  ========================================================================

#include <cstdlib>
#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <math.h>
#include <random>
#include <string>
#include <GL/freeglut.h>
#include "loadTGA.h" // load textures
#include <stdlib.h>

#define GL_CLAMP_TO_EDGE 0x812F     // To get rid of seams between textures

// Required opengl globals
GLUquadric *q;      // Required for creating cylindrical objects
GLuint txId[16];    // textures

float theta = 0;    // Angles in degrees
int FLOOR = 1000;   // Size of canvas
int window_id = 0;  // assigned in main(), used to close window
string USAGE_MESSAGE = "N (where N is the count of desired wagons)\n";

// Camera
float camera_pov[] = {0, 14.0, 200, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0};
float camera_pov_reset[] = {0, 14.0, 200, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0};
float cam_angle_x = 90.0; 
float cam_angle_y = 30.0; 
float dist_cam = 150.0;
float pan_radius = 800;
float pan_height = 70;
float pan_angle = 0.0;
bool FPS_MODE = true;

// wagons
float WAGON_WIDTH = 10;
int WAGONS = 1;         // Can also be set in main(), via argv[1]

// tunnels
float TUNNEL_OFFSET_X = 40;

// path
float PATH_WIDTH = 50;

// traffic light
bool STOP_LIGHT = true;

// tracks
int TRACK_LENGTH = 800;
float TRACK_OFFSET_X = 00.0;
float TRACK_OFFSET_Y = 0.0;
float TRACK_OFFSET_Z = 180.0;

// trains (make one train, copy and translate it 
float train0_location_x = 0.0;
float train0_location_z = TRACK_OFFSET_Z + (WAGON_WIDTH/2.0);     // half wagon
float train0_speed = 3;
bool TRAIN0_DECREASING = false;

float train1_location_x = 0.0;
float train1_location_z = -TRACK_OFFSET_Z + (WAGON_WIDTH/2.0);    // half wagon
float train1_speed = 3;
bool TRAIN1_DECREASING = false;

// Mouse
int previous_x = 0;
int previous_y = 0;

// init window dimensions, laptop friendly
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

// keyboard buffer 
bool keyboard_down[265] = {false};
int keyboard_down_count = 0;
unsigned char last_char = '\0';

// humanoids
int MODEL_COUNT = 10;
float RAND_HI = 40;
float RAND_LOW = 0.0;
float RAND_MID = (RAND_HI + RAND_LOW) / 2;
float humanoid_theta = 20;          // Degrees
float man_pos;
float random_float;
bool rising_theta = true;
bool rising_walk = true;

// barrier logic
float POS_Z_CUTOFF = TRACK_OFFSET_Z + 30;
float NEG_Z_CUTOFF = -TRACK_OFFSET_Z - 30;
float BARRIER_THETA = 1;

bool BARRIER_LOWERING = false;
float POS_Z_CUTOFF_BARRIER = TRACK_OFFSET_Z - 10;
float NEG_Z_CUTOFF_BARRIER = -TRACK_OFFSET_Z + 15;

// barrier asthetics
float BARRIER_R = 1.0;
float BARRIER_G = 1.0;
float BARRIER_B = 1.0;

void loadGLTextures()               // Load bitmaps And Convert To Textures
{
    glGenTextures(16, txId);        // Create texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]);
    //loadTGA("textures/grass1.tga");
    loadTGA("grass1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeat grass

    glBindTexture(GL_TEXTURE_2D, txId[1]);
    loadTGA("metal.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeat metal 

    glBindTexture(GL_TEXTURE_2D, txId[2]);
    loadTGA("barrier_body.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeat stone

    glBindTexture(GL_TEXTURE_2D, txId[3]);
    loadTGA("bricks.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeat brick 

    /*
    glBindTexture(GL_TEXTURE_2D, txId[4]);
    loadTGA("cobble.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeat cobble
    */

    glBindTexture(GL_TEXTURE_2D, txId[5]);
    loadTGA("stone.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeat stone
}



//-- Ground Plane --------------------------------------------------------
void floor()
{
	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glColor4f(0.1, 0.1, 0.1, 1.0);
	glNormal3f(0.0, 1.0, 0.0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);  //No specular reflections from the floor.

	//The floor is made up of several largish squares on a GLOBALLY DEFINED grid. Each square has a unit size.
	glBegin(GL_QUADS);
	for(int i = -FLOOR; i < FLOOR; i += 10)
	{
		for(int j = -FLOOR;  j < FLOOR; j += 10)
		{
			glVertex3f(i, 0.0, j);
			glVertex3f(i, 0.0, j+10);
			glVertex3f(i+10, 0.0, j+10);
			glVertex3f(i+10, 0.0, j);
		}
	}
    glDisable(GL_TEXTURE_2D);                   // Allow for other textures to be applied
	glEnd();

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);  //Enable specular reflections for remaining objects
}

//------- Rail Track ----------------------------------------------------
// A single circular track of specified radius
void track(float radius)  
{
	float angle1,angle2, ca1,sa1, ca2,sa2;
	float x1,z1, x2,z2, x3,z3, x4,z4;  //four points of a quad
    float toRad=3.14159265/180.0;  //Conversion from degrees to radians

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

void draw_linear_tracks(int number_of_bricks) {
    glBegin(GL_QUADS);
    //glColor4f(0.5, 0.0, 0.4, 1.0);
    glColor4f(1.0, 1.0, 1.0, 0.8);

    // code stolen from track() function given by lecturer 
    for (int i = 0; i < number_of_bricks; i += 5) {
        glNormal3f(0, 1., 0.);      
        glVertex3f(0+i, 1.0, 1);
        glVertex3f(5+i, 1.0, 1);
        glVertex3f(5+i, 1.0, 0);
        glVertex3f(0+i, 1.0, 0); 
        
        glNormal3f(0.0, 0.0, 1.0);   
        glVertex3f(5+i,0.,1);
        glVertex3f(0+i,0.,1);
        glVertex3f(0+i,1.,1);
        glVertex3f(5+i,1,1);

        glNormal3f(0.0, 0.0, -1.0);   
        glVertex3f(0+i, 0.0, 0);
        glVertex3f(5+i, 0.0, 0);
        glVertex3f(5+i, 1.0, 0);
        glVertex3f(0+i, 1.0, 0);
    }
    glEnd();
}

//-------- Tracks  ----------------------------------------------------
void tracks()
{
    // circlar track
	//glColor4f(0.0, 0.0, 0.3, 1.0);
	glColor4f(1.0, 1.0, 1.0, 0.5);
	track(115.0);   //Inner track has radius 115 units
	track(125.0);   //Outer track has radius 125 units

}

void parallel_tracks(float track_offset_x, float track_offset_y, float track_offset_z) { 
    glPushMatrix();
        glTranslatef(track_offset_x, 0.0, track_offset_z);
        draw_linear_tracks(TRACK_LENGTH);
        glPushMatrix();
            // Appropriate matching rail
            glTranslatef(0.0, 0.0, 10);
            draw_linear_tracks(TRACK_LENGTH);
        glPopMatrix();
    glPopMatrix();
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
void wagon() {
    base();
    // Randomly color wagons
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/1.0);
    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/1.0);
    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/1.0);

    //glColor4f(0.0, 1.0, 1.0, 1.0);
    glColor4f(r, g, b, 1.0);
    glPushMatrix();
      glTranslatef(0.0, 10.0, 0.0);
      glScalef(18.0, 10.0, 10.0);
      glutSolidCube(1.0);
    glPopMatrix();
    glColor4f(0.0, 1.0, 1.0, 1.0);
}


//---------------------------------------------------------------------
void initialize(void) 
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    /*
    float red[4] = {0.15, 0.0, 0.0, 1.0};
    float green[4] = {0.0, 0.15, 0.0, 1.0};
    float clear[4] = {0.0, 0.0, 0.0, 1.0};
    */


	q = gluNewQuadric();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    // train light
    glEnable(GL_LIGHT1);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);

    /*
    // stop light red
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, red);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT2, GL_SPECULAR, white);
    // stop light green
    glEnable(GL_LIGHT3);
    glLightfv(GL_LIGHT3, GL_AMBIENT, green);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT3, GL_SPECULAR, white);
    */

    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
 
    loadGLTextures();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
    gluQuadricDrawStyle(q, GLU_FILL);
	glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1500.0);   //Perspective projection
}

//--Translates engine and front facing spotlight -------------------
void position_engine(void)
{
   float lgt1_pos[] = {-10.0f, 14.0f, 0.0f, 1.0f};     // light1 position
   // NOTE: Leaving the last two values undefined results in some WACK lighting/glitches

   float lgt1_dir[] = {5.0f, -2.0f, 0.0f, 0.0f};        // light1 direction

   glPushMatrix();
        glTranslatef(0, 1, -120);
        glLightfv(GL_LIGHT1, GL_POSITION, lgt1_pos);    // light1 position
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lgt1_dir);
        engine();
   glPopMatrix();
}

//-------------------------------------------------------------------
// Generate 4 wagons. Invisible magic numbers, please
void position_wagon(void)
{
    for (int i=1; i<WAGONS + 1; i++) {
        glPushMatrix();
            glRotatef((i*10.5), 0, 1, 0);
            glTranslatef(0, 1, -120);
            wagon();
        glPopMatrix();
    }
}

//-------------------------------------------------------------------
void rotate_train(int value) {
    glutPostRedisplay();
    theta--;
    glutTimerFunc(20, rotate_train, 0); // 10ms
}

void linear_train_logic(int value) {
    float half_track_length = TRACK_LENGTH/2.0 - 40;
    // TODO: Add condition to slow down where near station, use train1_speed


    /* Debugging parallel trains 
    std::cout << "Half_track_length: " << half_track_length << endl;
    std::cout << "Train location x: " << train1_location_x << endl;
    */

    glFlush();
    if (TRAIN1_DECREASING) {
        // move in the positive direction
        if (train1_location_x <= -half_track_length) {
            TRAIN1_DECREASING = false;
        } else {
            train1_location_x -= train1_speed;
            train0_location_x += train0_speed;
        }
    } else {
        if (train1_location_x > half_track_length) {
            TRAIN1_DECREASING = true;
        } else {
            train1_location_x += train1_speed;
            train0_location_x -= train0_speed;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(30, linear_train_logic, 0);
}

void pan(int glut_requires) {
    if (pan_angle >= 360) {
        pan_angle = 0;
    }
    if (FPS_MODE == false) {
        pan_angle+=0.5;
        glutPostRedisplay();
    }
    glutTimerFunc(30, pan, 0);
}


void linear_train(bool train) {
    // terrible coding practice but was sadly running out of time d(-_-)b
    // double ended train, 'cause it goes 'backwards' too 
    int magic_wagon_count = 2;
    int offset = 0;

    glPushMatrix();
        glTranslatef(train1_location_x, 0, 0);

        glPushMatrix();
            glTranslatef(-58.0, 0, train1_location_z); // space
            glRotatef(180.0, 0, 1, 0);
            engine();
        glPopMatrix();
        
        for (int i = 1; i < magic_wagon_count + 1; i++) {
            offset = i * 20; // wagon length
            glPushMatrix();
                glTranslatef(-54+offset, 0, train1_location_z);
                wagon();
            glPopMatrix();
        }
        glPushMatrix();
            glTranslatef(8.0, 0, train1_location_z); // space
            engine();
        glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(train0_location_x, 0, 0);

        glPushMatrix();
            glTranslatef(-58.0, 0, train0_location_z); // space
            glRotatef(180.0, 0, 1, 0);
            engine();
        glPopMatrix();
        
        for (int i = 1; i < magic_wagon_count + 1; i++) {
            offset = i * 20; // wagon length
            glPushMatrix();
                glTranslatef(-54+offset, 0, train0_location_z);
                wagon();
            glPopMatrix();
        }
        glPushMatrix();
            glTranslatef(8.0, 0, train0_location_z); // space
            engine();
        glPopMatrix();
    glPopMatrix();

    glutPostRedisplay();
}


void move_camera(void) {
    if (FPS_MODE) {
        // hide curosr, avoids cursor ghosting
        glutSetCursor(GLUT_CURSOR_NONE); 
        gluLookAt(camera_pov[0], camera_pov[1], camera_pov[2],
                  camera_pov[0] + dist_cam*cos(cam_angle_x*M_PI/180.),
                  camera_pov[1] + dist_cam*sin(cam_angle_y*M_PI/180.),
                  camera_pov[2] + dist_cam*sin(cam_angle_x*M_PI/180.),
                  0.0, 1.0, 0.0);

        // LOCK CURSOR IN CENTER
        glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
    } else {
        gluLookAt(pan_radius*sin(pan_angle*M_PI/180.0), pan_height, pan_radius*cos(pan_angle*M_PI/180.0), 0,
                pan_height, 0, 0.0, 1.0, 0.0);
    }
}



//-----------------------------------------------------------------
// THIS SECTION HANDLES 2-key 'rollover' of the user's WASD strokes
// NOT IMPLEMENTED BUT WOULD HAVE BEEN NICE TO MOVE IN DIAGONALS
// ----------------------------------------------------------------
void keyboard_w(void) {
    camera_pov[0] += 2.*cos(cam_angle_x*M_PI/180.);
    camera_pov[2] += 2.*sin(cam_angle_x*M_PI/180.);
}

void keyboard_a(void) {
    camera_pov[0] += 2.*sin(cam_angle_x*M_PI/180.);
    camera_pov[2] += -2.*cos(cam_angle_x*M_PI/180.);
}

void keyboard_s(void) {
    camera_pov[0] -= 2.*cos(cam_angle_x*M_PI/180.);
    camera_pov[2] -= 2.*sin(cam_angle_x*M_PI/180.);
}

void keyboard_d(void) {
    camera_pov[0] += -2.*sin(cam_angle_x*M_PI/180.);
    camera_pov[2] += 2.*cos(cam_angle_x*M_PI/180.);
}


bool input_same_as_last(unsigned char key) {
    if (key == last_char) {
        return true;
    } else {
        last_char = key;
        return false;
    }
}

// Modify global keyboard array for release of key
void user_input_up(unsigned char key, int x, int y) {
    keyboard_down[key] = false;

    if (not input_same_as_last(key)) {
        keyboard_down_count -= 1;
    }
}

/*
 * Below is where I started to process multiple WASD
 * keyboard strokes
 *
void user_input(unsigned char key, int x, int y) {
    keyboard_down[key] = true;
    if (not input_same_as_last(key)) {
        keyboard_down_count += 1;
    }
    std::cout << "Keys down: " << keyboard_down_count;
    glFlush();
    if (keyboard_down_count == 2) {
        if (keyboard_down['w']) {
            if (keyboard_down['a']) {
                std::cout << "wa";
                keyboard_w();
                keyboard_a();
                glFlush();
            } else if (keyboard_down['d']) {
                std::cout << "wd";
                keyboard_w();
                keyboard_d();
                glFlush();
            } else {
                // last key wasn't WASD
                std::cout << "w";
                keyboard_w();
                glFlush();
            }
        }
    } else if (keyboard_down_count >= 3) {
        std::cout << "Are you a QA tester? Do you hate your keyboard? Only 2 keys or less at once, please\n\n";
        glFlush();
    } else {
        // single key press
        switch(key) {
            case 'w':
                camera_pov[0] += 2.*cos(cam_angle_x*M_PI/180.);
                camera_pov[2] += 2.*sin(cam_angle_x*M_PI/180.);
                break;
            case 'a':
                camera_pov[0] += 2.*sin(cam_angle_x*M_PI/180.);
                camera_pov[2] += -2.*cos(cam_angle_x*M_PI/180.);
                break;
            case 's':
                camera_pov[0] -= 2.*cos(cam_angle_x*M_PI/180.);
                camera_pov[2] -= 2.*sin(cam_angle_x*M_PI/180.);
                break;
            case 'd':
                camera_pov[0] += -2.*sin(cam_angle_x*M_PI/180.);
                camera_pov[2] += 2.*cos(cam_angle_x*M_PI/180.);
                break;
        }
    }
}
*/

void user_input(unsigned char key, int x, int y) {
    switch(key) {
        case 'w':
            camera_pov[0] += 3.*cos(cam_angle_x*M_PI/180.);
            camera_pov[2] += 3.*sin(cam_angle_x*M_PI/180.);
            break;
        case 'a':
            camera_pov[0] += 3.*sin(cam_angle_x*M_PI/180.);
            camera_pov[2] += -3.*cos(cam_angle_x*M_PI/180.);
            break;
        case 's':
            camera_pov[0] -= 3.*cos(cam_angle_x*M_PI/180.);
            camera_pov[2] -= 3.*sin(cam_angle_x*M_PI/180.);
            break;
        case 'd':
            camera_pov[0] += -3.*sin(cam_angle_x*M_PI/180.);
            camera_pov[2] += 3.*cos(cam_angle_x*M_PI/180.);
            break;
    }

}

/*
 * F1:  Quit
 * F3:  Pan
 */
void special_keys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            glutDestroyWindow(window_id);
            exit(0);
            break;

        case GLUT_KEY_F2:
            // station cam, no time sadly
            break;

        case GLUT_KEY_F3:
            if (FPS_MODE) {
                FPS_MODE = false;
                glFlush();
                break;
            
            } else {
                std::copy(std::begin(camera_pov_reset), std::end(camera_pov_reset), std::begin(camera_pov));
                FPS_MODE = true;
                break;
            }

        case GLUT_KEY_F4:
            // train cam, no time sadly
            break;
    }
}
//-----------------------------------------------------------------
// END OF KEYBOARD INPUT SECTION  
// ----------------------------------------------------------------

float convert_to_degrees(float x) {
    return x*M_PI/180.0;
}

// fps style mouse, no mouse acceleration 
void mouse_move(int x, int y) {
    int delta_mouse = x- (int)previous_x; // distance
    float rotation = 15;
    previous_x = x;

    if (delta_mouse > 0) {
        cam_angle_x += convert_to_degrees(rotation -5);
    } else if (delta_mouse < 0) {
        cam_angle_x -= convert_to_degrees(rotation - 5);
    }

    int delta_mouse_y = y - (int)previous_y; // distance
    previous_y = y;
    if (delta_mouse_y > 0) {
        cam_angle_y -= convert_to_degrees(rotation);
    } else if (delta_mouse_y < 0) {
        cam_angle_y += convert_to_degrees(rotation);
    }
}


void draw_station(void) {
    float roof_length = TRACK_LENGTH/2.0;

    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
        glBegin(GL_QUADS);
        // roof
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-roof_length, 60, TRACK_OFFSET_Z + 90);
        glTexCoord2f(9.0, 0.0);
        glVertex3f(roof_length, 60, TRACK_OFFSET_Z + 90);
        glTexCoord2f(9.0, 1.0);
        glVertex3f(roof_length, 80, TRACK_OFFSET_Z + 190);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-roof_length, 80, TRACK_OFFSET_Z + 190);

        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // pillars
    for (int i = -200; i < 400; i += 100) {

        glColor4f(0.0, 1.0, 0.0, 1.0);
        glPushMatrix();
            glTranslatef(i, 1, TRACK_OFFSET_Z + 90);
            glRotatef(270.0, 1, 0, 0);
            GLUquadricObj *quadObject = gluNewQuadric();
            gluCylinder(quadObject, 10, 1, 59, 10, 10);
        glPopMatrix();

        glColor4f(0.0, 0.0, 1.0, 1.0);
        glPushMatrix();
            glTranslatef(i, 1, TRACK_OFFSET_Z + 190);
            glRotatef(270.0, 1, 0, 0);
            GLUquadricObj *quadObject2 = gluNewQuadric();
            gluCylinder(quadObject2, 10, 1, 79, 10, 10);
        glPopMatrix();
    }

    
}

void draw_tunnel(void) {

    // Tunnels for circular track, wireframes are architecture, right?
    glPushMatrix();
        glTranslatef(-115.0, 7.0, 0.0);
        glutWireTorus(5.5, 20.7, 6, 6);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(115.0, 7.0, 0.0);
        glutWireTorus(5.5, 20.7, 6, 6);
    glPopMatrix();

    // Tunnels for parallel tracks
    glPushMatrix();
        glTranslatef(TUNNEL_OFFSET_X, 7.0, TRACK_OFFSET_Z+5);
        glScalef(15.0f, 2.0f, 1.0f);
        glRotatef(90.0, 0, 1.0, 0);
        glutSolidTorus(5.5, 20.7, 30, 30);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(TUNNEL_OFFSET_X, 7.0, -TRACK_OFFSET_Z+5);
        glScalef(15.0f, 2.0f, 1.0f);
        glRotatef(90.0, 0, 1.0, 0);
        glutSolidTorus(5.5, 20.7, 30, 30);
    glPopMatrix();
}

void draw_path() {
    glBindTexture(GL_TEXTURE_2D, txId[0]);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2f(0., 0.);
            glVertex3f(-PATH_WIDTH,  1., -TRACK_OFFSET_Z + 11);
            glTexCoord2f(5., 0.);
            glVertex3f(-PATH_WIDTH, 1., TRACK_OFFSET_Z - 2);
            glTexCoord2f(5., 1.);
            glVertex3f(PATH_WIDTH, 1, TRACK_OFFSET_Z - 2);
            glTexCoord2f(0., 1.);
            glVertex3f(PATH_WIDTH, 1, -TRACK_OFFSET_Z + 11);
        glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void draw_barrier() {

    glBindTexture(GL_TEXTURE_2D, txId[1]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
        glBegin(GL_QUADS);  // primitive type

            // front
            glTexCoord2f(0., 0.);
            glVertex3f(50.0, 10.0, POS_Z_CUTOFF_BARRIER);
            glTexCoord2f(5., 0.);
            glVertex3f(-50.0, 10.0, POS_Z_CUTOFF_BARRIER);
            glTexCoord2f(5., 1.);
            glVertex3f(-50.0, 15.0, POS_Z_CUTOFF_BARRIER);
            glTexCoord2f(0., 1.);
            glVertex3f(50, 15.0, POS_Z_CUTOFF_BARRIER);

        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void draw_barrier_body() {
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
        glBegin(GL_QUADS);

        // column/body
        glTexCoord2f(0., 0.);
        glVertex3f(-45.0, 0.0, POS_Z_CUTOFF_BARRIER-1);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(-55.0, 0.0, POS_Z_CUTOFF_BARRIER-1);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(-55.0, 15.0, POS_Z_CUTOFF_BARRIER-1);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-45.0, 15.0, POS_Z_CUTOFF_BARRIER-1);

        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void draw_traffic_light() {
    if (STOP_LIGHT) {
        // red
        glColor3f(1.0, 0.1, 0.1);
    } else {
        // green
        glColor3f(0.0, 1.0, 0.1);
    }
    glPushMatrix();
        glTranslatef(-50, 15, POS_Z_CUTOFF_BARRIER);
        glutSolidSphere(3.0, 20.0, 20.0);
    glPopMatrix();
}

void stop_light_logic(int value) {
    STOP_LIGHT = BARRIER_THETA > 45.0 ? false : true;
    glutTimerFunc(20, stop_light_logic, 0);
}
    

void animate_barrier() {
    glPushMatrix();
        glTranslatef(-50.0, 10, POS_Z_CUTOFF_BARRIER);
        glRotatef(BARRIER_THETA, 0.0, 0.0, 1);
        glTranslatef(50.0, -10, -POS_Z_CUTOFF_BARRIER);
        glVertex3f(-50.0, 10.0, POS_Z_CUTOFF_BARRIER);
        draw_barrier();
    glPopMatrix();
}

void barrier_logic(int value) {
    if (BARRIER_THETA >= 90) {
        BARRIER_LOWERING = true;
    } else if (BARRIER_THETA <= 0) {
        BARRIER_LOWERING = false;
    }

    if (BARRIER_LOWERING) {
        BARRIER_THETA -= 0.5;
    } else {
        BARRIER_THETA += 0.5;
    }
    glutTimerFunc(20, barrier_logic, 0);
}

void draw_speaker_tower(int value) {

    glColor4f(0.0, 1.0, 0.0, 1.0);

    glPushMatrix();
        glScalef(3.0, 15.5, 10.0);
        glRotatef(90.0, 1, 0, 0);
        glutSolidCube(10);
    glPopMatrix();
}

void draw_speak_tower_ears(void) {
    glColor4f(1.0, 0.0, 1.0, 1.0);
    glPushMatrix();
        glRotatef(270.0, 1, 0, 0);
        glutSolidCone(50.0, 50.0, 15, 15);
    glPopMatrix();
}


void draw_speakers(int value) {
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();

        glBegin(GL_LINE_LOOP);
        for(int i =0; i <= 300; i++) {
            double angle = 2 * M_PI * i / 300.0;
            double x = cos(angle);
            double y = sin(angle);
            glVertex2d(x,y);
        }
        glEnd(); 
    glPopMatrix();

}


void draw_dance_floor(int value) {

    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/1.0);
    float g = std::fmod(r, 0.5);
    float b = std::fmod(r + 0.2, 0.5);

    glPushMatrix();
        glBegin(GL_QUADS);

        if (r <= 0.33) {
                glColor3f(1.0f, 0.0, 0.0);
                glVertex3f(-100.0f, 0.0f, 75.0f);

                glColor3f(0.0f, 1.0, 0.0);
                glVertex3f(100.0f, 0.0f, 75.0f);

                glColor3f(0.0f, 0.0, 1.0);
                glVertex3f(100.0f, 0.0f, -75.0f);

                glColor3f(1.0f, 0.0, 1.0);
                glVertex3f(-100.0f, 0.0f, -75.0f);

        } else if (r < 0.67) {
                glColor3f(0.0f, 1.0, 1.0);
                glVertex3f(-100.0f, 0.0f, 75.0f);

                glColor3f(b, g, r);
                glVertex3f(100.0f, 0.0f, 75.0f);

                glColor3f(0.9f, 0.9, 0.9);
                glVertex3f(100.0f, 0.0f, -75.0f);

                glColor3f(r, b, g);
                glVertex3f(-100.0f, 0.0f, -75.0f);


        } else {
                glColor3f(r, g, b);
                glVertex3f(-100.0f, 0.0f, 75.0f);

                glColor3f(b, g, r);
                glVertex3f(100.0f, 0.0f, 75.0f);

                glColor3f(g, r, b);
                glVertex3f(100.0f, 0.0f, -75.0f);

                glColor3f(r, b, g);
                glVertex3f(-100.0f, 0.0f, -75.0f);

        }

        glEnd();
    glPopMatrix();
}

//--Draws a character model constructed using GLUT object ------------------
void drawModel(int model_count) {
    //for (int i=1; i<model_count; i++) {
    for (int i=1; i<2; i++) {
        glPushMatrix();
        if (i % 2 == 0) {
              //glTranslatef(man_pos * (i)/10, 0., i * 10.0);
              glTranslatef(i * 2, 0., (man_pos * i * 3));
              glScalef(2.0, 5.0, 3.0);
              //glRotatef(90.0, 0, 1, 0);
        } else {
              //glTranslatef(0-(man_pos * (i)/10), 0., i * -10.0);
              glTranslatef(i * 2, 0., (man_pos * i * 3));
              glScalef(2.0, 4.0, 3.0);
              //glRotatef(90.0, 0, 1, 0);
        }

            // Head
            glColor3f(1., 0.78, 0.06);		
            glPushMatrix();
              glTranslatef(0, 7.7, 0);
              glutSolidCube(1.4);
            glPopMatrix();

            // Torso
            glColor3f(1., 0., 0.);		
            glPushMatrix();
              glTranslatef(0, 5.5, 0);
              glScalef(3, 3, 1.4);
              glutSolidCube(1);
            glPopMatrix();

            // right leg
            glColor3f(1., 0., 0.);			
            glPushMatrix();
                glTranslatef(-0.8, 4, 0);
                glRotatef((0-humanoid_theta), 1, 0, 0);
                glTranslatef(0.8, -4, 0);
                glTranslatef(-0.8, 2.2, 0);
                glScalef(1, 4.6, 1);
                glutSolidCube(1);
            glPopMatrix();

            // left leg
            glColor3f(0., 0., 1.);			
            glPushMatrix();
                // rotate by global theta around the pivot point (0.8, 4, 0) 
                glTranslatef(0.8, 4, 0);
                glRotatef(humanoid_theta, 1, 0, 0);
                glTranslatef(-0.8, -4, 0);
                glTranslatef(0.8, 2.2, 0);
                glScalef(1, 4, 1);
                glutSolidCube(1);
            glPopMatrix();

            // right arm
            glColor3f(0., 0., 1.);			
            glPushMatrix();
                // rotate by global theta around the pivote point (-2, 6.5, 0) <right_arm>
                glTranslatef(-2, 6.5, 0);
                glRotatef(humanoid_theta, 1, 0, 0);
                glTranslatef(2, -6.5, 0);
                glTranslatef(-2, 5, 0);
                glScalef(1, 4, 1);
                glutSolidCube(1);
            glPopMatrix();

            // left arm
            glColor3f(1., 0., 0.);			
            glPushMatrix();
                glTranslatef(2, 6.5, 0);
                glRotatef((0-humanoid_theta), 1, 0, 0);
                glTranslatef(-2, -6.5, 0);
                glTranslatef(2, 5, 0);
                glScalef(1, 4.4, 1);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();
    }
}

//--Walk:   ---------------------------------------------------------------
void walk(int value) {
    glutPostRedisplay();
    if (rising_theta) {
        humanoid_theta++;
        if (humanoid_theta >= 20) rising_theta = false;
    } else {
        humanoid_theta--;
        if (humanoid_theta <= -20) rising_theta = true;
    }
    if (rising_walk) {
        man_pos += 0.2;
        if (man_pos >= RAND_HI) {
            rising_walk = false;
        }
    } else {
        man_pos -= 0.2;
        if (man_pos <= RAND_LOW) {
            rising_walk = true;
        }
    }
    glutTimerFunc(20, walk, 0);
}

//-------------------------------------------------------------------
void display(void)
{
    float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f};    //light0 position (directly above the origin)

    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    // cameras
    move_camera();
    glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);     

    // SCALE EVERYTHING UP
    glPushMatrix();

        glScalef(2.0, 2.0, 2.0);

        floor();
        // circular track and train/wagons
        glPushMatrix();
            glTranslatef(200.0f, 0, 0);
            tracks();  
            glPushMatrix();
                glRotatef(theta, 0, 1, 0);
                position_engine();
                position_wagon();
            glPopMatrix();
            draw_tunnel();
        glPopMatrix();

        // parallel tracks
        glPushMatrix();
            glTranslatef(-(TRACK_LENGTH/2.0), 0.0, 0);
            parallel_tracks(TRACK_OFFSET_X, TRACK_OFFSET_Y, TRACK_OFFSET_Z);
            parallel_tracks(TRACK_OFFSET_X, TRACK_OFFSET_Y, -TRACK_OFFSET_Z); // note the inverse z
        glPopMatrix();

        // Parallel trains
        glPushMatrix();
            linear_train(false);
            linear_train(true);
        glPopMatrix();


        draw_station();
        draw_path();

        glPushMatrix();
            glTranslatef(-170.0f, 0.1f, 0.f);
            draw_dance_floor(0);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-250.0f, 0.1f, 0.f);
            draw_speaker_tower(0);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-250.0f, 80.1f, 0.f);
            draw_speak_tower_ears();
        glPopMatrix();

        // Draw speakers/face, like ->>> 0.0
        // Mouth
        glPushMatrix();
            glTranslatef(-230.0f, 25.1f, 2.f);
            glRotatef(270.0f, 0, 1, 0);
            glScalef(8.0f, 8.0f, 3.0f);
            draw_speakers(1);
        glPopMatrix();

        // Left Eye
        glPushMatrix();
            glTranslatef(-230.0f, 50.1f, 30.0f);
            glRotatef(270.0f, 0, 1, 0);
            glScalef(14.0f, 14.0f, 3.0f);
            draw_speakers(1);
        glPopMatrix();

        // Right Eye
        glPushMatrix();
            glTranslatef(-230.0f, 50.1f, -30.0f);
            glRotatef(270.0f, 0, 1, 0);
            glScalef(14.0f, 14.0f, 3.0f);
            draw_speakers(1);
        glPopMatrix();

        // draw traffic light ontop of barrier
        draw_traffic_light();

        /*
         * Originally had ambient lights on the barrier arms
        glPushMatrix();
            if (STOP_LIGHT) {
                glDisable(GL_LIGHT3);
                glEnable(GL_LIGHT2);
                glLightfv(GL_LIGHT2, GL_POSITION, spotlight_pos);
                //glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotlight_dir);
            } else {
                glDisable(GL_LIGHT2);
                glEnable(GL_LIGHT3);
                glLightfv(GL_LIGHT3, GL_POSITION, spotlight_pos);
                //glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotlight_dir);
            }
        glPopMatrix();
        */

        // draw 'top' barrier
        animate_barrier();
        draw_barrier_body();

        // draw 'bottom' barrier
        glPushMatrix();
            glTranslatef(0, 0, 2*NEG_Z_CUTOFF_BARRIER);
            animate_barrier();
            draw_barrier_body();
            draw_traffic_light();
        glPopMatrix();
        

        // humanoid
        srand (static_cast <unsigned> (time(0))); // current time as seed
        random_float = RAND_LOW + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/RAND_HI);
        std::default_random_engine generator (random_float);
        std::normal_distribution<double> distribution (0.0, 1.0);

        random_float = 10 * distribution(generator);

        if (random_float > RAND_MID) {
            RAND_HI = random_float;
        } else if (random_float < RAND_MID) {
            RAND_LOW = random_float;
        }

        glEnable(GL_LIGHTING);	       //Enable lighting when drawing the model
        drawModel(MODEL_COUNT);
    glPopMatrix();

    glutSwapBuffers();   //Useful for animation
}



//---------------------------------------------------------------------
int main(int argc, char** argv) {
    // Works best with -std=c++0x -W -Wall -Wextra -pedantic

    if (argc > 1) {
       int input= atoi(argv[1]);
       WAGONS = (4 > input && input >= 1 ) ? input : 1;
    } else {
        std::cerr << "Usage: ./<program_name> <N> " << USAGE_MESSAGE << endl; 
        glFlush();
        return 1;
    }

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
   glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT); 
   glutInitWindowPosition (0, 0);
   window_id = glutCreateWindow ("github.com/C-Kenny | Mouse to look, WASD to move!");

   initialize ();

   glutDisplayFunc(display); 
   glutIgnoreKeyRepeat(0);
   glutSpecialFunc(special_keys);
   glutKeyboardFunc(user_input);
   glutKeyboardUpFunc(user_input_up);
   
   // control logic
   glutTimerFunc(10, pan, 0);
   glutTimerFunc(20, rotate_train, 0);
   glutTimerFunc(30, linear_train_logic, 0);
   glutTimerFunc(10, walk, 0);
   glutTimerFunc(10, barrier_logic, 0);
   glutTimerFunc(20, stop_light_logic, 0);
   glutPassiveMotionFunc(mouse_move); // process mouse when no button is down
   //glutMotionFunc(mouse_move); // process mouse when button is down, DESIGN CHOICE TO REMOVE
   glutMainLoop();
   return 0; // no errors
}
