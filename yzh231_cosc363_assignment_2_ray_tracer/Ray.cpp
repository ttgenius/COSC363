/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The ray class
-------------------------------------------------------------*/
#include "Ray.h"


//Normalizes the direction vector of the current ray to a unit vector
void Ray::normalize()
{
   dir = glm::normalize(dir);
}

//Finds the closest point of intersection of the current ray with scene objects
void Ray::closestPt(std::vector<SceneObject*> &sceneObjects)
{
	glm::vec3 point(0,0,0);
	float min = 1.e+6;
    for(int i = 0;  i < (int) sceneObjects.size();  i++)
	{
        float t = sceneObjects[i]->intersect(pt, dir);
		if(t > 0)        //Intersects the object
		{
			point = pt + dir*t;
			if(t < min)
			{
				xpt = point;
				xindex = i;
				xdist = t;
				min = t;
			}
		}
	}
}

