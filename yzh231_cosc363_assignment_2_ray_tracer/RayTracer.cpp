/*========================================================================
* COSC 363 Assignment 2 Ray Tracer
* Date: 24/05/2017
* Author: Melody Zhu
* ID: 97245310
* Build Command: g++ -Wall -o "%e" RayTracer.cpp Ray.cpp SceneObject.cpp Sphere.cpp Plane.cpp Cylinder.cpp Cone.cpp TextureBMP.cpp -lm -lGL -lGLU -lglut
*=========================================================================
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Plane.h"
#include <GL/glut.h>
#include "Cylinder.h"
#include "TextureBMP.h"
#include "Cone.h"

using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;
const float ETA = 1.003;

TextureBMP texture1;
TextureBMP texture2;
vector<SceneObject*> sceneObjects;  //A global list containing pointers to objects in the scene


//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{	
	glm::vec3 backgroundCol(0);
	glm::vec3 light(-10.0, 15, -5.0);//light source 1
	glm::vec3 light2(16.0, 16, 0); //light source 2
    glm::vec3 white(1);
	glm::vec3 specular(0);
	glm::vec3 specular2(0);
	glm::vec3 colorSum(0);
	glm::vec3 ambientTerm(0.2);
	
	float lgt1Intensity = 0.6;
	float f = 20.0;
	float transparency = 0.2;
	
    ray.closestPt(sceneObjects);		//Compute the closest point of intersetion of objects with the ray

    if(ray.xindex == -1) return backgroundCol;      //If there is no intersection return background colour
    
    glm::vec3 col = sceneObjects[ray.xindex]->getColor(); //else return bject's colour
    
    
    glm::vec3 normalVector = sceneObjects[ray.xindex]->normal(ray.xpt);
    
    glm::vec3 lightVector = light - ray.xpt;    
    glm::vec3 lightVector2 = light2 - ray.xpt;
   
    
    float lightDist = glm::length(lightVector);
    float lightDist2 = glm::length(lightVector2); 
    
    
    lightVector=glm::normalize(lightVector);
    lightVector2=glm::normalize(lightVector2);
    
    
    float lDotn = glm::dot(lightVector,normalVector);
    float lDotn2 = glm::dot(lightVector2,normalVector);
    
    glm::vec3 reflVector = glm::reflect(-lightVector, normalVector); //reflvector already normalized
	glm::vec3 reflVector2 = glm::reflect(-lightVector2, normalVector);
	
	
	float rDotv = glm::dot(reflVector,-ray.dir);  //rdotv >= 0
	rDotv = max(rDotv, 0.0f);
	float rDotv2 = glm::dot(reflVector2,-ray.dir);
	rDotv2 = max(rDotv, 0.0f);

//~ //-----------spot light-------------------------------------------------	
	//~ float alpha = M_PI/4.0f;
    //~ glm::vec3 spot(16,-20,-40);
    //~ glm::vec3 spotVector = glm::normalize(spot-light2);
    //~ float theta = glm::acos(glm::dot(spotVector, lightVector2));
    //~ float fade = 1.0f-theta/alpha;
	//~ fade = max(fade,0.0f);

//-------------------Textures-------------------------------------------	
	//sphere texture
    if (ray.xindex == 1)
	{
		glm::vec3 center(6.5, 5.0, -70.0);
		glm::vec3 d=glm::normalize(ray.xpt-center);
		float u=(0.5-atan2(d.z,d.x)+M_PI)/(2*M_PI);
		float v=0.5+asin(d.y)/M_PI;
		col = texture2.getColorAt(u, v);
   }
   
   //floor texture
   if(ray.xindex == 3){
		int modx = (int)((ray.xpt.x + 50) /8) % 2;
		int modz = (int)((ray.xpt.z + 200) /8) % 2;
		
       if((modx && modz) || (!modx && !modz)){
		   col = glm::vec3(0.2,0.2,0.2);}
	   else{
		   col = glm::vec3(1,1,1);}
	}	   		     
	
	//wall texture
	if(ray.xindex == 4)
	{
		float s = (ray.xpt.x+50)/100;
		float t = (ray.xpt.y+20)/70;
		//rDotv=0;   //turn off specular light 
		//rDotv2=0;
		col = texture1.getColorAt(s,t);
	}	
	
	//procedual texture on cylinder
	if(ray.xindex == 5)
	{
		if ((int(ray.xpt.x+ray.xpt.y-13) % 2 == 0)){
			col = glm::vec3(0.2,0.4,0);
		}else{
			col = glm::vec3(0.8,1,0.6);
		}   		
   }
   
  
//---------------Shadows------------------------------------------------  
	Ray shadow(ray.xpt,lightVector);
	shadow.closestPt(sceneObjects);
	
	Ray shadow2(ray.xpt,lightVector2);
	shadow2.closestPt(sceneObjects);
	

//---Shadow of light 1
	if (rDotv>=0){
		specular = pow(rDotv,f)*white;
	}

	
	if ((lDotn<=0 )||((shadow.xindex>-1)&&(shadow.xdist < lightDist))){
		colorSum = col*ambientTerm;
		if(shadow.xindex == 2){
			colorSum += (lDotn*col + specular)*glm::vec3(0.4)+sceneObjects[2]->getColor()*glm::vec3(0.02);
        }//transparent object:lighter shadow with a hint of the obj's color
	}else{
		colorSum=ambientTerm*col + (lDotn*col + specular)*lgt1Intensity;

	}
	
//---Shadow of light 2	
	if (rDotv2>=0){
		specular2 = pow(rDotv2,f)*white;
	}
	
	if ((lDotn2<=0 )||((shadow2.xindex>-1)&&(shadow2.xdist < lightDist2))){
		colorSum += col*ambientTerm;
		if(shadow2.xindex == 2){
			colorSum +=(lDotn2*col + specular2)*glm::vec3(0.15) + sceneObjects[2]->getColor()*glm::vec3(0.0125);//transparent object has lighter shadow 
        } 
	}else{
		colorSum+=ambientTerm*col + (lDotn2*col + specular2)*(1-lgt1Intensity);

	}

	
//---------------------Reflection---------------------------------------
	if((ray.xindex == 0||ray.xindex ==4) &&(step < MAX_STEPS)){    //Blue ball and wall have reflections
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVector);
		Ray reflectedRay(ray.xpt, reflectedDir);
		glm::vec3 reflectedCol = trace(reflectedRay, step+1); //Recursion!
		colorSum = colorSum + (0.8f*reflectedCol);
    }
    

//---------------Refraction with transparency---------------------------
    if((ray.xindex == 2) && (step < MAX_STEPS)){
		glm::vec3 refracDir1 = glm::refract(ray.dir,normalVector,1.0f/ETA);
		Ray refracRay1(ray.xpt,refracDir1);
		refracRay1.closestPt(sceneObjects);
		if(refracRay1.xindex==-1){
			return backgroundCol;
		}
		glm::vec3 normalVector2 = sceneObjects[refracRay1.xindex]->normal(refracRay1.xpt); 
		glm::vec3 refracDir2 = glm::refract(refracDir1, -normalVector2, ETA);
		Ray refracRay2(refracRay1.xpt,refracDir2);
		refracRay2.closestPt(sceneObjects);
		if(refracRay2.xindex==-1){
			return backgroundCol;
		}
		glm::vec3 refracCol2 = trace(refracRay2, step+1);
		colorSum = colorSum * transparency + refracCol2*(1-transparency); //transparent object
		
		return colorSum;
	}
  
	return colorSum;
}

//---------------------Draw cube-----------------------------------------------------
void drawCube(float x, float y, float z, float l, float w, float h, glm::vec3 color)
{   
	glm::vec3 A = glm::vec3(x,y,z);
	glm::vec3 B = glm::vec3(x+l,y,z);
	glm::vec3 C = glm::vec3(x+l,y+h,z);
	glm::vec3 D = glm::vec3(x,y+h,z);
	glm::vec3 E = glm::vec3(x+l,y,z-w);
	glm::vec3 F = glm::vec3(x+l,y+h,z-w);
	glm::vec3 G = glm::vec3(x,y+h,z-w);
	glm::vec3 H = glm::vec3(x,y,z-w);
	
	Plane *plane1 = new Plane(A,B,C,D,color);
	                          
	Plane *plane2 = new Plane(B,E,F,C,color);
	                          
	Plane *plane3 = new Plane(E,H,G,F,color);
	
	Plane *plane4 = new Plane(D,G,H,A,color);
	
	Plane *plane5 = new Plane(D,C,F,G,color);
	
	Plane *plane6 = new Plane(H,E,B,A,color);
	                                                  
	sceneObjects.push_back(plane1);
	sceneObjects.push_back(plane2);
	sceneObjects.push_back(plane3);
	sceneObjects.push_back(plane4);
	sceneObjects.push_back(plane5);
	sceneObjects.push_back(plane6);
	
}

//----------------------Draw tetrahedron---------------------------------------------
void drawTetrahedron(float x, float y, float z, float l, glm::vec3 color)
{
	glm::vec3 A = glm::vec3(x,y,z);
	glm::vec3 B = glm::vec3(x+l,y,z);
	glm::vec3 C = glm::vec3(x+l*0.5, y, z + sqrt(3.0f) * 0.5* l );
	glm::vec3 D = glm::vec3(x+l*0.5, y+sqrt(6.0f)/3.0f * l, z + sqrt(3.0f) * 0.25* l );

	glm::vec3 AB = glm::vec3(x+l/2,y,z);
	glm::vec3 CD = glm::vec3((C.x+D.x)/2,(C.y+D.y)/2,(C.z+D.z)/2);
	
	Plane *triangle1 = new Plane(AB,B,C,A,color);
	Plane *triangle2 = new Plane(B,D,CD,C,color);
	Plane *triangle3 = new Plane(D,A,C,CD,color);
	Plane *triangle4 = new Plane(A,D,B,AB,color);

	sceneObjects.push_back(triangle1);
	sceneObjects.push_back(triangle2);
	sceneObjects.push_back(triangle3);
	sceneObjects.push_back(triangle4);
}	

//-------------------Anti-aliasing(Super-sampling)--------------------------------
glm::vec3 anti_aliasing(glm::vec3 eye, float pixel_size, float xp, float yp){
	float quarter = pixel_size * 0.25;
	float quarter_and_half = pixel_size * 0.75;
	
	glm::vec3 colorSum(0);
	glm::vec3 avg(0.25);
	
	Ray ray = Ray(eye, glm::vec3(xp + quarter, yp + quarter, -EDIST));		
    ray.normalize();				
	colorSum+=trace(ray,1);
	
	ray = Ray(eye, glm::vec3(xp + quarter, yp + quarter_and_half, -EDIST));
	ray.normalize();				
	colorSum+=trace(ray,1);
	
	ray = Ray(eye, glm::vec3(xp + quarter_and_half, yp + quarter, -EDIST));
	ray.normalize();				
	colorSum+=trace(ray,1);
	
	ray = Ray(eye, glm::vec3(xp + quarter_and_half, yp + quarter_and_half, -EDIST));
	ray.normalize();				
	colorSum+=trace(ray,1);
	
	colorSum*= avg;
	return colorSum;
	
	     
}	
	                                                            
//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
	float cellY = (YMAX-YMIN)/NUMDIV;  //cell height

	glm::vec3 eye(0., 0., 0.);  //The eye position (source of primary rays) is the origin

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a quad.

	for(int i = 0; i < NUMDIV; i++)  	//For each grid point xp, yp
	{
		xp = XMIN + i*cellX;
		for(int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j*cellY;

		    glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);	//direction of the primary ray

		    Ray ray = Ray(eye, dir);		//Create a ray originating from the camera in the direction 'dir'
			ray.normalize();//Normalize the direction of the ray to a unit vector
			
			glm::vec3 col = anti_aliasing(eye,cellX,xp,yp); //Anti-aliasing(Super-sampling)			
		    //glm::vec3 col = trace (ray, 1); //Trace the primary ray and get the colour value

			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp+cellX, yp);
			glVertex2f(xp+cellX, yp+cellY);
			glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}


//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glClearColor(0, 0, 0, 1);
    
    //-----initilize textures
    texture1 = TextureBMP((char*)"wall2.bmp");
    texture2 = TextureBMP((char*)"Earth.bmp");
    
    //-- Create a pointer to a plane object  
	Plane *floor = new Plane (glm::vec3(-50., -20, -40),//Point A
							  glm::vec3(50., -20, -40),//Point B
							  glm::vec3(50., -20, -200),//Point C
							  glm::vec3(-50., -20, -200),//Point D
							  glm::vec3(0.8));//Colour
							  
	Plane *wall = new Plane(glm::vec3(-50., -20, -200),//Point A
							glm::vec3(50., -20, -200),//Point B
							glm::vec3(50., 50, -200),//Point C
							glm::vec3(-50., 50, -200),//Point D
							glm::vec3(0.2,0.2,0.2));//Colour			  
							  
	
	//-- Create a pointer to a sphere object
	Sphere *sphere1 = new Sphere(glm::vec3(-5.0, -5.0, -105.0), 14.0, glm::vec3(0,0,1));
	Sphere *sphere2 = new Sphere(glm::vec3(6.5, 5.0, -70.0), 2.5, glm::vec3(0, 0, 0));
    Sphere *sphere3 = new Sphere(glm::vec3(5, -10.0, -68.0), 3.2, glm::vec3(0, 1, 0));
    
    //-- Create a pointer to a cylinder object
    Cylinder *cylinder = new Cylinder(glm::vec3(14, -12, -70), 2, 8.0, glm::vec3(1,0,0));
    
    //-- Create a pointer to a cone object
    Cone *cone = new Cone(glm::vec3(10.5, -12.0, -80.0), 3, 12.0, glm::vec3(1, 0.7529, 0.7961));
	
	//--Add the above to the list of scene objects.
	sceneObjects.push_back(sphere1); //0
	sceneObjects.push_back(sphere2); //1
	sceneObjects.push_back(sphere3); //2
	
	sceneObjects.push_back(floor);   //3
	sceneObjects.push_back(wall);    //4
	
	sceneObjects.push_back(cylinder);//5
	
	sceneObjects.push_back(cone);    //6
	
	//-------Draw Tetrahedron 
	drawTetrahedron(11, -3, -70,5,glm::vec3(1, 0.7529, 0.7961));//7-10

	//-------Draw Cube
	drawCube(8, -10.0, -60.0,2,2,2,glm::vec3(1, 0.7529, 0.7961));//11-16

}



int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracer");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
