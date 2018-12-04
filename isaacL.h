
#ifndef ISAACL_H_
#define ISAACL_H_

#include  <GL/glx.h>
#include   <stdio.h>
#include  <stdlib.h>
#include    <math.h>
#include   "fonts.h"
#include "default.h"

#define  NUM_CLOUDS 15
#define SIZE_CLOUDS 175
#define NUM_BOTTLES 20

extern Global gl;

class Cloud {
	int    xpos, ypos;
	int    dx, dy;
	int    size;
	bool   init;
	GLuint texid;

public:
	Cloud();
	void draw();
	void setAttr(GLuint, int, int);
	void refreshX(GLuint, int);
	void refreshY(GLuint, int);
	void move();
	int  getX() {return xpos;}
	int  getY() {return ypos;}
	bool needInit() {return init;}
};

class Text {
	int    xpos, ypos;
	int    width, height;
	bool   active;
	GLuint texid;

public:
	Text ();
	void setTex(GLuint);
	void draw();
	void setActive(bool);
	void setLoc(int, int);
};

class Start {
public:
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
	int cloud_num;

	Image text_img[3] = {
		"./images/startmenu/start.png",
		"./images/startmenu/controls.png",
		"./images/startmenu/credits.png"
	};
	int text_num;

	// Object
	Cloud *clouds;
	Text  *text;

	// Textures
	GLuint *cloud_tex;
	GLuint *text_tex;
	GLuint  title_tex;

	// Variables
	bool display;
	bool restart;

	// Menu Options
	int *opt;
	int  opt_size;

	// Methods
	Start();
	void Display();
	void initClouds();
	void showClouds();
	void initText();
	void showText();
	bool showTitle(bool);
	int  checkKey(int);
};

class Credits {
public:
	//Images
	Image team_img[2] = {
		// Alex Pic

		// Isaac Pic
		"./images/credits/isaacL.jpg",
		// Austin Pic

		// Justin Pic
		"./images/credits/justinS.jpg"
	};
	int team_num;

	// Textures
	GLuint *team_tex;

	// Variables
	bool display;

	// Methods
	Credits();
	void Display();
	int  checkKey(int);
};

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
	
	int cloud_num;
	Cloud *clouds;
	//Controls Images
	Image controls_img[4] = {"./images/controls/arrows.png",
                             "./images/controls/wasd.png",
                             "./images/controls/movement.png",
                             "./images/controls/shoot.png" };

    //variables
    bool display = true;

	//textures
	GLuint *controls_tex;
	GLuint *cloud_tex;
    //Methods
    Controls();
    void Display();
	void initClouds();
    void showClouds();	
    int checkKey(int);
};
//////
class Bottle {
public:
	// Images
	Image bottle_img = "./images/player/bottle.png";

	// Texture
	GLuint bottle_tex;

	// Positions
	int xpos, ypos;
	int hitbox[4][2];

	// Variables
	int  size, step;
	int   direction;
	float ds, frame;
	bool     active;

	bool debug = false;

	// Methods
	Bottle();
	void Display();
	void drawHitBox();
	void Attack(int, int, int);
	void Shatter();
};

class Player {
public:
	// Image
	Image player_img[4] = {
		"./images/player/stand_right.png",
		"./images/player/stand_left.png",
		"./images/player/walk_right.png",
		"./images/player/walk_left.png"
	};
	int player_num;

	// Player Texture
	GLuint *player_tex;

	// Attack Weapon
	Bottle *bottles;
	int  timer;
	int   time;
	bool shoot;

	// Positions
	int xpos, ypos; 	// refers to bottom center of player
	int size, step;		// player is (2*size) X (2*size)

	// Display
	bool display;
	bool     fwd;
	bool     bwd;
	bool     mov;
	int    frame;
	int hitbox[4][2];

	bool debug = false;

	// Abilities
	int  fireRate;		// gl.fps/fireRate = shots/second
	int fireSpeed;

	// Methods
	Player();
	void Display();
	void drawHitBox();
	void Attack(int);
	void showAttack();
};

class Enemy {
public:
	// Image
	Image enemy_img[8] = {
		"./images/enemy/stand_right.png",		//0
		"./images/enemy/stand_left.png",		//1
		"./images/enemy/walk_right.png",		//2
		"./images/enemy/walk_left.png",			//3
		"./images/enemy/stand_up_right.png",	//4
		"./images/enemy/stand_up_left.png",		//5
		"./images/enemy/die_right.png",			//6
		"./images/enemy/die_left.png",			//7
		// "./images/enemy/attack_right.png",		//8
		// "./images/enemy/attack_left.png"		//9
	};
	int enemy_num;

	// Player Texture
	GLuint *enemy_tex;

	// Positions
	int xpos, ypos;
	int size, step;	//Enemy is 2*size tall and wide
	int  rand_diff;
	int  health;
	int  frame;

	// Display
	bool  near;
	bool stood;
	bool alive;
	int  slice;
	int hitbox[4][2];

	bool debug = false;

	bool fwd;
	bool bwd;
	bool mov;

	// Methods
	Enemy();
	void Display();
	void drawHitBox();
	void sitDraw();
	void standDraw();
	void moveDraw();
	void standupDraw();
	void sitdownDraw();
	void setLoc(int, int, int);
	void Move(int, int, int);
	void Attack();
	void Die();
};

class Level_1 {
public:
	// Images
	Image background_img = "./images/level1/levback.png";
	Image foreground_img = "./images/level1/levfore.png";
    Image health_img = "./images/player/milk_health.png";
	// Textures
	GLuint background_tex;
	GLuint foreground_tex;
    GLuint health_tex;
	// Variables
	bool    display;
	int enemies_num;
	int       frame;

	// Player Object
	Player *player;
	Enemy *enemies;

	// Methods
	Level_1(Player*);
	void Display();
	void checkMove();
	void refreshBottles();
	void checkHit();
	int  checkKey(int);
};

class Level_2 {
public:
	// Images
	Image background_img = "./images/level2/levback.png";
	Image foreground_img = "./images/level2/levfore.png";

	// Textures
	GLuint background_tex;
	GLuint foreground_tex;

	// Variables
	bool display;
	int enemies_num;
	int frame;

	// Object
	Player *player;
	Enemy *enemies;

	// Methods
	Level_2(Player*);
	void Display();
	void checkMove();
	void refreshBottles();
	void checkHit();
	int checkKey(int);
};

// Add Render Screens Here
struct Screens {
	Start *start;
	Level_1 *level_1;
	Level_2 *level_2;
	Credits *credits;
	Controls *controls;
	Player *player;
	bool *displays[4];
	Screens() 
	{
		start   = new Start();
		player  = new Player();
		level_1 = new Level_1(player);
		level_2 = new Level_2(player);
		controls = new Controls();
		credits = new Credits();

		displays[0] = &level_1->display;
		displays[3] = &start->display;
		displays[2] = &credits->display; 
		displays[1] = &controls->display;
	}
};

void menuSelect(Screens, int*, int);
void gotoStart(Screens screen);
#endif
