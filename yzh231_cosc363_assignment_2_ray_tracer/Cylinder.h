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

#ifndef H_CYLINDER
#define H_CYLINDER

#include <glm/glm.hpp>
#include "SceneObject.h"

/**
 * Defines a simple Cylinder located at 'center' 
 * with the specified radius
 */
class Cylinder : public SceneObject
{

private:
    glm::vec3 center;
    float radius;
    float height;

public:	
	Cylinder()
		: center(glm::vec3(0)), radius(1), height(1)
	{
		color = glm::vec3(1);
	};
	
    Cylinder(glm::vec3 c, float r, float h, glm::vec3 col)
		: center(c), radius(r), height(h)
	{
		color = col;
	};

	float intersect(glm::vec3 pos, glm::vec3 dir);

	glm::vec3 normal(glm::vec3 p);

};

#endif //!H_CYLINDER
