/*----------------------------------------------------------
 * COSC363  Ray Tracer
 *
 *  The cylinder class
 *  This is a subclass of Object, and hence implements the
 *  methods intersect() and normal().
 -------------------------------------------------------------*/

#ifndef H_CYLINDER
#define H_CYLINDER

#include "Object.h"

/**
 * Defines a simple upright Cylinder with the bottom center positioned
 * at center, with the specified height and radius
 */
class Cylinder : public Object
{
    
private:
    Vector center;
    float height;
    float radius;
    
public:
    Cylinder()
    : center(Vector()), height(1), radius(1)
    {
        color = Color::WHITE;
    };
    
    Cylinder(Vector c, float h, float r, Color col)
    : center(c), height(h), radius(r)
    {
        color = col;
    };
    
    float intersect(Vector pos, Vector dir);
    
    Vector normal(Vector p);
    
};

#endif