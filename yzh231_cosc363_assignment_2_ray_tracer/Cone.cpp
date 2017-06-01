/*----------------------------------------------------------
* COSC 363 Assignment 2 Ray Tracer
* Date: 24/05/2017
* Author: Melody Zhu
* ID: 97245310
* 
* The cone class
* This is a subclass of Object, and hence implements the
* methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cone.h"
#include <math.h>

float Cone::intersect(glm::vec3 pos, glm::vec3 dir)
{   
    glm::vec3 d = pos - center;
    float yd = height - pos.y + center.y;
    float stan = (radius / height) * (radius / height);
    float a = (dir.x * dir.x) + (dir.z * dir.z) - (stan*(dir.y * dir.y));
    float b = 2*(d.x*dir.x + d.z*dir.z + stan*yd*dir.y);
    float c = (d.x*d.x) + (d.z*d.z) - (stan*(yd*yd));
    float delta = b*b - 4*(a*c);
	
	if(fabs(delta) < 0.001){
		return -1.0;
	}	 
    if(delta < 0.0){
		 return -1.0;
    }
    
    float tSmall;
    float tBig;
    float t1 = (-b - sqrt(delta))/(2*a);
    float t2 = (-b + sqrt(delta))/(2*a);
  
    
    if(t1<0.01){
		t1=-1;
	}
	if(t2<0.01){
		t2=-1;
	}

 
	if (t1>t2){
		tSmall = t2;
		tBig=t1;
    }else{
		tSmall = t1;
		tBig=t2;
	}	
    					
						

    float ypos = pos.y + dir.y*tSmall;
    if((ypos >= center.y) && (ypos <= center.y + height)){
		return tSmall;
	}	
    else{
		float ypos = pos.y + dir.y*tBig;
		if((ypos >= center.y) && (ypos <= center.y + height)){
			return tBig;
		}else{
			return -1.0;
	}
}

}
/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cone.
*/
glm::vec3 Cone::normal(glm::vec3 p)
{
	glm::vec3 d = p-center;
    float r = sqrt(d.x * d.x + d.z * d.z);
    glm::vec3 n= glm::vec3 (d.x, r*(radius/height), d.z);
    n=glm::normalize(n);
    return n;
}
