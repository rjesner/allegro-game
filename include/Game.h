#ifndef _GAME_H_
#define _GAME_H_

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include <cstdio>
#include <memory>
 
class Game {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const int TILE_SIZE = 16;
	const float FPS = 60.0;
	const float FRAME_FPS = 15.0;
	static constexpr int MAX_CONTROLS = 4;
	enum Direction { DOWN, LEFT, RIGHT, UP };

	bool done = false;
	bool draw = true;
	bool active = false;
	float x = 320; 
	float y = 320;
	float moveSpeed = TILE_SIZE;
	int dir = DOWN;
	int sourceX = TILE_SIZE;
	int sourceY = 0;

	int target[MAX_CONTROLS] = {0};
	int count = 0;
	bool controlFlag = false;

	ALLEGRO_DISPLAY *display;
	ALLEGRO_BITMAP *player;
	ALLEGRO_BITMAP *background;
	ALLEGRO_KEYBOARD_STATE keyState;
	ALLEGRO_TRANSFORM camera;
	ALLEGRO_TIMER *timer;
	ALLEGRO_TIMER *frameTimer;
	ALLEGRO_EVENT_QUEUE *eventQueue;
public:
	~Game();

	bool create();
	void runMainLoop();
	void drawContent();
	void drawGrid();
};

#endif
