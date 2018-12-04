
#ifndef DEFAULT_H_
#define DEFAULT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <ctime>
#include "log.h"

//function prototypes
void initOpengl();
void physics();
void render();
int checkKeys(XEvent *e);

class Timers {
public:
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
	struct timespec walkTime;
	
	Timers();
	double timeDiff(struct timespec*, struct timespec*);
	void timeCopy(struct timespec*, struct timespec*);
	void recordTime(struct timespec*);
};

class Image {
public:
	int width, height;
	unsigned char *data;
	
	~Image() { delete [] data; }
	Image(const char*);
	void init(const char*);
};

class Global {
public:
	unsigned char keys[65536];
	int xres, yres;
	double fps;

	~Global() { logClose(); }
	Global();
};

class X11_wrapper {
	Display *dpy;
	Window win;

public:
	~X11_wrapper();
	X11_wrapper();
	void setTitle();
	void setupScreenRes(const int, const int);
	void reshapeWindow(int, int);
	void checkResize(XEvent*);
	XEvent getXNextEvent();
	void swapBuffers();
	bool getXPending() { return XPending(dpy); }
};

unsigned char *buildAlphaData(Image*);

unsigned char *buildAlphaCustom(Image*, unsigned char red,
							    unsigned char green, unsigned char blue);


#endif
