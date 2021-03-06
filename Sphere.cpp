/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Sphere.h"
#include <math.h>

/**
* Sphere's intersection method.  The input is a ray (pos, dir). 
*/
float Sphere::intersect(Vector pos, Vector dir)
{
    Vector vdif = pos - center;
    float b = dir.dot(vdif);
    float len = vdif.length();
    float c = len*len - radius*radius;
    float delta = b*b - c;
   
	if(fabs(delta) < 0.001) return -1.0;
    if(delta < 0.0) return -1.0;

    float t1 = -b - sqrt(delta);
    float t2 = -b + sqrt(delta);
    if(fabs(t1) < 0.001 )
    {
        if (t2 > 0) return t2;
        else t1 = -1.0;
    }
    if(fabs(t2) < 0.001 ) t2 = -1.0;

	return (t1 < t2)? t1: t2;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
Vector Sphere::normal(Vector p, Vector src)
{
    Vector n = p - center;
    n.normalise();
    return n;
}

Color Sphere::getColor(Vector point)
{
    if (pattern == 0) {
        return color;
    } else {
        if ((int)(point.x * pattern->size) % 2 == 0) {
            return pattern->col1;
        } else {
            return pattern->col2;
        }
    }
}
