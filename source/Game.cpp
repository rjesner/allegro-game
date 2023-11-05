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
	::al_init_primitives_addon();

	display = std::shared_ptr<ALLEGRO_DISPLAY>(::al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT), ::al_destroy_display);

	::al_install_keyboard();
	::al_init_image_addon();

	player = std::shared_ptr<ALLEGRO_BITMAP>(::al_load_bitmap("../resource/player.png"), ::al_destroy_bitmap);
	background = std::shared_ptr<ALLEGRO_BITMAP>(::al_load_bitmap("../resource/forest.png"), ::al_destroy_bitmap);
 
	timer = std::shared_ptr<ALLEGRO_TIMER>(::al_create_timer(1.0 / FPS), ::al_destroy_timer);
	frameTimer = std::shared_ptr<ALLEGRO_TIMER>(::al_create_timer(1.0 / FRAME_FPS), ::al_destroy_timer);

	eventQueue = std::shared_ptr<ALLEGRO_EVENT_QUEUE>(::al_create_event_queue(), ::al_destroy_event_queue);
	::al_register_event_source(eventQueue.get(), ::al_get_timer_event_source(timer.get()));
	::al_register_event_source(eventQueue.get(), ::al_get_timer_event_source(frameTimer.get()));
	::al_register_event_source(eventQueue.get(), ::al_get_display_event_source(display.get()));
	::al_register_event_source(eventQueue.get(), ::al_get_keyboard_event_source());

	::al_start_timer(timer.get());
	::al_start_timer(frameTimer.get());
	return true;
}

void Game::runMainLoop() {
	while(!done) {
		ALLEGRO_EVENT events;
		::al_wait_for_event(eventQueue.get(), &events);
		::al_get_keyboard_state(&keyState);

		if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			done = true;
		}
		else if(events.type == ALLEGRO_EVENT_TIMER) {
			if(events.timer.source == timer.get()) {
				/* Here happens the checking of each keys if pressed!
				If a key is pressed, a target is set from zero to non-zero,
				so the walking animation happens smoothly, i.e., with each frame
				passing in FPS = 60 FPS. The control flag ensures that two keys do not
				get pressed at the same time (avoiding diagonal movement). */
				if(::al_key_down(&keyState, ALLEGRO_KEY_DOWN)) {
					if(!controlFlag && !target[DOWN]) {
						controlFlag = true;
						target[DOWN] = y + TILE_SIZE;
					}
					dir = DOWN;
					active = true;
				}
				else if(::al_key_down(&keyState, ALLEGRO_KEY_UP)) {
					if(!controlFlag && !target[UP]) {
						controlFlag = true;
						target[UP] = y - TILE_SIZE;
					}
					dir = UP;
					active = true;
				}
				else if(::al_key_down(&keyState, ALLEGRO_KEY_RIGHT)) {
					if(!controlFlag && !target[RIGHT]) {
						controlFlag = true;
						target[RIGHT] = x + TILE_SIZE;
					}
					dir = RIGHT;
					active = true;
				}
				else if(::al_key_down(&keyState, ALLEGRO_KEY_LEFT)) {
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


				::al_identity_transform(&camera);
				
				//::al_translate_transform(&camera, SCREEN_WIDTH / 2 - x, SCREEN_HEIGHT / 2 - y);
				
				::al_scale_transform(&camera, 2, 2);
				::al_translate_transform(&camera, SCREEN_WIDTH / 2 - 2 * x, SCREEN_HEIGHT / 2 - 2 * y);

				::al_use_transform(&camera);
			}
			else if(events.timer.source == frameTimer.get()) {
				/* This code makes the transition of the sprites, i.e.,
				the warking animation of the player. */
				if(active) {
					sourceX += ::al_get_bitmap_width(player.get()) / 3;
					active = 0;
				}
				else
					sourceX = 0;
				if(sourceX >= ::al_get_bitmap_width(player.get()))
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
	std::shared_ptr<ALLEGRO_BITMAP> subBitmap(::al_create_sub_bitmap(player.get(), sourceX, sourceY * TILE_SIZE,
		TILE_SIZE, TILE_SIZE), ::al_destroy_bitmap);
	::al_draw_bitmap(background.get(), 0, 0, 0);
	::al_draw_bitmap(subBitmap.get(), x, y, 0);

	drawGrid();

	::al_flip_display();
	::al_clear_to_color(::al_map_rgb(0, 0, 0));
}

void Game::drawGrid() {
	int gridSpacing = TILE_SIZE;
	for(int y = 0; y < SCREEN_HEIGHT; y += gridSpacing) {
		::al_draw_line(0, y, SCREEN_WIDTH, y, ::al_map_rgb(255, 0, 0), -1);
	}
	for(int x = 0; x < SCREEN_WIDTH; x += gridSpacing) {
		::al_draw_line(x, 0, x, SCREEN_HEIGHT, ::al_map_rgb(255, 0, 0), -1);
	}
}
