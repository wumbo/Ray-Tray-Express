CC=clang++
IDIR=/opt/X11/include/
LDIR=/opt/X11/lib/

CFLAGS=-c -Wall -I$(IDIR)
LDFLAGS=-lGL -lGLU -lglut -L$(LDIR)

AA=1

EXEC = RayTracer
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CC) -c -DANTI_ALIASING=$(AA) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXEC)
