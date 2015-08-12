# Ray-Tray-Express
A ray tracing application with reflections, shadows, anti-aliasing, and more!

## Building
To build, you need to have GLUT installed.

### Mac OS X
The Makefile is configured to be built on Mac (with a Macports GLUT install) by default, so you can build the application by running:

`make`

### Linux
To build on Linux, you need to specify your C++ compiler when running the `make` command. The following should work on Ubuntu-based systems:

`make CC=g++`

### Additional Options

#### Anti-Aliasing
By default, anti-aliasing is turned on. You can specify whether or not you want it using the make macro `AA`. This will turn it off:

`make AA=0`

If you leave anti-aliasing on, the scene may take over a minute to render, depending on your machine.

## Running

After the application has been built, you can run it using:

`./RayTracer`

The scene could take a few seconds to a few minutes, depending on whether anti-aliasing is turned on and how fast your machine is.

## Cleaning

You can delete the executable and object files by running:

`make clean`
