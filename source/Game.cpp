#include "Game.h"

int main(void) {
	std::shared_ptr<Game> game = std::make_shared<Game>();
	if(!game->create()) {
		return 1;
	}
	game->runMainLoop();
	return 0;
}

bool Game::create() {
	al_init();
	al_init_primitives_addon();

	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

	al_set_window_position(display, 200, 200);

	al_install_keyboard();
	al_init_image_addon();

	player = al_load_bitmap("../resource/player.png");
	background = al_load_bitmap("../resource/forest.png");
 
	timer = al_create_timer(1.0 / FPS);
	frameTimer = al_create_timer(1.0 / FRAME_FPS);

	eventQueue = al_create_event_queue();
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_timer_event_source(frameTimer));
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_keyboard_event_source());

	al_start_timer(timer);
	al_start_timer(frameTimer);
	return true;
}

Game::~Game() {
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_timer(frameTimer);
	al_destroy_bitmap(player);
	al_destroy_bitmap(background);
	al_destroy_event_queue(eventQueue);
}

void Game::runMainLoop() {
	while(!done) {
		ALLEGRO_EVENT events;
		al_wait_for_event(eventQueue, &events);
		al_get_keyboard_state(&keyState);

		if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			done = true;
		}
		else if(events.type == ALLEGRO_EVENT_TIMER) {
			if(events.timer.source == timer) {
				/* Here happens the checking of each keys if pressed!
				If a key is pressed, a target is set from zero to non-zero,
				so the walking animation happens smoothly, i.e., with each frame
				passing in FPS = 60 FPS. The control flag ensures that two keys do not
				get pressed at the same time (avoiding diagonal movement). */
				if(al_key_down(&keyState, ALLEGRO_KEY_DOWN)) {
					if(!controlFlag && !target[DOWN]) {
						controlFlag = true;
						target[DOWN] = y + TILE_SIZE;
					}
					dir = DOWN;
					active = true;
				}
				else if(al_key_down(&keyState, ALLEGRO_KEY_UP)) {
					if(!controlFlag && !target[UP]) {
						controlFlag = true;
						target[UP] = y - TILE_SIZE;
					}
					dir = UP;
					active = true;
				}
				else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT)) {
					if(!controlFlag && !target[RIGHT]) {
						controlFlag = true;
						target[RIGHT] = x + TILE_SIZE;
					}
					dir = RIGHT;
					active = true;
				}
				else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT)) {
					if(!controlFlag && !target[LEFT]) {
						controlFlag = true;
						target[LEFT] = x - TILE_SIZE;
					}
					dir = LEFT;
					active = true;
				}
				
				/* Here, if the target was set by the code above, then the smooth
				transition happens from the initial position to the final target.
				Independently of the value of the target, when TILE_SIZE = 16 pixels
				are done by the movement, the end of walking happens. */
				if(target[DOWN] != 0) {
					if(y != target[DOWN])
						y += 1;	
					count++;
				}
				if(target[UP] != 0) {
					if(y != target[UP])
						y -= 1;	
					count++;
				}
				if(target[RIGHT] != 0) {
					if(x != target[RIGHT])
						x += 1;	
					count++;
				}
				if(target[LEFT] != 0) {
					if(x != target[LEFT])
						x -= 1;
					count++;
				}

				if(count == TILE_SIZE) {
					for(int i = 0; i < 4; i++) {
						target[i] = 0;
					}
					count = 0;
					controlFlag = false;
				}

				/* There is a bug which happens if the target calculated value is zero,
				but the control flag was set. This code solves it, but makes it impossible
				to walk near the left and upper border of the screen. However, a possible
				workaround it create collisions in this are with, for examples, rocks or trees. */
				bool allFieldsZero = true;
				for(int i = 0; i < MAX_CONTROLS; i++) {
					if(target[i] != 0)
						allFieldsZero = false;
				}
				if(allFieldsZero && controlFlag)
					controlFlag = false;


				al_identity_transform(&camera);
				
				//al_translate_transform(&camera, SCREEN_WIDTH / 2 - x, SCREEN_HEIGHT / 2 - y);
				
				al_scale_transform(&camera, 2, 2);
				al_translate_transform(&camera, SCREEN_WIDTH / 2 - 2 * x, SCREEN_HEIGHT / 2 - 2 * y);

				al_use_transform(&camera);
			}
			else if(events.timer.source == frameTimer) {
				/* This code makes the transition of the sprites, i.e.,
				the warking animation of the player. */
				if(active) {
					sourceX += al_get_bitmap_width(player) / 3;
					active = 0;
				}
				else
					sourceX = 0;
				if(sourceX >= al_get_bitmap_width(player))
					sourceX = 0;
				sourceY = dir;	
			}

			draw = true;
		}
		if(draw)
			drawContent();
	}
}

void Game::drawContent() {
	ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(player, sourceX, sourceY * TILE_SIZE, TILE_SIZE, TILE_SIZE);
	al_draw_bitmap(background, 0, 0, 0);
	al_draw_bitmap(subBitmap, x, y, 0);

	drawGrid();

	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_destroy_bitmap(subBitmap);
}

void Game::drawGrid() {
	int gridSpacing = TILE_SIZE;
	for (int y = 0; y < SCREEN_HEIGHT; y += gridSpacing) {
		al_draw_line(0, y, SCREEN_WIDTH, y, al_map_rgb(255, 0, 0), -1);
	}
	for (int x = 0; x < SCREEN_WIDTH; x += gridSpacing) {
		al_draw_line(x, 0, x, SCREEN_HEIGHT, al_map_rgb(255, 0, 0), -1);
	}
}
