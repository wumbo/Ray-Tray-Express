/*----------------------------------------------------------
 * COSC363  Ray Tracer
 *
 *  The cylinder class
 *  This is a subclass of Object, and hence implements the
 *  methods intersect() and normal().
 -------------------------------------------------------------*/

#include "Cylinder.h"
#include <math.h>
#include <iostream>

/**
 * Cylinder's intersection method.  The input is a ray (pos, dir).
 */
float Cylinder::intersect(Vector pos, Vector dir)
{
    // First term of quadratic
    float a = pow(dir.x, 2) + pow(dir.z, 2);
    // Second term of quadratic
    float b = 2 * (dir.x * (pos.x - center.x) +
                   dir.z * (pos.z - center.z));
    // Third term of quadratic
    float c = (pow(pos.x - center.x, 2) +
               pow(pos.z - center.z, 2) -
               pow(radius, 2));
    
    // Discriminant of quadratic
    float d = pow(b, 2) - 4 * a * c;
    
    if(fabs(d) < 0.001) return -1.0;
    if(d < 0.0) return -1.0;
    
    // First solution of quadratic
    float t1 = (-b - sqrt(d)) / (2 * a);
    // Second solution of quadratic
    float t2 = (-b + sqrt(d)) / (2 * a);
    
    if(fabs(t1) < 0.001 )
    {
        if (t2 > 0) return t2;
        else t1 = -1.0;
    }
    if(fabs(t2) < 0.001 ) t2 = -1.0;
    
    if (t1 > 0 && t2 < 0) {
        Vector i = pos + dir * t1;
        if (i.y > center.y + height || i.y < center.y) {
            return -1;
        }
    } else if (t2 > 0 && t1 < 0) {
        Vector i = pos + dir * t2;
        if (i.y > center.y + height || i.y < center.y) {
            return -1;
        }
    } else if (t1 > 0 && t2 > 0) {
        Vector i1, i2;
        i1 = pos + dir * (t1 < t2 ? t1 : t2);
        i2 = pos + dir * (t1 > t2 ? t1 : t2);
        if (dir.y < 0 && i1.y > center.y + height && i2.y < center.y + height) {
            return t1 > t2 ? t1 : t2;
        } else if (dir.y > 0 && i1.y < center.y && i2.y > center.y) {
            return t1 > t2 ? t1 : t2;
        } else if ((i1.y > center.y + height && i2.y > center.y + height) ||
                   (i1.y < center.y && i2.y < center.y)) {
            return -1;
        }
    }
    
    return (t1 < t2)? t1: t2;
}

/**
 * Returns the unit normal vector at a given point.
 * Assumption: The input point p lies on the cylinder.
 */
Vector Cylinder::normal(Vector p)
{
    Vector n = p - center;
    n.y = 0;
    n.normalise();
    return n;
}