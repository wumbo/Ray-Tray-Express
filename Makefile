CC=clang++
IDIR=/opt/X11/include/
LDIR=/opt/X11/lib/

CFLAGS=-c -Wall -I$(IDIR)
LDFLAGS=-lGL -lGLU -lglut -L$(LDIR)
    
all: RayTracer

RayTracer: RayTracer.o Color.o Cylinder.o Object.o Plane.o Sphere.o Square.o Vector.o
	$(CC) $(LDFLAGS) RayTracer.o Color.o Cylinder.o Object.o Plane.o Sphere.o Square.o Vector.o -o RayTracer

RayTracer.o: RayTracer.cpp
	$(CC) $(CFLAGS) RayTracer.cpp

Color.o: Color.cpp Color.h
	$(CC) $(CFLAGS) Color.cpp

Cylinder.o: Cylinder.cpp Cylinder.h
	$(CC) $(CFLAGS) Cylinder.cpp

Object.o: Object.cpp Object.h
	$(CC) $(CFLAGS) Object.cpp

Sphere.o: Sphere.cpp Sphere.h
	$(CC) $(CFLAGS) Sphere.cpp

Square.o: Square.cpp Square.h Plane.o
	$(CC) $(CFLAGS) Square.cpp

Vector.o: Vector.cpp Vector.h
	$(CC) $(CFLAGS) Vector.cpp

Plane.o: Plane.cpp Plane.h
	$(CC) $(CFLAGS) Plane.cpp

clean:
	rm *.o RayTracer
