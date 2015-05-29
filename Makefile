CC=clang++
IDIR=/opt/X11/include/
LDIR=/opt/X11/lib/

CFLAGS=-c -Wall -I$(IDIR)
LDFLAGS=-lGL -lGLU -lglut -L$(LDIR)
    
all: RayTracer

RayTracer: RayTracer.o Color.o Object.o Plane.o Sphere.o Vector.o
	$(CC) $(LDFLAGS) RayTracer.o Color.o Object.o Plane.o Sphere.o Vector.o -o RayTracer

RayTracer.o: RayTracer.cpp
	$(CC) $(CFLAGS) RayTracer.cpp

Color.o: Color.cpp
	$(CC) $(CFLAGS) Color.cpp

Object.o: Object.cpp
	$(CC) $(CFLAGS) Object.cpp

Sphere.o: Sphere.cpp
	$(CC) $(CFLAGS) Sphere.cpp

Vector.o: Vector.cpp
	$(CC) $(CFLAGS) Vector.cpp

Plane.o: Plane.cpp
	$(CC) $(CFLAGS) Plane.cpp

clean:
	rm *.o RayTracer
