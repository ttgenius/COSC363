/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The ray class
-------------------------------------------------------------*/

#ifndef H_RAY
#define H_RAY
#include <glm/glm.hpp>
#include <vector>
#include "SceneObject.h"

class Ray
{

public:
    glm::vec3 pt;	//The source point of the ray
	glm::vec3 dir;	//The UNIT direction of the ray
	glm::vec3 xpt;	//The closest point of intersection on the ray
	int xindex;	//The index of the object that gives the closet point of intersection
	float xdist;	//The distance from the source to xpt along the ray.

    Ray()
	{
		pt = glm::vec3(0, 0, 0);
		dir = glm::vec3(0, 0, -1);
		xpt = glm::vec3(0, 0, 0);
		xindex = -1;
		xdist = 0;
	}	;
	
    Ray(glm::vec3 point, glm::vec3 direction)
		: pt(point), dir(direction)
	{
		xpt = glm::vec3(0, 0, 0);
		xindex = -1;
		xdist = 0;
	} ;

    void normalize();
	void closestPt(std::vector<SceneObject*> &sceneObjects);

};
#endif
