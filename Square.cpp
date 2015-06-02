//
//  Square.cpp
//  RayTracer1
//
//  Created by Simon Crequer on 2/06/15.
//  Copyright (c) 2015 Simon Crequer. All rights reserved.
//

#include "Square.h"
#include <math.h>
#include <algorithm>

float Square::intersect(Vector pos, Vector dir)
{
    float t[6];
    t[0] = planes[0].intersect(pos, dir);
    t[1] = planes[1].intersect(pos, dir);
    t[2] = planes[2].intersect(pos, dir);
    t[3] = planes[3].intersect(pos, dir);
    t[4] = planes[4].intersect(pos, dir);
    t[5] = planes[5].intersect(pos, dir);

    float min = -1;
    for (int i = 0; i < 6; i++) {
        if ((t[i] > 0) && (min == -1 || t[i] < min)) {
            min = t[i];
        }
    }
    
    return min;
}

Vector Square::normal(Vector p, Vector src)
{
    for (int i = 0; i < 6; i++) {
        if (planes[i].isInside(p)) {
            return planes[i].normal(p, src);
        }
    }
}