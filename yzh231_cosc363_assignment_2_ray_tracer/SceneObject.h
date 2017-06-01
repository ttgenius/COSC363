/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The object class
*  This is a generic type for storing objects in the scene.
*  Being an abstract class, this class cannot be instantiated.
*  Sphere, Plane etc, must be defined as subclasses of Object
*      and provide implementations for the virtual functions
*      intersect()  and normal().
-------------------------------------------------------------*/

#ifndef H_SOBJECT
#define H_SOBJECT
#include <glm/glm.hpp>


class SceneObject
{
protected:
	glm::vec3 color;
public:
	SceneObject() {}
    virtual float intersect(glm::vec3 pos, glm::vec3 dir) = 0;
	virtual glm::vec3 normal(glm::vec3 pos) = 0;
	virtual ~SceneObject() {}
	glm::vec3 getColor();
	void setColor(glm::vec3 col);
};

#endif
