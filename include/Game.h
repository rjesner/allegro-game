#ifndef _GAME_H_
#define _GAME_H_

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include <cstdio>
#include <memory>
#include <array>

class Game {
	static constexpr int SCREEN_WIDTH = 640;
	static constexpr int SCREEN_HEIGHT = 480;
	static constexpr int TILE_SIZE = 16;
	static constexpr float FPS = 60.0;
	static constexpr float FRAME_FPS = 15.0;
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

	std::array<int, MAX_CONTROLS> target{};
	int count = 0;
	bool controlFlag = false;

	std::shared_ptr<ALLEGRO_DISPLAY> display;
	std::shared_ptr<ALLEGRO_BITMAP> player;
	std::shared_ptr<ALLEGRO_BITMAP> background;
	ALLEGRO_KEYBOARD_STATE keyState;
	ALLEGRO_TRANSFORM camera;
	std::shared_ptr<ALLEGRO_TIMER> timer;
	std::shared_ptr<ALLEGRO_TIMER> frameTimer;
	std::shared_ptr<ALLEGRO_EVENT_QUEUE> eventQueue;
public:
	bool create();
	void runMainLoop();
	void drawContent();
	void drawGrid();
};

#endif
