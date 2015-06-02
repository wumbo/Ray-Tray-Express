//
//  Square.h
//  RayTracer1
//
//  Created by Simon Crequer on 2/06/15.
//  Copyright (c) 2015 Simon Crequer. All rights reserved.
//

#ifndef __RayTracer1__Square__
#define __RayTracer1__Square__

#include <stdio.h>
#include "Vector.h"
#include "Plane.h"

class Square : public Object
{
    
private:
    Vector center;
    float size;
    Plane planes[6];
    
public:
    
    Square()
    : center(Vector()), size(1)  //Default constructor creates a unit square
    {
        color = Color::WHITE;
        
        // Generate points
        Vector points[8];
        points[0] = Vector(center.x-size/2, center.y-size/2, center.z+size/2); // front bottom left
        points[1] = Vector(center.x+size/2, center.y-size/2, center.z+size/2); // front bottom right
        points[2] = Vector(center.x+size/2, center.y+size/2, center.z+size/2); // front top right
        points[3] = Vector(center.x-size/2, center.y+size/2, center.z+size/2); // front top left
        points[4] = Vector(center.x-size/2, center.y-size/2, center.z-size/2); // back bottom left
        points[5] = Vector(center.x+size/2, center.y-size/2, center.z-size/2); // back bottom right
        points[6] = Vector(center.x+size/2, center.y+size/2, center.z-size/2); // back top right
        points[7] = Vector(center.x-size/2, center.y+size/2, center.z-size/2); // back top left
        // Generate planes
        planes[0] = Plane(points[0], points[1], points[2], points[3], color); // front
        planes[1] = Plane(points[1], points[5], points[6], points[2], color); // right
        planes[2] = Plane(points[5], points[4], points[7], points[6], color); // back
        planes[3] = Plane(points[4], points[0], points[3], points[7], color); // left
        planes[4] = Plane(points[4], points[5], points[1], points[0], color); // bottom
        planes[5] = Plane(points[3], points[2], points[6], points[7], color); // top
    };
    
    Square(Vector c, float s, Color col)
    : center(c), size(s)
    {
        color = col;
        
        // Generate points
        Vector points[8];
        points[0] = Vector(center.x-size/2, center.y-size/2, center.z+size/2); // front bottom left
        points[1] = Vector(center.x+size/2, center.y-size/2, center.z+size/2); // front bottom right
        points[2] = Vector(center.x+size/2, center.y+size/2, center.z+size/2); // front top right
        points[3] = Vector(center.x-size/2, center.y+size/2, center.z+size/2); // front top left
        points[4] = Vector(center.x-size/2, center.y-size/2, center.z-size/2); // back bottom left
        points[5] = Vector(center.x+size/2, center.y-size/2, center.z-size/2); // back bottom right
        points[6] = Vector(center.x+size/2, center.y+size/2, center.z-size/2); // back top right
        points[7] = Vector(center.x-size/2, center.y+size/2, center.z-size/2); // back top left
        // Generate planes
        planes[0] = Plane(points[0], points[1], points[2], points[3], color); // front
        planes[1] = Plane(points[1], points[5], points[6], points[2], color); // right
        planes[2] = Plane(points[5], points[4], points[7], points[6], color); // back
        planes[3] = Plane(points[4], points[0], points[3], points[7], color); // left
        planes[4] = Plane(points[4], points[5], points[1], points[0], color); // bottom
        planes[5] = Plane(points[3], points[2], points[6], points[7], color); // top
    };
    
    float intersect(Vector pos, Vector dir);
    
    Vector normal(Vector p, Vector src);
    
};

#endif /* defined(__RayTracer1__Square__) */
