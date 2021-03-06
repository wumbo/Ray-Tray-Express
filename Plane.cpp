/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Plane class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Plane.h"
#include "Vector.h"
#include <math.h>
#include <iostream>



//Function to test if an input point is within the quad.
bool Plane::isInside(Vector q)
{
	Vector n = normal(q, Vector());
	Vector ua = b-a,  ub = c-b, uc = d-c, ud = a-d;
	Vector va = q-a,  vb = q-b, vc = q-c, vd = q-d;
	
    if (((ua.cross(va)).dot(n) > 0) &&
        ((ub.cross(vb)).dot(n) > 0) &&
        ((uc.cross(vc)).dot(n) > 0) &&
        ((ud.cross(vd)).dot(n) > 0))
    {
        return true;
    }
    else {
        return false;
    }
}

//Function to compute the paramter t at the point of intersection.
float Plane::intersect(Vector pos, Vector dir)
{
	Vector n = normal(pos, Vector());
	Vector vdif = a-pos;
	float vdotn = dir.dot(n);
	if(fabs(vdotn) < 1.e-4) return -1;
    float t = vdif.dot(n)/vdotn;
	if(fabs(t) < 0.0001) return -1;
	Vector q = pos + dir*t;
	if(isInside(q)) return t;
    else return -1;
}

// Function to compute the unit normal vector
// Remember to output a normalised vector!
Vector Plane::normal(Vector pos, Vector src)
{
    Vector bMinusA = b - a;
    Vector cMinusA = c - a;
    Vector n = bMinusA.cross(cMinusA);
    n.normalise();
    
    return n;
}

Color Plane::getColor(Vector point)
{
    if (pattern == 0) {
        return color;
    } else {
        bool first;
        if (((int)(b.x - point.x) % pattern->size)  == 0) {
            first = true;
        } else {
            first = false;
        }
        
        if (((int)(b.z - point.z) % pattern->size)  == 0) {
            first = !first;
        }
        
        return first ? pattern->col1 : pattern->col2;
    }
}



