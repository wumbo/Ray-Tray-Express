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

#define ANTI_ALIASING 1

using namespace std;

const float WIDTH = 26.0;
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int PPU = 30;     //Total 600x600 pixels
const int MAX_STEPS = 10;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<Object*> sceneObjects;

Vector light = Vector(10.0, 40.0, -10.0);
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

    Color col = sceneObjects[q.index]->getColor(); //Object's colour
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
            spec = pow(rDotv, 10);
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
        //refractionVector.normalise();
        Color refractionCol;
        if (insideSphere || dynamic_cast<Sphere*>(sceneObjects[q.index]) == NULL) {
            refractionCol = trace(q.point, refractionVector, step+1);
        } else {
            refractionCol = trace(q.point, refractionVector, step+1, true);
        }
        //if (step % 2 == 1) {
        colorSum.combineColor(refractionCol, transparency);
        //}
        
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
    backgroundCol = Color(0.1, 0.1, 0.1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(backgroundCol.r, backgroundCol.g, backgroundCol.b, 1);
    
    Plane *floor = new Plane(Vector(-20, -10, -25),
                             Vector(20, -10, -25),
                             Vector(20., -10, -100),
                             Vector(-20., -10, -100),
                             Color(0.5, 0.5, 0.5));
    
    Plane *back = new Plane(Vector(-20, -10, -100),
                            Vector(20, -10, -100),
                            Vector(20, 30, -100),
                            Vector(-20, 30, -100),
                            Color::BLACK);
    back->reflectionCoefficient = 1;
    
    Square *square1 = new Square(Vector(0, -6, -60), 12.0, Color::BLUE);
    square1->transparency = 0.9;
    square1->refractionIndex = 1.5;
    Cylinder *cylinder1 = new Cylinder(Vector(0, -10, -90), 8.0, 3.0, Color::YELLOW);
    Sphere *sphere1 = new Sphere(Vector(-3, -7, -40), 3.0, Color(0.5, 0.5, 0.5));
    sphere1->transparency = 0.8;
    sphere1->refractionIndex = 1.1;
    
    sceneObjects.push_back(floor);
    sceneObjects.push_back(back);
    sceneObjects.push_back(square1);
    sceneObjects.push_back(cylinder1);
    sceneObjects.push_back(sphere1);
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
