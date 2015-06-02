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

#ifndef H_OBJECT
#define H_OBJECT

#include "Vector.h"
#include "Color.h"

struct Pattern
{
    Color col1;
    Color col2;
    int size; // Width of checkered squares
};

class Object 
{
protected:
	Color color;
public:
    Pattern *pattern;
    float transparency;
    float refractionIndex;
    float reflectionCoefficient;
	Object()
    {
        transparency = 0;
        refractionIndex = 1;
        reflectionCoefficient = 0;
    }
    virtual float intersect(Vector pos, Vector dir) = 0;
	virtual Vector normal(Vector pos, Vector src) = 0;
	virtual ~Object() {}
    virtual Color getColor(Vector point);
	void setColor(Color col);
};

#endif