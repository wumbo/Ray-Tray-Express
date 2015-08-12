// ========================================================================
// COSC 363  Computer Graphics  Lab07
// A simple ray tracer
// ========================================================================

#include <iostream>
#include <cmath>
#include <vector>
#include "Vector.h"
#include "Sphere.h"
#include "Square.h"
#include "Color.h"
#include "Object.h"
#include "Plane.h"
#include "Cylinder.h"
#include <GL/glut.h>

using namespace std;

const float WIDTH = 26.0;
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int PPU = 30;
const int MAX_STEPS = 10;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<Object*> sceneObjects;

Vector light = Vector(15, 40, 15);
Color backgroundCol;

//A useful struct
struct PointBundle
{
	Vector point;
	int index;
	float dist;
};

/*
* This function compares the given ray with all objects in the scene
* and computes the closest point  of intersection.
*/
PointBundle closestPt(Vector pos, Vector dir)
{
    Vector  point(0, 0, 0);
	float min = 10000.0;

	PointBundle out = {point, -1, 0.0};

    for(int i = 0;  i < sceneObjects.size();  i++)
	{
        float t = sceneObjects[i]->intersect(pos, dir);
		if(t > 0)        //Intersects the object
		{
			point = pos + dir*t;
			if(t < min)
			{
				out.point = point;
				out.index = i;
				out.dist = t;
				min = t;
			}
		}
	}

	return out;
}

/*
* Computes the colour value obtained by tracing a ray.
* If reflections and refractions are to be included, then secondary rays will 
* have to be traced from the point, by converting this method to a recursive
* procedure.
*/

Color trace(Vector pos, Vector dir, int step, bool insideSphere = false)
{
    PointBundle q = closestPt(pos, dir);

    if(q.index == -1) return backgroundCol;        //no intersection

    Color col = sceneObjects[q.index]->getColor(q.point); //Object's colour
    Vector n = sceneObjects[q.index]->normal(q.point, pos); // Normal vector
    float transparency = sceneObjects[q.index]->transparency;
    float refractionIndex = sceneObjects[q.index]->refractionIndex;
    float reflectionCoefficient = sceneObjects[q.index]->reflectionCoefficient;
    
    Vector l = light - q.point;
    l.normalise();
    float lDotn = l.dot(n);
    
    Vector lightVector = light - q.point;
    float lightDist = lightVector.length();
    lightVector.normalise();
    PointBundle s = closestPt(q.point, lightVector);
    
    Color colorSum;
    Vector r = ((n * 2) * lDotn) - l;
    r.normalise();
    Vector v(-dir.x, -dir.y, -dir.z);

    if (lDotn <= 0 || (s.index > -1 && s.dist < lightDist)) {
        colorSum = col.phongLight(backgroundCol, 0.0, 0.0);
    } else {

        float rDotv = r.dot(v);
        float spec;
        if (rDotv < 0) {
            spec = 0.0;
        } else {
            spec = pow(rDotv, 100);
        }
        
        colorSum = col.phongLight(backgroundCol, lDotn, spec);
    }
    
    // Reflections
    if ((reflectionCoefficient > 0) && step < MAX_STEPS) {
        Vector reflectionVector = ((n*2)* (n.dot(v))) - v;
        
        reflectionVector.normalise();
        Color reflectionCol = trace(q.point, reflectionVector, step+1);
        colorSum.combineColor(reflectionCol, reflectionCoefficient);
    }
    
    // Refractions
    if ((transparency > 0) && step < MAX_STEPS) {
        if (insideSphere) {
            //refractionIndex = 1 / refractionIndex;
            n *= -1;
        }
        
        /*if (step == 2) {
         return Color::PINK;
         }*/
        
        float cosTheta = sqrt(1 - pow(refractionIndex, 2) * (1 - pow(dir.dot(n), 2)));
        Vector refractionVector = (dir * refractionIndex) - n * (refractionIndex *
                                                                 dir.dot(n) + cosTheta);
        refractionVector.normalise();
        Color refractionCol;
        if (insideSphere || dynamic_cast<Sphere*>(sceneObjects[q.index]) == NULL) {
            refractionCol = trace(q.point, refractionVector, step+1);
        } else {
            refractionCol = trace(q.point, refractionVector, step+1, true);
        }
        colorSum.combineColor(refractionCol, transparency);
        
    }
    
    return colorSum;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each pixel as quads.
//---------------------------------------------------------------------------------------
void display()
{
	int widthInPixels = (int)(WIDTH * PPU);
	int heightInPixels = (int)(HEIGHT * PPU);
	float pixelSize = 1.0/PPU;
	float halfPixelSize = pixelSize/2.0;
	float x1, y1, xc, yc;
	Vector eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);  //Each pixel is a quad.

	for(int i = 0; i < widthInPixels; i++)	//Scan every "pixel"
	{
		x1 = XMIN + i*pixelSize;
		xc = x1 + halfPixelSize;
		for(int j = 0; j < heightInPixels; j++)
		{
			y1 = YMIN + j*pixelSize;
			yc = y1 + halfPixelSize;
            
            if (ANTI_ALIASING) {
                Vector dir = Vector(xc - halfPixelSize/2, yc - halfPixelSize/2, -EDIST);	//direction of the primary ray
                dir.normalise();			//Normalise this direction
                Color col1 = trace (eye, dir, 1); //Trace the primary ray and get the colour value
                
                dir = Vector(xc - halfPixelSize/2, yc + halfPixelSize/2, -EDIST);	//direction of the primary ray
                dir.normalise();			//Normalise this direction
                Color col2 = trace (eye, dir, 1); //Trace the primary ray and get the colour value
                
                dir = Vector(xc + halfPixelSize/2, yc - halfPixelSize/2, -EDIST);	//direction of the primary ray
                dir.normalise();			//Normalise this direction
                Color col3 = trace (eye, dir, 1); //Trace the primary ray and get the colour value
                
                dir = Vector(xc + halfPixelSize/2, yc + halfPixelSize/2, -EDIST);	//direction of the primary ray
                dir.normalise();			//Normalise this direction
                Color col4 = trace (eye, dir, 1); //Trace the primary ray and get the colour value
                
                float red = (col1.r + col2.r + col3.r + col4.r) / 4;
                float green = (col1.g + col2.g + col3.g + col4.g) / 4;
                float blue = (col1.b + col2.b + col3.b + col4.b) / 4;
                
                glColor3f(red, green, blue);
            } else {
                Vector dir = Vector(xc, yc, -EDIST);	//direction of the primary ray
                dir.normalise();			//Normalise this direction
                Color col = trace (eye, dir, 1); //Trace the primary ray and get the colour value
                
                glColor3f(col.r, col.g, col.b);
            }
            
			glVertex2f(x1, y1);				//Draw each pixel with its color value
			glVertex2f(x1 + pixelSize, y1);
			glVertex2f(x1 + pixelSize, y1 + pixelSize);
			glVertex2f(x1, y1 + pixelSize);
        }
    }

    glEnd();
    glFlush();
}



void initialize()
{
    backgroundCol = Color(0.2, 0.2, 0.2);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(backgroundCol.r, backgroundCol.g, backgroundCol.b, 1);
    
    Plane *floor = new Plane(Vector(-20, -10, 1),
                             Vector(20, -10, 1),
                             Vector(20., -10, -100),
                             Vector(-20., -10, -100),
                             Color(0.5, 0.5, 0.5));
    Pattern *p = new Pattern();
    p->col1 = Color(0.0, 0.0, 0.0);
    p->col2 = Color::WHITE;
    p->size = 3;
    floor->pattern = p;
    floor->reflectionCoefficient = 0.5;
    
    Plane *left = new Plane(Vector(-20, -10, 1),
                            Vector(-20, -10, -100),
                            Vector(-20, 20, -100),
                            Vector(-20, 20, 1),
                            Color(1, 0.95, 0.85));
    
    Plane *right = new Plane(Vector(20, -10, -100),
                             Vector(20, -10, 1),
                             Vector(20, 20, 1),
                             Vector(20, 20, -100),
                             Color(1, 0.95, 0.85));
    
    Plane *back = new Plane(Vector(-20, -10, -100),
                            Vector(20, -10, -100),
                            Vector(20, 20, -100),
                            Vector(-20, 20, -100),
                            Color::BLACK);
    back->reflectionCoefficient = 1;
    
    Square *square1 = new Square(Vector(-6, -8, -45), 4, Color::GREEN);
    Square *square2 = new Square(Vector(-6, -4, -45), 4, Color::BLUE);
    square1->transparency = 0.8;
    square2->transparency = 0.8;
    Cylinder *cylinder1 = new Cylinder(Vector(-12, -10, -55), 8.0, 1.0, Color(0.8, 0.6, 0.15));
    cylinder1->reflectionCoefficient = 0.07;
    Cylinder *cylinder2 = new Cylinder(Vector(12, -10, -55), 8.0, 1.0, Color(0.8, 0.6, 0.15));
    cylinder2->reflectionCoefficient = 0.07;
    Sphere *sphere1 = new Sphere(Vector(-12, -0.5, -55), 2.0, Color::BLACK);
    Pattern *p1 = new Pattern();
    p1->col1 = Color(0.8, 0.6, 0.15);
    p1->col2 = Color::GRAY;
    p1->size = 3;
    sphere1->pattern = p1;
    sphere1->reflectionCoefficient = 0.1;
    Sphere *sphere2 = new Sphere(Vector(12, -0.5, -55), 2.0, Color::BLACK);
    sphere2->pattern = p1;
    sphere2->reflectionCoefficient = 0.1;
    
    Sphere *sphere3 = new Sphere(Vector(0, -6, -60), 4, Color::BLACK);
    sphere3->reflectionCoefficient = 1;
    Sphere *sphere4 = new Sphere(Vector(8, -7, -45), 3, Color(0.5, 0, 0));
    sphere4->refractionIndex = 1.03;
    sphere4->transparency = 1;
    
    sceneObjects.push_back(floor);
    sceneObjects.push_back(left);
    sceneObjects.push_back(right);
    sceneObjects.push_back(back);
    sceneObjects.push_back(square1);
    sceneObjects.push_back(square2);
    sceneObjects.push_back(cylinder1);
    sceneObjects.push_back(cylinder2);
    sceneObjects.push_back(sphere1);
    sceneObjects.push_back(sphere2);
    sceneObjects.push_back(sphere3);
    sceneObjects.push_back(sphere4);
}


int main(int argc, char *argv[]) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(780, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Ray Tray Express");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
