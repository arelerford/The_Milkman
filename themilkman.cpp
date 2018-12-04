  //3350
//program: walk2.cpp
//author:  Gordon Griesel
//date:    spring 2018
//

#include "fonts.h"
#include "default.h"
#include "isaacL.h"

// Setup timers
Timers::Timers()
{
	this->physicsRate = 1.0 / 30.0;
	this->oobillion = 1.0 / 1e9;
}

double Timers::timeDiff(struct timespec *start, struct timespec *end)
{
	return (double)(end->tv_sec - start->tv_sec ) +
		   (double)(end->tv_nsec - start->tv_nsec) * oobillion;
}

void Timers::timeCopy(struct timespec *dest, struct timespec *source)
{
	memcpy(dest, source, sizeof(struct timespec));
}

void Timers::recordTime(struct timespec *t)
{
	clock_gettime(CLOCK_REALTIME, t);
}
Timers timers;

// Global
Global::Global()
{
	logOpen();
	// xres = 1350;
	// yres = 750;
	xres = 900;
	yres = 550;
	 fps = 1.0 / 30;	//second per frame
	memset(keys, 0, 65536);	
}
Global gl;

// X Windows variables
X11_wrapper::~X11_wrapper()
{
	XDestroyWindow(dpy, this->win);
	XCloseDisplay(dpy);
}

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

	//-------------------------------------------------------------------------
	// Disable Window Resize
	XSizeHints *hints;
	hints = XAllocSizeHints();

	hints->min_width  = gl.xres;
	hints->max_width  = gl.xres;
	hints->min_height = gl.yres;
	hints->max_height = gl.yres;

	//hints->min_aspect.x = 900;
	//hints->min_aspect.y = 501;
	//hints->max_aspect.x = 1801;
	//hints->max_aspect.y = 1000;

	hints->flags |= PMinSize;
	hints->flags |= PMaxSize;
	//hints->flags |= PAspect;

	XSetWMNormalHints(dpy, win, hints);
	//-------------------------------------------------------------------------

	GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	setTitle();
}

void X11_wrapper::setTitle() {
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "The Milkman");
}

void X11_wrapper::setupScreenRes(const int w, const int h)
{
	gl.xres = w;
	gl.yres = h;
}

void X11_wrapper::reshapeWindow(int width, int height)
{
	//window has been resized.
	setupScreenRes(width, height);
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	setTitle();
}

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
}

XEvent X11_wrapper::getXNextEvent()
{
	XEvent e;
	XNextEvent(dpy, &e);
	return e;
}

void X11_wrapper::swapBuffers()
{
	glXSwapBuffers(dpy, win);
}
X11_wrapper x11;

// Setup Image Class
Image::Image(const char *fname)
{
	init(fname);
}

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
}

// Screen object holds all the different view objects. i.e. startmenu, level_1, level_2, credits
Screens screen;
//-----------------------------------------------------------------------------
// MAIN
int main(void)
{
	std::clock_t begin;
	double duration;

	initOpengl();
	int done = 0;

	while (!done) {
		begin = std::clock();
		while (x11.getXPending() ) {
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);			
			done = checkKeys(&e);
		}
		physics();
		render();
		x11.swapBuffers();

		duration = (double)( std::clock() - begin ) / (double) CLOCKS_PER_SEC;			
		while (duration < gl.fps) {
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
	// add 4th component to RGB stream...
	// uses arguments red, green, blue to determine 
	// transparent color
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
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
		ptr  += 4;
		data += 3;
	}
	return newdata;
}

void initOpengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, gl.xres, gl.yres);

	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();

	//This sets 2D mode (no perspective)
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);

	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
}

int checkKeys(XEvent *e)
{
	static int shift = 0;

	if (e->type != KeyPress && e->type != KeyRelease) {
		return 0;
	}
	
	int key = XLookupKeysym(&e->xkey, 0);
	if (key >= 65536) return 0;

	if (e->type == KeyRelease) {
		gl.keys[key] = 0;
	} else if (e->type == KeyPress) {
		gl.keys[key] = 1;
	}
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

	if (screen.start->display) {
		return screen.start->checkKey(key);
	} else if (screen.controls->display) {
		return screen.controls->checkKey(key);
    } else if (screen.credits->display) {
        return screen.credits->checkKey(key);
    } else if (screen.level_1->display) {
		return screen.level_1->checkKey(key);
	} else {
		return 0;
	}
}

void physics(void)
{
}

void render(void)
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	screen.start->Display();
    screen.controls->Display();
	screen.credits->Display();
	screen.level_1->Display();
	screen.level_2->Display();
}
