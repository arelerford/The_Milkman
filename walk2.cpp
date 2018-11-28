//3350
//program: walk2.cpp
//author:  Gordon Griesel
//date:    summer 2017
//         spring 2018
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//
//This program includes:
//  multiple sprite-sheet animations
//  a level tiling system
//  parallax scrolling of backgrounds
//
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
#include "fonts.h"
#include "Entity.h"
#include "default.h"
#include "isaacL.h"
#include "austinR.h"
#include "CollisonManager.h"
#include "Player.h"

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(v, x, y, z) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
const int Max_BULLETS = 5;
#define ALPHA 1

//function prototypes
void initOpengl();
int checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics();
void render();
int defaultKeys(int);
class Image;

extern float convert_pixel_position (float, float);

//
struct Coord {
    float x, y;
};

//start menu prototypes 
void startMenu();
void creditScreen();

//-----------------------------------------------------------------------------
//Setup timers
Timers::Timers()
{
	this->physicsRate = 1.0 / 30.0;
	this->oobillion = 1.0 / 1e9;
};

double Timers::timeDiff(struct timespec *start, struct timespec *end)
{
	return (double)(end->tv_sec - start->tv_sec ) +
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
};

void Timers::timeCopy(struct timespec *dest, struct timespec *source)
{
	memcpy(dest, source, sizeof(struct timespec));
};

void Timers::recordTime(struct timespec *t)
{
	clock_gettime(CLOCK_REALTIME, t);
};

Timers timers;
//-----------------------------------------------------------------------------
// Setup Sprite
Sprite::Sprite()
{
	this->onoff = 0;
	this->frame = 0;
	this->image = NULL;
};
//-----------------------------------------------------------------------------
// SetUp Global
Global::Global()
{
	logOpen();
	camera[0] = camera[1] = 0.0;
	movie=0;
	movieStep=2;
	xres=800;
	yres=600;
	walk=0;
	walkFrame=0;
	walkImage=NULL;
	bottleThrow.frame=0;
	bottleThrow.image=NULL;
	MakeVector(ball_pos, 520.0, 0, 0);
	MakeVector(ball_vel, 0, 0, 0);
	delay = 0.1;
	exp.onoff=0;
	exp.frame=0;
	exp.image=NULL;
	exp.delay = 0.02;
	exp44.onoff=0;
	exp44.frame=0;
	exp44.image=NULL;
	exp44.delay = 0.022;
	for (int i=0; i<20; i++) {
		box[i][0] = rnd() * xres;
		box[i][1] = rnd() * (yres-220) + 220.0;
		box[i][2] = 0.0;
	}
	memset(keys, 0, 65536);	
};
Global gl;
//-----------------------------------------------------------------------------
// SetUp Level
Level::Level()
{
	//Log("Level constructor\n");
	tilesize[0] = 32;
	tilesize[1] = 32;
	ftsz[0] = (Flt)tilesize[0];
	ftsz[1] = (Flt)tilesize[1];
	tile_base = 220.0;
	//read level
	FILE *fpi = fopen("level1.txt","r");
	if (fpi) {
		nrows=0;
		char line[100];
		while (fgets(line, 100, fpi) != NULL) {
			removeCrLf(line);
			int slen = strlen(line);
			ncols = slen;
			//Log("line: %s\n", line);
			for (int j=0; j<slen; j++) {
				arr[nrows][j] = line[j];
			}
			++nrows;
		}
		fclose(fpi);
		//printf("nrows of background data: %i\n", nrows);
	}
	// for (int i=0; i<nrows; i++) {
	// 	for (int j=0; j<ncols; j++) {
	// 		printf("%c", arr[i][j]);
	// 	}
	// 	printf("\n");
	// }	
};

void Level::removeCrLf(char *str)
{
	//remove carriage return and linefeed from a Cstring
	char *p = str;
	while (*p) {
		if (*p == 10 || *p == 13) {
			*p = '\0';
			break;
		}
		++p;
	}
};
Level lev;
//-----------------------------------------------------------------------------
//X Windows variables
X11_wrapper::~X11_wrapper()
{
	XDestroyWindow(dpy, this->win);
	XCloseDisplay(dpy);
};

X11_wrapper::X11_wrapper()
{
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	XSetWindowAttributes swa;
	setupScreenRes(gl.xres, gl.yres);
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
	} 
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
		vi->depth, InputOutput, vi->visual,
		CWColormap | CWEventMask, &swa);
	GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	setTitle();
};

void X11_wrapper::setTitle() {
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "3350 - Walk Cycle");
};

void X11_wrapper::setupScreenRes(const int w, const int h)
{
	gl.xres = w;
	gl.yres = h;
};

void X11_wrapper::reshapeWindow(int width, int height)
{
	//window has been resized.
	setupScreenRes(width, height);
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	setTitle();
};

void X11_wrapper::checkResize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != gl.xres || xce.height != gl.yres) {
		//Window size did change.
		reshapeWindow(xce.width, xce.height);
	}
};

XEvent X11_wrapper::getXNextEvent()
{
	XEvent e;
	XNextEvent(dpy, &e);
	return e;
};

void X11_wrapper::swapBuffers()
{
	glXSwapBuffers(dpy, win);
}
X11_wrapper x11;
//-----------------------------------------------------------------------------
// SetUp Image Class
Image::Image(const char *fname)
{
	init(fname);
};

void Image::init(const char *fname)
{
	if (fname[0] == '\0') { return; }
		//printf("fname **%s**\n", fname);
		int ppmFlag = 0;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0)
			ppmFlag = 1;
		if (ppmFlag) {
			strcpy(ppmname, name);
		} else {
			name[slen-4] = '\0';
			//printf("name **%s**\n", name);
			sprintf(ppmname,"%s.ppm", name);
			//printf("ppmname **%s**\n", ppmname);
			char ts[100];
			//system("convert eball.jpg eball.ppm");
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
		}
		//sprintf(ts, "%s", name);
		//printf("read ppm **%s**\n", ppmname); fflush(stdout);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			//skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;			
			data = new unsigned char[n];			
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		if (!ppmFlag)
			unlink(ppmname);
};
Image img[4] = {
"./images/walk.gif",
"./images/exp.png",
"./images/exp44.png",
"./images/player/bottle_throw.png"
};
//-----------------------------------------------------------------------------
/*class Entity {
    public:
        float x, y;
        float width, height;
        bool isStatic;
        
        virtual void render () {

        }

        virtual void update () {

        }

        virtual bool checkCollison () {
            return false;
        }
};*/

/*class Player : public Entity {
    public:
        Player () 
        {
            CollisonManager::getInstance().add(this);
        }

        Player(float _xpos, float _ypos, float _width, float _height) 
        {
            x = _xpos;
            y = _ypos;
            width = _width;
            height = _height;
            isStatic = false;
            CollisonManager::getInstance().add(this);
        }

        void move (float nx, float ny) 
        {
            x += nx;
            y += ny;        
        }

        virtual void render() 
        {
            glColor3f (0.0f, 1.0f, 0.0f);

            glBegin(GL_QUADS);
			    glTexCoord2f(0, 0); 
                    glVertex2f(x, y);
    			glTexCoord2f(0, 1); 
                    glVertex2f(x, y + height);
			    glTexCoord2f(1, 1); 
                    glVertex2f(x + width, y + height);
    			glTexCoord2f(1, 0); 
                    glVertex2f(x + width, y);
		    glEnd();
        }

        virtual void update() 
        {

        }

        void SpawnPlayer (float nx, float ny) 
        {
            x = nx;
            y = ny;
        }

        virtual bool checkCollison () 
        {
            return false;
        }
} player (100, 100, 75, 100);*/

Player player ("Player", 100, 100, 75, 100, false);

/*class Enemy : public Entity {
    private:
        float bound1, bound2;
    public:
        float walkDist;        
        float walkSpeed;

        Enemy () 
        {
            CollisonManager::getInstance().add(this);
        }
    
        Enemy (float _xpos, float _ypos, float _width, float _height, float _walkDist, float _walkSpeed)
        {
            x = _xpos;
            y = _ypos;
            width = _width;
            height = _height;
            walkDist = _walkDist;
            walkSpeed = _walkSpeed;

            bound1 = x + walkDist;
            bound2 = x - walkDist;

            CollisonManager::getInstance().add(this);
        }

        virtual void render ()
        {
            glColor3f (1.0f, 0.0f, 0.0f);

            glBegin(GL_QUADS);
			    glTexCoord2f(0, 0); 
                    glVertex2f(x, y);
    			glTexCoord2f(0, 1); 
                    glVertex2f(x, y + height);
			    glTexCoord2f(1, 1); 
                    glVertex2f(x + width, y + height);
    			glTexCoord2f(1, 0); 
                    glVertex2f(x + width, y);
		    glEnd();
        }

        virtual void update ()
        {
            if (x >= bound1)
                walkSpeed *= -1;
            else if (x <= bound2)
                walkSpeed *= -1;

            move (x + walkSpeed, 0);
        }

        void move (float nx, float ny)
        {
            x = nx;
            y = ny;
        }
};*/

class Camera {
    
} mainCamera;

///////////////////////////////////////////////////////////////////////////////

Screens screen;

///////////////////////////////////////////////////////////////////////////////

int main(void)
{
	std::clock_t begin;
	double duration;

	// 30 Frames per sceond
	double temp = 1. / 30;

	initOpengl();
	init();
	int done = 0;
	while (!done) {
		begin = std::clock();
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);
			checkMouse(&e);
			done = checkKeys(&e);
		}
		physics();
		render();
		x11.swapBuffers();

		duration = (double)( std::clock() - begin ) / (double) CLOCKS_PER_SEC;
			
		while (duration < temp) {
			duration = (double)( std::clock() - begin ) / (double) CLOCKS_PER_SEC;
		};
	}
	cleanup_fonts();
	return 0;
}

unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

unsigned char *buildAlphaCustom(Image *img, unsigned char red, 
						        unsigned char green, unsigned char blue)
{
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = red;
	unsigned char t1 = green;
	unsigned char t2 = blue;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void initOpengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, gl.xres, gl.yres);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//load the images file into a ppm structure.
	//
	int w = img[0].width;
	int h = img[0].height;
	//
	//create opengl texture elements
	glGenTextures(1, &gl.walkTexture);
	//-------------------------------------------------------------------------
	//silhouette
	//this is similar to a sprite graphic
	//
	glBindTexture(GL_TEXTURE_2D, gl.walkTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *walkData = buildAlphaData(&img[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	free(walkData);
	//-------------------------------------------------------------------------
	//create opengl texture elements
	w = img[1].width;
	h = img[1].height;
	glGenTextures(1, &gl.exp.tex);
	//-------------------------------------------------------------------------
	//this is similar to a sprite graphic
	glBindTexture(GL_TEXTURE_2D, gl.exp.tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//must build a new set of data...
	unsigned char *xData = buildAlphaData(&img[1]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, xData);
	free(xData);
	//-------------------------------------------------------------------------
	w = img[2].width;
	h = img[2].height;
	//create opengl texture elements
	glGenTextures(1, &gl.exp44.tex);
	
	w = img[3].width;
	h = img[3].height;
	
	glGenTextures(1, &gl.bottleThrow.tex);
	//-------------------------------------------------------------------------
	//this is similar to a sprite graphic
	glBindTexture(GL_TEXTURE_2D, gl.exp44.tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//must build a new set of data...
	xData = buildAlphaData(&img[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, xData);
	free(xData);
}

void init() {

	//initialize buttons...
        gl.nbuttons=0;
        //size and position
        gl.button[gl.nbuttons].r.width = 140;
        gl.button[gl.nbuttons].r.height = 60;
        gl.button[gl.nbuttons].r.left = 20;
        gl.button[gl.nbuttons].r.bot = 320;
        gl.button[gl.nbuttons].r.right =
           gl.button[gl.nbuttons].r.left + gl.button[gl.nbuttons].r.width;
        gl.button[gl.nbuttons].r.top =
           gl.button[gl.nbuttons].r.bot + gl.button[gl.nbuttons].r.height;
        gl.button[gl.nbuttons].r.centerx =
           (gl.button[gl.nbuttons].r.left + gl.button[gl.nbuttons].r.right) / 2;
        gl.button[gl.nbuttons].r.centery =
           (gl.button[gl.nbuttons].r.bot + gl.button[gl.nbuttons].r.top) / 2;
        strcpy(gl.button[gl.nbuttons].text, "Start Game");
        gl.button[gl.nbuttons].down = 0;
        gl.button[gl.nbuttons].click = 0;
        gl.button[gl.nbuttons].color[0] = 0.4f;
        gl.button[gl.nbuttons].color[1] = 0.4f;
        gl.button[gl.nbuttons].color[2] = 0.7f;
        gl.button[gl.nbuttons].dcolor[0] = gl.button[gl.nbuttons].color[0] * 0.5f;
        gl.button[gl.nbuttons].dcolor[1] = gl.button[gl.nbuttons].color[1] * 0.5f;
        gl.button[gl.nbuttons].dcolor[2] = gl.button[gl.nbuttons].color[2] * 0.5f;
        gl.button[gl.nbuttons].text_color = 0x00ffffff;
        gl.nbuttons++;
        gl.button[gl.nbuttons].r.width = 140;
        gl.button[gl.nbuttons].r.height = 60;
        gl.button[gl.nbuttons].r.left = 20;
        gl.button[gl.nbuttons].r.bot = 160;
        gl.button[gl.nbuttons].r.right =
           gl.button[gl.nbuttons].r.left + gl.button[gl.nbuttons].r.width;
        gl.button[gl.nbuttons].r.top = gl.button[gl.nbuttons].r.bot +
           gl.button[gl.nbuttons].r.height;
        gl.button[gl.nbuttons].r.centerx = (gl.button[gl.nbuttons].r.left +
           gl.button[gl.nbuttons].r.right) / 2;
        gl.button[gl.nbuttons].r.centery = (gl.button[gl.nbuttons].r.bot +
           gl.button[gl.nbuttons].r.top) / 2;
        strcpy(gl.button[gl.nbuttons].text, "Credits");
        gl.button[gl.nbuttons].down = 0;
        gl.button[gl.nbuttons].click = 0;
        gl.button[gl.nbuttons].color[0] = 0.3f;
        gl.button[gl.nbuttons].color[1] = 0.3f;
        gl.button[gl.nbuttons].color[2] = 0.6f;
        gl.button[gl.nbuttons].dcolor[0] = gl.button[gl.nbuttons].color[0] * 0.5f;
        gl.button[gl.nbuttons].dcolor[1] = gl.button[gl.nbuttons].color[1] * 0.5f;
        gl.button[gl.nbuttons].dcolor[2] = gl.button[gl.nbuttons].color[2] * 0.5f;
        gl.button[gl.nbuttons].text_color = 0x00ffffff;
        gl.nbuttons++;

}

int checkMouse(XEvent *e)
{
	//printf("checkMouse()...\n"); fflush(stdout);
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	int i,x,y;
	int lbutton=0;
	int rbutton=0;
	//
	 if (e->type == ButtonRelease)
                return 0;
        if (e->type == ButtonPress) {
                if (e->xbutton.button==1) {
                        //Left button is down
                        lbutton=1;
                }
                if (e->xbutton.button==3) {
                        //Right button is down
                        rbutton=1;
                        if (rbutton){}
                }
        }
        x = e->xbutton.x;
        y = e->xbutton.y;
        y = gl.yres - y;
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
                //Mouse moved
                savex = e->xbutton.x;
                savey = e->xbutton.y;
        }
	
	for (i=0; i<gl.nbuttons; i++) {
	     gl.button[i].over=0;
                if (x >= gl.button[i].r.left &&
                        x <= gl.button[i].r.right &&
                        y >= gl.button[i].r.bot &&
                        y <= gl.button[i].r.top) {
                        gl.button[i].over=1;
                        if (gl.button[i].over) {
			   gl.button[i].down = 1;
			    gl.button[i].click = 1;
                              if (lbutton){
                                if (i==0) {
                                        //user clicked QUIT
					printf("Credits was clicked!\n");
                                        // screen.credits->display = !screen.credits->display;
                                }
                                if (i==1) {
                                        //user clicked button 0
                                       // credits.display = !credits.display;
                                }
                        }
                }
	    }
	}	
		/*for (i=0; i<gl.nbuttons; i++) {
                gl.button[i].over=0;
                if (x >= gl.button[i].r.left &&
                        x <= gl.button[i].r.right &&
                        y >= gl.button[i].r.bot &&
                        y <= gl.button[i].r.top) {
                        gl.button[i].over=1;
                        if (gl.button[i].over) {
                                if (lbutton) {
                                        switch (i) {
                                            case 0: 
							printf("Credits was clicked!\n");
           						credits.display = !credits.display;
                                                        break; 
                                             case 1:
						    	printf("Credits was clicked!\n"); //to tell if it's working
           						credits.display = !credits.display;
							
                                        }
                                }
                        }
                }
        }*/
        
		return 0;
}

void screenCapture()
{
	static int fnum = 0;
	static int vid = 0;
	if (!vid) {
		system("mkdir ./vid");
		vid = 1;
	}
	unsigned char *data = (unsigned char *)malloc(gl.xres * gl.yres * 3);
    glReadPixels(0, 0, gl.xres, gl.yres, GL_RGB, GL_UNSIGNED_BYTE, data);
	char ts[32];
	sprintf(ts, "./vid/pic%03i.ppm", fnum);
	FILE *fpo = fopen(ts,"w");	
	if (fpo) {
		fprintf(fpo, "P6\n%i %i\n255\n", gl.xres, gl.yres);
		unsigned char *p = data;
		//go backwards a row at a time...
		p = p + ((gl.yres-1) * gl.xres * 3);
		unsigned char *start = p;
		for (int i=0; i<gl.yres; i++) {
			for (int j=0; j<gl.xres*3; j++) {
				fprintf(fpo, "%c",*p);
				++p;
			}
			start = start - (gl.xres*3);
			p = start;
		}
		fclose(fpo);
		char s[256];
		sprintf(s, "convert ./vid/pic%03i.ppm ./vid/pic%03i.gif", fnum, fnum);
		system(s);
		unlink(ts);
	}
	++fnum;
}

int checkKeys(XEvent *e)
{
	static int shift = 0;

	if (e->type != KeyPress && e->type != KeyRelease) {
		return 0;
	}

	int key = XLookupKeysym(&e->xkey, 0);
	gl.keys[key] = 1;
	if (e->type == KeyRelease) {
		gl.keys[key] = 0;
		if (key == XK_Shift_L || key == XK_Shift_R){
			shift = 0;
		}
		return 0;
	}
	gl.keys[key] = 1;
	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift = 1;
		return 0;
	}
	(void)shift;
	
	if (key == XK_d) {
	    struct timespec bt;
	    clock_gettime(CLOCK_REALTIME, &bt);
	    double ts = timers.timeDiff(&g.bulletTimer, &bt);
	    if (ts > 0.1) {
		timers.timeCopy(&g.bulletTimer, &bt);
		//shoot
		if (g.nbullets < MAX_BULLETS) {
		    Bullet *b = &g.barr[g.nbullets];
		    timers.timeCopy(&b->time, &bt);
		    b->pos[0] = player.x;
		    b->pos[1] = player.y;
		    b->vel[0] = 30;
		    b->vel[1] = 0;
		    b->color[0] = 1.0f;
		    b->color[1] = 1.0f;
		    b->color[2] = 1.0f;
		    ++g.nbullets;
		}

	    }
	}

	if (screen.start->display)	      return screen.start->checkKey(key);
	else if (screen.credits->display) return screen.credits->checkKey(key);
	else if (screen.level_1->display) return screen.level_1->checkKey(key);
	else return defaultKeys(key);
}

Flt VecNormalize(Vec vec)
{
	Flt len, tlen;
	Flt xlen = vec[0];
	Flt ylen = vec[1];
	Flt zlen = vec[2];
	len = xlen*xlen + ylen*ylen + zlen*zlen;
	if (len == 0.0) {
		MakeVector(vec, 0.0, 0.0, 1.0);
		return 1.0;
	}
	len = sqrt(len);
	tlen = 1.0 / len;
	vec[0] = xlen * tlen;
	vec[1] = ylen * tlen;
	vec[2] = zlen * tlen;
	return(len);
}

void physics(void)
{
	if (gl.walk || gl.keys[XK_Right] || gl.keys[XK_Left]) {
		//man is walking...
		//when time is up, advance the frame.
        player.move (0.5f, 0.0f);
		timers.recordTime(&timers.timeCurrent);
		double timeSpan = timers.timeDiff(&timers.walkTime, &timers.timeCurrent);
		if (timeSpan > gl.delay) {
			//advance
			++gl.walkFrame;
			if (gl.walkFrame >= 16)
				gl.walkFrame -= 16;
			timers.recordTime(&timers.walkTime);
		}
		for (int i=0; i<20; i++) {
			if (gl.keys[XK_Left]) {
				gl.box[i][0] += 1.0 * (0.05 / gl.delay);
				if (gl.box[i][0] > gl.xres + 10.0)
					gl.box[i][0] -= gl.xres + 10.0;
				gl.camera[0] -= 2.0/lev.tilesize[0] * (0.05 / gl.delay);
				if (gl.camera[0] < 0.0)
					gl.camera[0] = 0.0;
			} else {
				gl.box[i][0] -= 1.0 * (0.05 / gl.delay);
				if (gl.box[i][0] < -10.0)
					gl.box[i][0] += gl.xres + 10.0;
				gl.camera[0] += 2.0/lev.tilesize[0] * (0.05 / gl.delay);
				if (gl.camera[0] < 0.0)
					gl.camera[0] = 0.0;
			}
		}
		if (gl.exp.onoff) {
			gl.exp.pos[0] -= 2.0 * (0.05 / gl.delay);
		}
		if (gl.exp44.onoff) {
			gl.exp44.pos[0] -= 2.0 * (0.05 / gl.delay);
		}
	}
	if (gl.exp.onoff) {
		//explosion is happening
		timers.recordTime(&timers.timeCurrent);
		double timeSpan = timers.timeDiff(&gl.exp.time, &timers.timeCurrent);
		if (timeSpan > gl.exp.delay) {
			//advance explosion frame
			++gl.exp.frame;
			if (gl.exp.frame >= 23) {
				//explosion is done.
				gl.exp.onoff = 0;
				gl.exp.frame = 0;
			} else {
				timers.recordTime(&gl.exp.time);
			}
		}
	}
	if (gl.exp44.onoff) {
		//explosion is happening
		timers.recordTime(&timers.timeCurrent);
		double timeSpan = timers.timeDiff(&gl.exp44.time, &timers.timeCurrent);
		if (timeSpan > gl.exp44.delay) {
			//advance explosion frame
			++gl.exp44.frame;
			if (gl.exp44.frame >= 16) {
				//explosion is done.
				gl.exp44.onoff = 0;
				gl.exp44.frame = 0;
			} else {
				timers.recordTime(&gl.exp44.time);
			}
		}
	}
	//====================================
	//Adjust position of ball.
	//Height of highest tile when ball is?
	//====================================
	Flt dd = lev.ftsz[0];
	int col = (int)((gl.camera[0]+gl.ball_pos[0]) / dd);
	col = col % lev.ncols;
	int hgt = 0;
	for (int i=0; i<lev.nrows; i++) {
		if (lev.arr[i][col] != ' ') {
			hgt = (lev.nrows-i) * lev.tilesize[1];
			break;
		}
	}
	if (gl.ball_pos[1] < (Flt)hgt) {
		gl.ball_pos[1] = (Flt)hgt;
		MakeVector(gl.ball_vel, 0, 0, 0);
	} else {
		gl.ball_vel[1] -= 0.9;
	}
	gl.ball_pos[1] += gl.ball_vel[1];

	//Update bottle positions
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i=0;
	while (i <g.nbullets) {
	    Bullet *b = &g.barr[i];
	    double ts = timers.timeDiff (&b->time, &bt);
	    if (ts >2.0) {
		memcpy(&g.barr[i], &g.barr[g.nbullets-1],
			sizeof(Bullet));
		g.nbullets--;
		continue;
	    }
	    //move bottle
	    b->pos[0] += b->vel[0];
	    b->pos[1] += b->vel[1];
            if (b->pos[0] < 0.0) {
                b->pos[0] += (float)gl.xres;
            }
            else if (b->pos[0] > (float)gl.xres) {
		memcpy(&g.barr[i], &g.barr[g.nbullets-1],
                        sizeof(Bullet));
                g.nbullets--;
                continue; 
            }
            else if (b->pos[1] < 0.0) {
                memcpy(&g.barr[i], &g.barr[g.nbullets-1],
                        sizeof(Bullet));
                g.nbullets--;
                continue;
            }
            else if (b->pos[1] > (float)gl.yres) {
                b->pos[1] -= (float)gl.yres;
            }
	    i++;
	}
}

void render(void)
{
	Rect r;
	//Clear the screen
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);	
	unsigned int c = 0x00ffff44;
	
	float cx = gl.xres/2.0;
	float cy = gl.yres/2.0;
	//
	//show ground
	glBegin(GL_QUADS);
		glColor3f(0.2, 0.2, 0.2);
		glVertex2i(0,       220);
		glVertex2i(gl.xres, 220);
		glColor3f(0.4, 0.4, 0.4);
		glVertex2i(gl.xres,   0);
		glVertex2i(0,         0);
	glEnd();
	//
	//show boxes as background
	for (int i=0; i<20; i++) {
		glPushMatrix();
		glTranslated(gl.box[i][0],gl.box[i][1],gl.box[i][2]);
		glColor3f(0.2, 0.2, 0.2);
		glBegin(GL_QUADS);
			glVertex2i( 0,  0);
			glVertex2i( 0, 30);
			glVertex2i(20, 30);
			glVertex2i(20,  0);
		glEnd();
		glPopMatrix();
	}
	///DRAWING BUTTONS///
	 for (int i=0; i<gl.nbuttons; i++) {
                if (gl.button[i].over) {
                        int w=2;
                        glColor3f(1.0f, 1.0f, 0.0f);
                        //draw a highlight around button
                        glLineWidth(3);
                        glBegin(GL_LINE_LOOP);
                                glVertex2i(gl.button[i].r.left-w,  gl.button[i].r.bot-w);
                                glVertex2i(gl.button[i].r.left-w,  gl.button[i].r.top+w);
                                glVertex2i(gl.button[i].r.right+w, gl.button[i].r.top+w);
                                glVertex2i(gl.button[i].r.right+w, gl.button[i].r.bot-w);
                                glVertex2i(gl.button[i].r.left-w,  gl.button[i].r.bot-w);
                        glEnd();
                        glLineWidth(1);
                }
                if (gl.button[i].down) {
                        glColor3fv(gl.button[i].dcolor);
                } else {
                        glColor3fv(gl.button[i].color);
                }
                glBegin(GL_QUADS);
                        glVertex2i(gl.button[i].r.left,  gl.button[i].r.bot);
                        glVertex2i(gl.button[i].r.left,  gl.button[i].r.top);
                        glVertex2i(gl.button[i].r.right, gl.button[i].r.top);
                        glVertex2i(gl.button[i].r.right, gl.button[i].r.bot);
                glEnd();
                r.left = gl.button[i].r.centerx;
                r.bot  = gl.button[i].r.centery-8;
                r.center = 1;
                if (gl.button[i].down) {
                        ggprint16(&r, 0, gl.button[i].text_color, "Pressed!");
                } else {
                        ggprint16(&r, 0, gl.button[i].text_color, gl.button[i].text);
                }
        }

	//
	//========================
	//Render the tile system
	//========================
	int tx = lev.tilesize[0];
	int ty = lev.tilesize[1];
	Flt dd = lev.ftsz[0];
	Flt offy = lev.tile_base;
	int ncols_to_render = gl.xres / lev.tilesize[0] + 2;
	int col = (int)(gl.camera[0] / dd);
	col = col % lev.ncols;
	//Partial tile offset must be determined here.
	//The leftmost tile might be partially off-screen.
	//cdd: camera position in terms of tiles.
	Flt cdd = gl.camera[0] / dd;
	//flo: just the integer portion
	Flt flo = floor(cdd);
	//dec: just the decimal portion
	Flt dec = (cdd - flo);
	//offx: the offset to the left of the screen to start drawing tiles
	Flt offx = -dec * dd;
	//Log("gl.camera[0]: %lf   offx: %lf\n",gl.camera[0],offx);
	for (int j=0; j<ncols_to_render; j++) {
		int row = lev.nrows-1;
		for (int i=0; i<lev.nrows; i++) {
			if (lev.arr[row][col] == 'w') {
				glColor3f(0.8, 0.8, 0.6);
				glPushMatrix();
				//put tile in its place
				glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
				glBegin(GL_QUADS);
					glVertex2i( 0,  0);
					glVertex2i( 0, ty);
					glVertex2i(tx, ty);
					glVertex2i(tx,  0);
				glEnd();
				glPopMatrix();
			}
			if (lev.arr[row][col] == 'b') {
				glColor3f(0.9, 0.2, 0.2);
				glPushMatrix();
				glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
				glBegin(GL_QUADS);
					glVertex2i( 0,  0);
					glVertex2i( 0, ty);
					glVertex2i(tx, ty);
					glVertex2i(tx,  0);
				glEnd();
				glPopMatrix();
			}
			--row;
		}
		col = (col+1) % lev.ncols;
	}
	glColor3f(1.0, 1.0, 0.1);
	glPushMatrix();
	//put ball in its place
	glTranslated(gl.ball_pos[0], lev.tile_base+gl.ball_pos[1], 0);
	glBegin(GL_QUADS);
		glVertex2i(-10, 0);
		glVertex2i(-10, 20);
		glVertex2i( 10, 20);
		glVertex2i( 10, 0);
	glEnd();
	glPopMatrix();
	//--------------------------------------
	//
	//#define SHOW_FAKE_SHADOW
	#ifdef SHOW_FAKE_SHADOW
	glColor3f(0.25, 0.25, 0.25);
	glBegin(GL_QUADS);
		glVertex2i(cx-60, 150);
		glVertex2i(cx+50, 150);
		glVertex2i(cx+50, 130);
		glVertex2i(cx-60, 130);
	glEnd();
	#endif
	//
	//
	float h = 200.0;
	float w = h * 0.5;
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, gl.walkTexture);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	int ix = gl.walkFrame % 8;
	int iy = 0;
	if (gl.walkFrame >= 8)
		iy = 1;
	float fx = (float)ix / 8.0;
	float fy = (float)iy / 2.0;
	glBegin(GL_QUADS);
		if (gl.keys[XK_Left]) {
			glTexCoord2f(fx+.125, fy+.5); glVertex2i(cx-w, cy-h);
			glTexCoord2f(fx+.125, fy);    glVertex2i(cx-w, cy+h);
			glTexCoord2f(fx,      fy);    glVertex2i(cx+w, cy+h);
			glTexCoord2f(fx,      fy+.5); glVertex2i(cx+w, cy-h);
		} else {
			glTexCoord2f(fx,      fy+.5); glVertex2i(cx-w, cy-h);
			glTexCoord2f(fx,      fy);    glVertex2i(cx-w, cy+h);
			glTexCoord2f(fx+.125, fy);    glVertex2i(cx+w, cy+h);
			glTexCoord2f(fx+.125, fy+.5); glVertex2i(cx+w, cy-h);
		}
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	//
	//
	if (gl.exp.onoff) {
		h = 80.0;
		w = 80.0;
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, gl.exp.tex);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glTranslated(gl.exp.pos[0], gl.exp.pos[1], gl.exp.pos[2]);
		int ix = gl.exp.frame % 5;
		int iy = gl.exp.frame / 5;
		float tx = (float)ix / 5.0;
		float ty = (float)iy / 5.0;
		glBegin(GL_QUADS);
			glTexCoord2f(tx,     ty+0.2); glVertex2i(cx-w, cy-h);
			glTexCoord2f(tx,     ty);     glVertex2i(cx-w, cy+h);
			glTexCoord2f(tx+0.2, ty);     glVertex2i(cx+w, cy+h);
			glTexCoord2f(tx+0.2, ty+0.2); glVertex2i(cx+w, cy-h);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
	}
	//
	//
	if (gl.exp44.onoff) {
		h = 80.0;
		w = 80.0;
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, gl.exp44.tex);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glTranslated(gl.exp44.pos[0], gl.exp44.pos[1], gl.exp44.pos[2]);
		int ix = gl.exp44.frame % 4;
		int iy = gl.exp44.frame / 4;
		float tx = (float)ix / 4.0;
		float ty = (float)iy / 4.0;
		glBegin(GL_QUADS);
			glTexCoord2f(tx,      ty+0.25); glVertex2i(cx-w, cy-h);
			glTexCoord2f(tx,      ty);      glVertex2i(cx-w, cy+h);
			glTexCoord2f(tx+0.25, ty);      glVertex2i(cx+w, cy+h);
			glTexCoord2f(tx+0.25, ty+0.25); glVertex2i(cx+w, cy-h);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
	}
	// unsigned int c = 0x00ffff44;
	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, c, "W   Walk cycle");
	ggprint8b(&r, 16, c, "E   Explosion");
	ggprint8b(&r, 16, c, "+   faster");
	ggprint8b(&r, 16, c, "-   slower");
	ggprint8b(&r, 16, c, "right arrow -> walk right");
	ggprint8b(&r, 16, c, "left arrow  <- walk left");
	ggprint8b(&r, 16, c, "Press 'm' for main menu");
	ggprint8b(&r, 16, c, "frame: %i", gl.walkFrame);

	player.render();

	screen.start->Display(&gl);
	screen.credits->Display(&gl);
	screen.level_1->Display(&gl);

    Bullet *b = &g.barr[0];
    for (int i=0; i<g.nbullets; i++) {
        Log("draw bullet...\n");
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POINTS);
            glVertex2f(b->pos[0],      b->pos[1]);
            glVertex2f(b->pos[0]-1.0f, b->pos[1]);
            glVertex2f(b->pos[0]+1.0f, b->pos[1]);
            glVertex2f(b->pos[0],      b->pos[1]-1.0f);
            glVertex2f(b->pos[0],      b->pos[1]+1.0f);
            glColor3f(0.8, 0.8, 0.8);
            glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
            glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
            glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
            glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
        glEnd();
        ++b;
    }	

}

int defaultKeys(int key)
{
	switch (key) {
		case XK_s:
			screenCapture();
			break;

		case XK_w:
			timers.recordTime(&timers.walkTime);
			gl.walk ^= 1;
			break;
		
		case XK_e:
			gl.exp.pos[0] = 200.0;
			gl.exp.pos[1] = -60.0;
			gl.exp.pos[2] =   0.0;
			timers.recordTime(&gl.exp.time);
			gl.exp.onoff ^= 1;
			break;

		case XK_f:
			gl.exp44.pos[0] = 200.0;
			gl.exp44.pos[1] = -60.0;
			gl.exp44.pos[2] =   0.0;
			timers.recordTime(&gl.exp44.time);
			gl.exp44.onoff ^= 1;
			break;

		case XK_Left:
			
			break;

		case XK_Right:
			
			break;

		case XK_Up:
			break;

		case XK_Down:
			break;

		case XK_equal:
			gl.delay -= 0.005;
			if (gl.delay < 0.005)
				gl.delay = 0.005;
			break;

		case XK_minus:
			gl.delay += 0.005;
			break;

		case XK_Escape:
			return 1;
			break;

		case XK_m:
			screen.start->display = true;
			break;
	}
	return 0;
}
