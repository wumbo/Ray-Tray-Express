CC=clang++
IDIR=/opt/X11/include/
LDIR=/opt/X11/lib/

CFLAGS=-c -Wall -I$(IDIR)
LDFLAGS=-lGL -lGLU -lglut -L$(LDIR)

EXEC = RayTracer
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: RayTracer

$(EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXEC)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXEC)
