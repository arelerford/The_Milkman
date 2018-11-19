
#ifndef DEFAULT_H_
#define DEFAULT_H_

#include "log.h"

typedef double Vec[3];
typedef double Flt;
typedef struct t_button {
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
} Button;


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

class Sprite {
public:
	int onoff, frame;
	double delay;
	Vec pos;
	GLuint tex;
	Image *image;
	struct timespec time;
	
	Sprite(); 
};

class Global {
public:
	Button button[4];
	int nbuttons;
	unsigned char keys[65536];
	int xres, yres;
	int movie, movieStep;
	int walk;
	int walkFrame;
	double delay;
	Image *walkImage;
	GLuint walkTexture;
	Vec box[20];
	Sprite exp;
	Sprite exp44;
	Vec ball_pos;
	Vec ball_vel;
	//camera is centered at (0,0) lower-left of screen. 
	Flt camera[2];

	~Global() { logClose(); }
	Global();
};

class Level {
public:
	unsigned char arr[16][80];
	int nrows, ncols;
	int tilesize[2];
	Flt ftsz[2];
	Flt tile_base;

	Level();
	void removeCrLf(char*);
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
