

#ifndef ISAACL_H_
#define ISAACL_H_

#include "default.h"
#include "Player.h"

#define NUM_CLOUDS 15
#define SIZE_CLOUDS 175

using namespace std;

class Cloud {
	int xpos, ypos;
	int dx = rand() % 3 + 2;
	int dy = 1;
	int size = SIZE_CLOUDS;
	bool init = true;
	GLuint texid;

public:
	Cloud() {}
	void draw();
	void setAttr(GLuint, int, int);
	void refreshX(GLuint, int);
	void refreshY(GLuint, int);
	void move();
	int getX() {return xpos;}
	int getY() {return ypos;}
	bool needInit() {return init;}
};

class Text {
	int xpos, ypos;
	int width, height;
	GLuint texid;
	bool active = false;

public:
	Text ();
	void setAttr(GLuint, int, int);
	void draw();
	void setActive(bool);
	void setLoc(int , int);
};

class Start {
public:
	const char *name = "mainmenu";
	// Images
	Image title_img = "./images/startmenu/title.png";
	Image cloud_img[8] = {
    "./images/startmenu/cloud1.jpg",
    "./images/startmenu/cloud2.jpg",
    "./images/startmenu/cloud3.jpg",
    "./images/startmenu/cloud4.jpg",
    "./images/startmenu/cloud5.jpg",
    "./images/startmenu/cloud6.jpg",
    "./images/startmenu/cloud7.jpg",
    "./images/startmenu/cloud8.jpg"
	};
	int cloud_num = sizeof cloud_img / sizeof *(cloud_img);
	Image text_img[3] = {
        "./images/startmenu/levels.png",
        "./images/startmenu/credits.png",
        "./images/startmenu/controls.png"
	};
	int text_num = sizeof text_img / sizeof *(text_img);

	// Textures
	GLuint *cloud_tex;
	GLuint *text_tex;
	GLuint title_tex;

	// Variables
	bool display = true;
	int opt[3] = { 1, 0, 0};
	int opt_size = sizeof opt / sizeof *opt;

	// Methods
	Start();
	void Display(Global*);
	int checkKey(int);
};

class Credits {
public:
	const char *name = "credits";
	//Images
	Image team_img[2] = {
		"./images/credits/isaacL.jpg",
		"./images/credits/justinS.jpg"
	};
	int team_num = sizeof team_img / sizeof *(team_img);

	// Textures
	GLuint *team_tex;
	GLuint *cloud_tex;
	// Variables
	bool display = false;

	// Methods
	Credits();
	void Display(Global*);
	int checkKey(int);
};
////alex code///
class Controls{
public:
        const char *name = "controls";
	
	 Image cloud_img[8] = {
    "./images/startmenu/cloud1.jpg",
    "./images/startmenu/cloud2.jpg",
    "./images/startmenu/cloud3.jpg",
    "./images/startmenu/cloud4.jpg",
    "./images/startmenu/cloud5.jpg",
    "./images/startmenu/cloud6.jpg",
    "./images/startmenu/cloud7.jpg",
    "./images/startmenu/cloud8.jpg"
        };

	//Controls Images
	Image controls_img = "./images/controls/arrows.png";

	int cloud_num = sizeof cloud_img / sizeof *(cloud_img);
        //variables
        bool display = false;

	//textures
	GLuint controls_tex;
	GLuint *cloud_tex;
        //Methods
        Controls();
        void Display(Global*);
        int checkKey(int);
};
//////
class Level_1 {
public:
	const char *name = "level_1";


	// Images
	Image background_img = "./images/level1/levback.png";
	Image foreground_img = "./images/level1/levfore.png";
	Image health_img = "./images/player/milk_health.png";

	// Textures
	GLuint background_tex;
	GLuint foreground_tex;
  	GLuint health_tex;

	// Variables
	bool display = false;
	int frame;
    //Player player("Player", 5, 100, 100, 75, 100, false);
    //Player player("Player", 5, 100, 100, 75, 100, false);

	// Methods
	Level_1();
	void Display(Global*);
	int checkKey(int);
};

// Add Render Screens Here
typedef struct Screens {
	Start *start;
	Level_1 *level_1;
	Credits *credits;
    Controls *controls;
	bool *displays[4];
	Screens() 
	{
		start   = new Start;
		level_1 = new Level_1;
		credits = new Credits;
        controls = new Controls;

		displays[0] = &start->display;
		displays[1] = &level_1->display;
		displays[2] = &credits->display;
        displays[3] = &controls->display;
	}
} Screens;

void testFunct(Screens, int*, int);

#endif
