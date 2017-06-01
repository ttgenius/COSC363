/*----------------------------------------------------------
* COSC 363 Assignment 2 Ray Tracer
* Date: 24/05/2017
* Author: Melody Zhu
* ID: 97245310
*
* The cylinder class
* This is a subclass of Object, and hence implements the
* methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cylinder.h"
#include <math.h>

float Cylinder::intersect(glm::vec3 pos, glm::vec3 dir)
{
	glm::vec3 d = pos - center;
	float a = (dir.x * dir.x) + (dir.z * dir.z);
    float b = 2 * (dir.x * d.x + dir.z * d.z);
    float c = d.x * d.x + d.z * d.z - (radius * radius);
    
    float delta = b*b - 4*(a*c);
	
	if(delta < 0.0){
		return -1.0;
    }
    
    if((fabs(delta)) < 0.001)
	{
		return -1.0;
	}
		 
    float tSmall;
    float tBig;
    float t1 = (-b - sqrt(delta))/(2 * a);
    float t2 = (-b + sqrt(delta))/(2 * a);
 
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


glm::vec3 Cylinder::normal(glm::vec3 p)
{
	glm::vec3 d = p - center;
	glm::vec3 n = glm::vec3 (d.x,0,d.z);
	n = glm::normalize(n); //normalize
	return n;

}
