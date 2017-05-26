//  ========================================================================
//  COSC363: Computer Graphics;  University of Canterbury (2017)
//
//  FILE NAME: TorusDraw.cpp
//
//	The program generates and loads the mesh data for a torus.
//  The vertex shader includes code for both transformations and lighting.
//  Requires files Shader.h, Torus.h, Torus.cpp, Torus.vert, Torus.frag
//  See Lab09.pdf for details.
//  ========================================================================

#define  GLM_FORCE_RADIANS
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Torus.h"
using namespace std;


GLuint mvMatrixLoc, mvpMatrixLoc, norMatrixLoc, lgtLoc;
Torus *torus;
float angle = 0;
float CDR = 3.14159265/180.0;   //Conversion from degrees to radians

glm::mat4 viewMatrix, projMatrix;  //View and projection matrices

void initialize()
{
	GLuint program = createShaderProg("Torus.vert", "Torus.frag");

	mvMatrixLoc = glGetUniformLocation(program, "mvMatrix");  //Get locations of uniform variables declared in shader
	mvpMatrixLoc = glGetUniformLocation(program, "mvpMatrix");
	norMatrixLoc = glGetUniformLocation(program, "norMatrix");
	lgtLoc = glGetUniformLocation(program, "lightPos");
     
    glm::vec3 eye = glm::vec3(0,0,25);
    glm::vec3 origin = glm::vec3(0,0,0);
    glm::vec3 up = glm::vec3(0,1,0); 
	glm::vec4 light = glm::vec4(20.0, 10.0, 20.0, 1.0);     //Light's position in world coordinates
	projMatrix = glm::perspective(60.0f*CDR, 1.0f, 10.0f, 40.0f);
	viewMatrix = glm::lookAt(eye,origin,up);  //Create a view matrix using GLM function lookAt

	glm::vec4 lightEye = viewMatrix * light; //Light's position in eye coordinates
	glUniform4fv(lgtLoc, 1, &lightEye[0]);   //Pass this value to the shader

	torus = new Torus(4.0, 8.0, 36, 36);
	
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void update(int value)
{
	angle++;
	glutTimerFunc(50.0, update, 0);
	glutPostRedisplay();
}

void display()
{
	glm::mat4 identity = glm::mat4(1.0f);  //Identity matrix
	glm::mat4 rotationMatrix = glm::rotate(identity, angle*CDR, glm::vec3(1.0f, 0.0f, 0.0f));  //rotation matrix
	glm::mat4 mvMatrix = viewMatrix * rotationMatrix;   //The model-view matrix
	glm::mat4 mvpMatrix = projMatrix * mvMatrix; //The model-view-projection matrix

	glm::mat4 invMatrix = glm::inverse(mvMatrix);  //Inverse of model-view matrix for normal transformation
	glUniformMatrix4fv(mvMatrixLoc, 1, GL_FALSE, &mvMatrix[0][0]);
	glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, &mvpMatrix[0][0]);
	glUniformMatrix4fv(norMatrixLoc, 1, GL_TRUE, &invMatrix[0][0]);  //Use transpose matrix here


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	torus->render();

	glFlush();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("A Torus");
	glutInitContextVersion (4, 2);
	glutInitContextProfile ( GLUT_CORE_PROFILE );

	if(glewInit() == GLEW_OK)
	{
		cout << "GLEW initialization successful! " << endl;
		cout << " Using GLEW version " << glewGetString(GLEW_VERSION) << endl;
	}
	else
	{
		cerr << "Unable to initialize GLEW  ...exiting." << endl;
		exit(EXIT_FAILURE);
	}

	initialize();
	glutDisplayFunc(display);
	glutTimerFunc(50, update, 0); 
	glutMainLoop();
}

