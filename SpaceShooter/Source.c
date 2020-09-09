// Space Shooter Game
// NUST SEECS BEE-9C Introduction to Programming Project
// By Zain Amir Zaman, Afaq Ahmed Khan and Asmar Ali

#define _CRT_SECURE_NO_WARNINGS  //  To disable annoying visual studio warnings that prevent the use of sprintf()

//  ***  LIBRARIES  ***  //

#include <stdio.h>  //  Standard input / output library
#include <SDL.h>  //  SDL 2.0 graphics library
#include <SDL_image.h>  //  For importing external images (sprites)
#include <SDL_ttf.h>  //  For importing and using fonts
#include <SDL_mixer.h>  //  For importing and using sound files
#include <math.h>  //  For using advanced mathematical functions
#include <time.h>  //  For using the srand() function to provide a time based seed to the existing rand() function
#include <stdbool.h>  //  For using the boolean variable type

//  ***  CONSTANTS  ***  //

//  System

#define FPS 60  //  Frames-per-second of the game
#define WINDOW_WIDTH 420 //  Width of game window in pixels
#define WINDOW_HEIGHT 540  //  Height of game window in pixels
#define PI 3.14159  //  Value of pi

//  Font sizes

#define MAIN_FONT_SIZE 17
#define TITLE_FONT_SIZE 39
#define TITLESCREEN_SELECTIONS_FONT_SIZE 28

//  Stages

#define STAGE_ABOUT 0
#define STAGE_TITLESCREEN 1
#define STAGE_OPTIONS 2
#define STAGE_GAME 3
#define STAGE_PAUSESCREEN 4
#define STAGE_GAMEOVER 5
#define STAGE_CREDITS 6

//  Scoring

#define SCORE_INCREASE_ENEMY_TYPE_1 100
#define SCORE_INCREASE_ENEMY_TYPE_2 200
#define SCORE_INCREASE_ENEMY_TYPE_3 260

//  Player ship

#define MAX_LIVES 3
#define PLAYER_SPEED 5

//  Player bullet

#define PLAYER_BULLET_ARRAY_SIZE 60
#define PLAYER_BULLET_ARRAY_NUM_PROPERTIES 7
#define PLAYER_BULLET_SPEED 17
#define PLAYER_BULLET_DELAY 4
#define PLAYER_BULLET_INTERBULLET_DISTANCE 12

#define PLAYER_BULLET_UPGRADED_ANGLE_DEV 0.05
#define PLAYER_BULLET_UPGRADED_OFFSET_SCALE_FACTOR 2.5

//  Upgrades

#define UPGRADE_NONE 0
#define UPGRADE_TYPE_ATTACK 1
#define UPGRADE_TYPE_HELPER 2

#define HELPER_RADIUS 50
#define HELPER_SPEED 0.1
#define HELPER_BULLET_SPEED 17

#define UPGRADE_DURATION 500

//  Enemy ships

#define ENEMY_ARRAY_SIZE 100 
#define ENEMY_ARRAY_NUM_PROPERTIES 9
#define STATUS_NONEXISTENT 0
#define STATUS_EXISTS 1

#define ENEMY_TYPE_1 0
#define ENEMY_TYPE_2 1
#define ENEMY_TYPE_3 2

#define ENEMY_TYPE_1_DEFAULT_HEALTH 5
#define ENEMY_TYPE_2_DEFAULT_HEALTH 8
#define ENEMY_TYPE_3_DEFAULT_HEALTH 3

#define ENEMY_SPAWN_DELAY 200
#define ENEMY_SPAWN_DELAY_MAX_DEVIATION 80

#define ENEMY_NUM_AVG 5
#define ENEMY_NUM_MAX_DEVIATION 2

#define ENEMY_TYPE_1_VEL_AVG 4
#define ENEMY_TYPE_1_VEL_MAX_DEVIATION 2

#define ENEMY_TYPE_2_SPAWN_CHANCE_PERCENT 20
#define ENEMY_TYPE_2_VEL_AVG 2
#define ENEMY_TYPE_2_VEL_MAX_DEVIATION 1
#define ENEMY_TYPE_2_POSTATTACK_DELAY 120

#define ENEMY_TYPE_3_SPAWN_CHANCE_PERCENT 30
#define ENEMY_TYPE_3_INITIAL_Y_VEL 4
#define ENEMY_TYPE_3_X_VEL 4
#define ENEMY_TYPE_3_POSTATTACK_DELAY 100

//  Sprite dimensions

#define SPRITE_PLAYERSHIP_W 30
#define SPRITE_PLAYERSHIP_H 30
#define SPRITE_BULLET_W 2
#define SPRITE_BULLET_H 18

#define SPRITE_HELPER_W 9
#define SPRITE_HELPER_H 8

#define SPRITE_ENEMY_TYPE_1_W 22
#define SPRITE_ENEMY_TYPE_1_H 24
#define SPRITE_ENEMY_TYPE_2_W 20
#define SPRITE_ENEMY_TYPE_2_H 33
#define SPRITE_ENEMY_TYPE_3_W 28
#define SPRITE_ENEMY_TYPE_3_H 13

#define SPRITE_UPGRADE_W 20
#define SPRITE_UPGRADE_H 20

//  Difficulty levels

#define DIFFICULTY_LV1 0
#define DIFFICULTY_LV2 1
#define DIFFICULTY_LV3 2
#define DIFFICULTY_LV_MAX 3

#define DIFFICULTY_LV1_DURATION 5  //  Duration in seconds
#define DIFFICULTY_LV2_DURATION 5
#define DIFFICULTY_LV3_DURATION 5

//  ***  GLOBAL VARIABLES  ***  //

int game_stage = STAGE_TITLESCREEN;

int game_score;
size_t game_quit;
size_t difficulty_level;
float time_elapsed_beforegame, time_elapsed_game;

//  --  ENEMY ARRAY  --  //

//  The ID and properties of each enemy are contained in the 2D array enemy[][]. 
//  The first dimension defines the maximum possible number of enemies on-screen.
//  The second dimension contains the properties unique to each enemy.

//  These properties are:

//  0: type
//  1: x position
//  2: y position
//  3: velocity
//  4: sprite width
//  5: sprite height
//  6: status (dead or alive)
//  7: health
//  8: counter for showing health bar

int enemy[ENEMY_ARRAY_SIZE][ENEMY_ARRAY_NUM_PROPERTIES];
float enemy_type_2_additional_attributes[ENEMY_ARRAY_SIZE][3]; // 0: delay, 1: resume tracking status, 2: angle
int enemy_type_3_additional_attributes[ENEMY_ARRAY_SIZE][2];  //  0: delay, 1: resume tracking status

//  --  PLAYER BULLET ARRAY  --  //

//  0: x position, 1: y position, 2: velocity, 3: angle from horizontal, 4: status of existence, 5: ID
//  6: angle

int player_bullet[PLAYER_BULLET_ARRAY_SIZE][PLAYER_BULLET_ARRAY_NUM_PROPERTIES];
float player_bullet_angle_holder[PLAYER_BULLET_ARRAY_SIZE][2]; // 0 - angle, 1 - ID

//  NOTE: A separate array for holding the angle of the bullet has been created because
//        the angle is a float variable so it cannot be stored in the integer array 'player_bullet[][]'

//  --  ENEMY BULLET ARRAY  --  //

//  0: x position, 1: y position, 2: velocity, 3: angle from horizontal, 4: status of existence, 5: ID
//  6: angle

int enemy_bullet[PLAYER_BULLET_ARRAY_SIZE][PLAYER_BULLET_ARRAY_NUM_PROPERTIES];
float enemy_bullet_angle_holder[PLAYER_BULLET_ARRAY_SIZE][2]; // 0 - angle, 1 - ID

//  --  HELPER BULLET ARRAY  --  //

//int helper_bullet[5][3];  //  0: x position, 1: y position, 2: status of existence
//float helper_bullet_angle_holder[5][2]; // 0 - angle, 1 - ID

//  Keyboard input variables

size_t key_up, key_down, key_left, key_right, key_space, key_escape, key_bullet;

//  Mouse coordinates

int mouse_x, mouse_y;

//  Player ship attributes

int playership_x, playership_y;
int playership_w, playership_h;
float playership_x_vel, playership_y_vel;
int playership_lives;
float playership_angle;
size_t playership_is_dead; 

int playership_respawn_counter;
int playership_respawn_counter_max;

int playership_invisibility_counter;
int player_invisibility_counter_max;

int bullet_x, bullet_y, bullet_w, bullet_h;
float bullet_x_pos, bullet_y_pos;

float pe_distance = 500;
int playership_fire_rate;

//  Player ship upgrades

size_t current_upgrade = UPGRADE_NONE;

//  Enemy ship attributes

int enemy_vel;

int enemy_time_counter, enemies_onscreen;

//  Upgrades

int upgrade_type, upgrade_x, upgrade_y, upgrade_exists = 1;
int upgrade_time_left = 0;

int helper_x, helper_y;
int helper_radius;

int helper_bullet_x, helper_bullet_y;
float helper_bullet_angle;
int helper_bullet_exists;

//  SDL Variables

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Texture* tex_playership;
SDL_Texture* tex_enemyship;
SDL_Texture* tex_player_bullet;
SDL_Texture* tex_player_bullet_upgraded;
SDL_Texture* tex_homingmissile;
SDL_Texture* tex_shield;
SDL_Texture* tex_helper;
SDL_Texture* tex_helper_mine;
SDL_Texture* tex_helper_mine_explosion;
SDL_Texture* tex_upgrade_attack;
SDL_Texture* tex_upgrade_homingmissile;
SDL_Texture* tex_upgrade_shield;
SDL_Texture* tex_upgrade_helper;
SDL_Texture* tex_enemyship_type2;
SDL_Texture* tex_enemyship_type3;
SDL_Texture* tex_enemyship_type2_bullet;
SDL_Texture* tex_enemyship_type3_bullet;

TTF_Font* main_font;
TTF_Font* title_font;
TTF_Font* titlescreen_options_font;
TTF_Font* gameover_font;

Mix_Music* titlescreen_bgm;
Mix_Chunk* snd_playership_bullet_std;
Mix_Chunk* snd_destroyed;
Mix_Chunk* snd_fire_missile;
Mix_Chunk* snd_select;
Mix_Chunk* snd_enemy_fire;

//  ***  FUNCTION PROTOTYPES  ***  //

float radToDegree(float);
int randomRange(int, int);

void initSDL_GraphicsLib(void);
void loadSprites(void);
void initSDL_TTFLib(void);
void initSDL_MixerLib(void);

void processControls(void);
void processPlayershipMovement(void);
void processPlayershipRotation(void);
void playershipFireBullets(void);

void spawnEnemies();
void processEnemyMovement();
int playerEnemyCollision(int i);
void killEnemy(int i);
void renderGraphics();
void drawText(char text[], int r, int g, int b, int text_x, int text_y);
void enemyFireBullet(int i);
void enemyBulletMovement(void);
void processUpgrades(void);
int playerUpgradeCollision(void);
void killPlayership(void);
void respawnPlayership(void);
void determineEnemyType(int i, int *enemy_w, int *enemy_h);
void gameOver(void);
void initVariables(void);
void processHelperUpgrade(void);

void processEnemyType1Movement(int i);
void processEnemyType2Movement(int i);
void processEnemyType3Movement(int i);

//  ***  FUNCTIONS  ***  //

//  processControls(): Checks for keyboard + mouse inputs

void processControls(void) {

	SDL_Event event;

	//  Check for event type

	while (SDL_PollEvent(&event)) {
		switch (event.type) {  //  Check which event is currently occuring
		case SDL_QUIT:  //  If the close button is clicked, stop game loop
			game_quit = 1;
			break;
		case SDL_KEYDOWN:  //  If any key is pressed...
			switch (event.key.keysym.scancode) {   //  Check which key is pressed
			case SDL_SCANCODE_W:
			case SDL_SCANCODE_UP:
				key_up = 1;
				break;
			case SDL_SCANCODE_A:
			case SDL_SCANCODE_LEFT:
				key_left = 1;
				break;
			case SDL_SCANCODE_S:
			case SDL_SCANCODE_DOWN:
				key_down = 1;
				break;
			case SDL_SCANCODE_D:
			case SDL_SCANCODE_RIGHT:
				key_right = 1;
				break;
			case SDL_SCANCODE_ESCAPE:
				key_escape = 1;
				if ((game_stage == STAGE_GAME) || (game_stage == STAGE_PAUSESCREEN)) {
					if (game_stage == STAGE_GAME) game_stage = STAGE_PAUSESCREEN; else game_stage = STAGE_GAME;
				}
				break;
			case SDL_SCANCODE_U:
				//if (current_upgrade == UPGRADE_NONE) current_upgrade = UPGRADE_TYPE_ATTACK; else current_upgrade = UPGRADE_NONE;  //  debug purposes
				break;
			case SDL_SCANCODE_SPACE:
				key_space = 1;
				break;
			}
			break;
		case SDL_KEYUP:  //  If any key is released...
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_W:
			case SDL_SCANCODE_UP:
				key_up = 0;
				break;
			case SDL_SCANCODE_A:
			case SDL_SCANCODE_LEFT:
				key_left = 0;
				break;
			case SDL_SCANCODE_S:
			case SDL_SCANCODE_DOWN:
				key_down = 0;
				break;
			case SDL_SCANCODE_D:
			case SDL_SCANCODE_RIGHT:
				key_right = 0;
				break;
			case SDL_SCANCODE_SPACE:
				key_space = 0;
				break;
			case SDL_SCANCODE_ESCAPE:
				key_escape = 0;
				break;
			}
			break;

			//  Update mouse coordinates

		case SDL_MOUSEMOTION:  //  If mouse movement is detected...
			mouse_x = event.motion.x;
			mouse_y = event.motion.y;
			break;

		case SDL_MOUSEBUTTONDOWN: 
			if (event.button.button == SDL_BUTTON_LEFT) key_bullet = 1;
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) key_bullet = 0;
			break;
		default:
			game_quit = 0;
		}
	}
}

//  introScreen(): Code for intro screen

void aboutScreen(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
	SDL_RenderClear(renderer);

	char text[450] = "";

	sprintf(text, "NUST SEECS ELECTRICAL ENGINEERING FIRST-SEMESTER 2017-18\nINTRODUCTION TO PROGRAMMING END-OF-SEMESTER PROJECT\n\nSECTION: BEE-9C\n\nBY:\n\nZAIN AMIR ZAMAN (installing and implementing SDL libraries for graphics + sound + fonts, core game engine including player movement, enemies, bullets...)\n\nAFAQ AHMED KHAN (upgrades system)\n\nASMAR ALI (player ship rotation, randomRange() function, collision detection algorithm)\n\n");

	drawText(text, 255, 255, 255, 10, 10);

	drawText("Press ESC to go back", 255, 255, 255, 10, WINDOW_HEIGHT - 30);

	if (key_escape) game_stage = STAGE_TITLESCREEN;

	SDL_RenderPresent(renderer);

	SDL_Delay(1000 / FPS);
}

//  titelScreen(): Shows titlescreen

void titleScreen(void) {

	int selections_x_pos = 160;

	static size_t selection_counter = 1;
	static size_t total_selections = 3;
	static size_t key_pressed = 0;

	if ((key_up) && (!key_pressed)) {
		if (selection_counter > 1) {
			selection_counter--;
		}
		else {
			selection_counter = total_selections;
		}

		key_pressed = 1;
	}
	else if ((key_down) && (!key_pressed)) {
		if (selection_counter < total_selections) {
			selection_counter++;
		}
		else {
			selection_counter = 1;
		}

		key_pressed = 1;
	}

	if ((!key_up) && (!key_down)) key_pressed = 0;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
	SDL_RenderClear(renderer);

	char game_title[40] = "SPACE SHOOTER";
	char play[30] = "PLAY";
	char about[30] = "ABOUT";
	char exit[30] = "EXIT";
	char help[70] = "Use arrow keys or WSAD to navigate menu\nPress space to select";

	SDL_Color color_title = { 255, 255, 255 };
	SDL_Color color_unselected = { 255, 255, 255 };
	SDL_Color color_selected = { 38, 176, 106 };

	//  Create title text

	SDL_Surface *surface_game_title = TTF_RenderText_Blended_Wrapped(title_font, game_title, color_title, 300);
	SDL_Texture *tex_game_title = SDL_CreateTextureFromSurface(renderer, surface_game_title);
	SDL_Rect rect_game_title = {(WINDOW_WIDTH - surface_game_title->w) / 2, 80, surface_game_title->w , surface_game_title->h};
	SDL_FreeSurface(surface_game_title);
	SDL_RenderCopy(renderer, tex_game_title, NULL, &rect_game_title);

	//  Create PLAY text

	SDL_Surface *surface_play = NULL;

	if (selection_counter == 1) {
		surface_play = TTF_RenderText_Blended_Wrapped(titlescreen_options_font, play, color_selected, 300);
	}
	else {
		surface_play = TTF_RenderText_Blended_Wrapped(titlescreen_options_font, play, color_unselected, 300);
	}
	SDL_Texture *tex_play = SDL_CreateTextureFromSurface(renderer, surface_play);
	SDL_Rect rect_play = { selections_x_pos, 200, surface_play->w , surface_play->h };
	SDL_RenderCopy(renderer, tex_play, NULL, &rect_play);

	//  Create ABOUT text

	SDL_Surface *surface_about = NULL;

	if (selection_counter == 2) {
		surface_about = TTF_RenderText_Blended_Wrapped(titlescreen_options_font, about, color_selected, 300);
	}
	else {
		surface_about = TTF_RenderText_Blended_Wrapped(titlescreen_options_font, about, color_unselected, 300);
	}
	SDL_Texture *tex_about = SDL_CreateTextureFromSurface(renderer, surface_about);
	SDL_Rect rect_about = { selections_x_pos, 200 + surface_play->h + 20, surface_about->w , surface_about->h };

	SDL_RenderCopy(renderer, tex_about, NULL, &rect_about);

	//  Create EXIT text

	SDL_Surface *surface_exit = NULL;

	if (selection_counter == 3) {
		surface_exit = TTF_RenderText_Blended_Wrapped(titlescreen_options_font, exit, color_selected, 300);
	}
	else {
		surface_exit = TTF_RenderText_Blended_Wrapped(titlescreen_options_font, exit, color_unselected, 300);
	}
	SDL_Texture *tex_exit = SDL_CreateTextureFromSurface(renderer, surface_exit);
	SDL_Rect rect_exit = { selections_x_pos, 200 + surface_play->h + surface_about->h + 40, surface_exit->w , surface_exit->h };

	SDL_RenderCopy(renderer, tex_exit, NULL, &rect_exit);

	//  Create help text

	SDL_Surface *surface_help = TTF_RenderText_Blended_Wrapped(main_font, help, color_title, 300);
	SDL_Texture *tex_help = SDL_CreateTextureFromSurface(renderer, surface_help);
	SDL_Rect rect_help = { 30, WINDOW_HEIGHT - 30 - surface_help->h, surface_help -> w , surface_help->h };
	SDL_FreeSurface(surface_help);
	SDL_RenderCopy(renderer, tex_help, NULL, &rect_help);

	//  ...

	if (key_space) {

		Mix_Volume(1, 100);
		Mix_PlayChannel(1, snd_select, 0);

		switch (selection_counter) {
		case 1:
			time_elapsed_beforegame = SDL_GetTicks();
			game_stage = STAGE_GAME;
			break;
		case 2:
			game_stage = STAGE_ABOUT;
			break;
		case 3:
			game_quit = 1;
			break;
		}
	}

	//  Clear surfaces

	SDL_FreeSurface(surface_play);
	SDL_FreeSurface(surface_about);
	SDL_FreeSurface(surface_exit);

	SDL_RenderPresent(renderer);
	SDL_Delay(1000 / FPS);
}

//  gameOver(): Code for game over screen

void gameOver(void) {

	if (key_space) {
		initVariables();  //  Reset all variables
		game_stage = STAGE_TITLESCREEN;  //  Go to titlescreen
	}

	//  Render graphics

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
	SDL_RenderClear(renderer);

	char gameover_text[30] = "";
	char score_text[200] = "";
	char instruction_text[40] = "";

	sprintf(gameover_text, "GAME OVER");
	sprintf(score_text, "Your score: %d", game_score);
	sprintf(instruction_text, "Press space to return to titlescreen");

	SDL_Color color = { 255, 255, 255 };

	SDL_Surface *message = TTF_RenderText_Blended_Wrapped(title_font, gameover_text, color, 300);
	SDL_Texture *message_tex = SDL_CreateTextureFromSurface(renderer, message);
	SDL_Rect text_container = { (WINDOW_WIDTH - message->w) / 2, (WINDOW_HEIGHT - message->h) / 2 - 60, message->w, message->h };

	SDL_RenderCopy(renderer, message_tex, NULL, &text_container);

	SDL_Surface *message2 = TTF_RenderText_Blended_Wrapped(main_font, score_text, color, 300);
	SDL_Texture *message2_tex = SDL_CreateTextureFromSurface(renderer, message2);
	SDL_Rect text2_container = { (WINDOW_WIDTH - message2->w) / 2, (WINDOW_HEIGHT - message2->h) / 2 + 100, message2->w, message2->h };
	
	SDL_RenderCopy(renderer, message2_tex, NULL, &text2_container);

	SDL_Surface *message3 = TTF_RenderText_Blended_Wrapped(main_font, instruction_text, color, 300);
	SDL_Texture *message3_tex = SDL_CreateTextureFromSurface(renderer, message3);
	SDL_Rect text3_container = { 20, WINDOW_HEIGHT - message3->h - 20, message3->w, message3->h };

	SDL_RenderCopy(renderer, message3_tex, NULL, &text3_container);

	SDL_FreeSurface(message);
	SDL_FreeSurface(message2);
	SDL_FreeSurface(message3);

	SDL_RenderPresent(renderer);
	SDL_Delay(1000 / FPS);
}

//  pauseGame(): Pauses the game

void pauseGame(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
	SDL_RenderClear(renderer);
	char text[100] = "";

	sprintf(text, "PAUSE SCREEN\n\nPRESS ESC TO RETURN");

	drawText(text, 255, 255, 255, 20, 20);

	SDL_RenderPresent(renderer);

	SDL_Delay(1000 / FPS);
}

//  initVariables(): Initializes all global variables

void initVariables(void) {

	game_score = 0;
	difficulty_level = DIFFICULTY_LV1;

	time_elapsed_beforegame = 0;
	time_elapsed_game = 0;

	//  --  USER INPUT VARIABLES  --  //

	key_up = 0;  //  Becomes 1 if UP or W key is pressed
	key_down = 0;  //  Becomes 1 if DOWN or S key is pressed 
	key_left = 0;  //  Becomes 1 if LEFT or A key is pressed 
	key_right = 0;  //  Becomes 1 if RIGHT or D key is pressed
	key_escape = 0;  //  Becomes 1 if ESCAPE key is pressed
	key_bullet = 0;  //  Becomes 1 if LEFT MOUSE BUTTON is pressed
	key_space = 0;  //  Becomes 1 if SPACE is pressed

	mouse_x = 0;  //  x coordinate of the mouse position in the game window
	mouse_y = 0;  //  y coordinate of the mouse position in the game window

	//  --  PLAYER SHIP ATTRIBUTES  --  //

	playership_x = (WINDOW_WIDTH - playership_w) / 2;  //  x coordinate of player ship (set to centre of game window)
	playership_y = (WINDOW_HEIGHT - playership_h) / 2 + 150;  //  y coordinate of player ship (set a little below the centre of the window)
	playership_w = SPRITE_PLAYERSHIP_W;  //  Width of player ship sprite
	playership_h = SPRITE_PLAYERSHIP_H;  //  Height of player ship sprite
	playership_x_vel = 0;  //  x velocity of player ship
	playership_y_vel = 0;  //  y velocity of player ship
	playership_lives = MAX_LIVES;  //  Number of lives of player ship
	playership_angle = 0.5 * PI;    //  Angle between the horizontal and the line joining the centre of the player ship and the the mouse
	playership_is_dead = 0;  //  Becomes 1 if player ship loses all its lives / health

	playership_respawn_counter = 0;
	playership_respawn_counter_max = 100;

	playership_invisibility_counter;
	player_invisibility_counter_max = 150;
	
	playership_fire_rate = 2;

	current_upgrade = UPGRADE_NONE;

	upgrade_type = 0; 
	upgrade_exists = 0;
	upgrade_time_left = 0;

	helper_radius = HELPER_RADIUS;
	helper_bullet_angle = 0;
	helper_bullet_exists = 0;

	//  --  ENEMY SHIP ATTRIBUTES  --  //

	enemy_time_counter = 0;
	enemies_onscreen = 0;

	//  Define the enemy array

	for (int i = 0; i < ENEMY_ARRAY_SIZE; i++) {
		enemy[i][0] = i;  //  Define ID of each enemy slot in the array
		enemy[i][6] = STATUS_NONEXISTENT;  //  Set enemy status as dead for all enemies

		enemy_type_3_additional_attributes[i][0] = ENEMY_TYPE_3_POSTATTACK_DELAY;
		enemy_type_3_additional_attributes[i][1] = 1;
	}

	//  Define player bullet array

	for (int i = 0; i < PLAYER_BULLET_ARRAY_SIZE; i++) {
		player_bullet[i][3] = STATUS_NONEXISTENT;
	}

	//  Define enemy bullet array

	for (int i = 0; i < PLAYER_BULLET_ARRAY_SIZE; i++) {
		enemy_bullet[i][3] = STATUS_NONEXISTENT;
	}
}

//  processDifficulty(): Adjusts difficulty level based on time elapsed

void processDifficulty(void) {
	switch ((int)time_elapsed_game) {
	case DIFFICULTY_LV1_DURATION:
		difficulty_level = DIFFICULTY_LV2;
		break;
		
	case DIFFICULTY_LV1_DURATION + DIFFICULTY_LV2_DURATION:
		difficulty_level = DIFFICULTY_LV3;
		break;

	case DIFFICULTY_LV1_DURATION + DIFFICULTY_LV2_DURATION + DIFFICULTY_LV3_DURATION:
		difficulty_level = DIFFICULTY_LV_MAX;
		break;
	}
}

//  processPlayershipMovement(): Allows movement of player ship as directed by
//                               keyboard inputs

void processPlayershipMovement(void) {
	
	if (!playership_is_dead) {

		//  Reset velocities if no input is given

		playership_x_vel = 0;
		playership_y_vel = 0;

		//  Assign velocity depending on keybard input

		if (key_up && !key_down) playership_y_vel = -PLAYER_SPEED;
		if (key_down && !key_up) playership_y_vel = PLAYER_SPEED;
		if (key_left && !key_right) playership_x_vel = -PLAYER_SPEED;
		if (key_right && !key_left) playership_x_vel = PLAYER_SPEED;

		playership_x += playership_x_vel;
		playership_y += playership_y_vel;

		//  Collision detection with window boundaries to prevent the player ship from going out of the
		//  window

		if (playership_x <= 0) playership_x = 0;
		if (playership_y <= 0) playership_y = 0;
		if (playership_x >= WINDOW_WIDTH - playership_w) playership_x = WINDOW_WIDTH - playership_w;
		if (playership_y >= WINDOW_HEIGHT - playership_h) playership_y = WINDOW_HEIGHT - playership_h;

		//  If player ship is hit by a bullet, kill the player

		if ((playerBulletCollision()) && (playership_respawn_counter == 0)) killPlayership();
	}

	if (playership_respawn_counter > 0) {
		playership_respawn_counter--;
	}
	else if (playership_is_dead) {
		playership_invisibility_counter = player_invisibility_counter_max;
		respawnPlayership(); 
	}

	if (playership_invisibility_counter > 0) playership_invisibility_counter--;
}

//  processPlayershipRotation(): Makes the player ship face the mouse if KBD +
//                               MOUSE mode is on

void processPlayershipRotation(void) {

	//  dx: x distance between mouse_x and playership_x
	//  dy: y distance between mouse_y and playership_y
	//  angle_with_hor: angle (in degrees) made between the line
	//                  joining the playership centre with the mouse
	//                  position and the horizontal x-axis

	float dx, dy, angle_with_hor;

	dx = (mouse_x - playership_x - playership_w / 2);
	dy = (mouse_y - playership_y - playership_h / 2);

	angle_with_hor = atan((float)dy / dx);

	if (angle_with_hor < 0) angle_with_hor *= -1;

	//  IMPORTANT BACKGROUND INFO: 
	//  + In the game window, the origin is at the top left corner
	//  + As expected, x values increase as we go from left to right
	//  + However, y values also increase as we move from top to bottom 
	//    (instead of decreasing, similar to an inverted y-axis)

	//  The block of code below is necessary because angle_with_hor
	//  always give the angle with respect to the horizontal x
	//  axis, which means that it won't go above 90 degrees, making
	//  it useless. To get angles from 0 all the way to 360 we must 
	//  find the angle with respect to the positive horizontal x-axis.
	//  This can be done by taking a look considering the four quadrants
	//  of the Cartesian plane

	if ((dx >= 0) && (dy <= 0)) playership_angle = angle_with_hor;  //  First quadrant
	if ((dx < 0) && (dy < 0)) playership_angle = PI - angle_with_hor;  //  Second quadrant
	if ((dx < 0) && (dy > 0)) playership_angle = PI + angle_with_hor;  //  Third quadrant
	if ((dx > 0) && (dy > 0)) playership_angle = 2 * PI - angle_with_hor;  //  Fourth quadrant
}

//  killPlayer(): Destroys the player ship

void killPlayership(void) {

	//  Play sound effect

	Mix_Volume(1, 100);
	Mix_PlayChannel(1, snd_destroyed, 0);

	playership_is_dead = 1;
	playership_lives--;
	playership_respawn_counter = playership_respawn_counter_max;
}

//  respawnPlayer(): Creates the playership

void respawnPlayership(void) {
	playership_x = (WINDOW_WIDTH - playership_w) / 2;
	playership_y = (WINDOW_HEIGHT - playership_h) / 2 + 150;
	playership_is_dead = 0;
}

//  playershipFireBullets(): Allows the player ship to fire bullets if left mouse button is pressed

void playershipFireBullets(void) {

	if (!playership_is_dead) {
		if (current_upgrade == UPGRADE_NONE) {
			playership_fire_rate = 2;
		}
		else if (current_upgrade == UPGRADE_TYPE_ATTACK) {
			playership_fire_rate = 4;
		}

		static size_t bullet_timer = 0;  //  Timer for firing bullets
		static size_t i = 0;  //  ID of an element in the bullet array
		static size_t bullets_fired = 0;  //  Counter for number of bullets fired per round

		if (key_bullet) bullet_timer++; else bullet_timer = PLAYER_BULLET_DELAY - 1;

		if ((key_bullet) && (bullet_timer == PLAYER_BULLET_DELAY)) {

			//  'offset' is the distance between the playership centre and each of the two bullets

			int offset = PLAYER_BULLET_INTERBULLET_DISTANCE / 2;

			//  If the attack upgrade is active, the last two extra bullets should diverge a little away from
			//  the direction of fire. 'upgraded_angle_dev' will store the value of this angular deviation

			float upgraded_angle_dev = 0;

			//  The while loop below runs up till 'fire_rate' times

			while (bullets_fired < playership_fire_rate) {

				bullets_fired++;

				if (current_upgrade != UPGRADE_TYPE_ATTACK) {
					if (player_bullet[i][3] == STATUS_NONEXISTENT) {
						player_bullet[i][3] = STATUS_EXISTS;
						player_bullet[i][0] = playership_x + playership_w / 2 + (SPRITE_PLAYERSHIP_W / 2) * cos(playership_angle) - offset * sin(playership_angle);
						player_bullet[i][1] = playership_y + playership_h / 2 - (SPRITE_PLAYERSHIP_H / 2) * sin(playership_angle) - offset * cos(playership_angle);
						player_bullet_angle_holder[i][1] = playership_angle;

						offset *= -1;  //  Invert offset for the other bullet
					}
				}
				else {
					if (player_bullet[i][3] == STATUS_NONEXISTENT) {

						player_bullet[i][3] = STATUS_EXISTS;
						player_bullet[i][0] = playership_x + playership_w / 2 + (SPRITE_PLAYERSHIP_W / 2) * cos(playership_angle) - offset * sin(playership_angle);
						player_bullet[i][1] = playership_y + playership_h / 2 - (SPRITE_PLAYERSHIP_H / 2) * sin(playership_angle) - offset * cos(playership_angle);
						player_bullet_angle_holder[i][1] = playership_angle + upgraded_angle_dev;

						if (bullets_fired == 2) {
							offset *= PLAYER_BULLET_UPGRADED_OFFSET_SCALE_FACTOR;  //  Scale offset once per round to fire the last 2 bullets at a farther distance from the player ship centre
							upgraded_angle_dev = PLAYER_BULLET_UPGRADED_ANGLE_DEV;  //  Makes the other two bullets diverge slightly from direction of fire to ensure that the upgrade results in the bullets covering a greater area to take down more enemies
						}

						if (bullets_fired > 2) upgraded_angle_dev *= -1;  //  Invert angle deviation for last bullet
						offset *= -1;  //  Alternatingly invert offset for subsequent bullets
					}
				}

				Mix_Volume(1, 25);  //  Adjust volume
				Mix_PlayChannel(1, snd_playership_bullet_std, 0);  //  Play firing sound effect
				i++;
			}

			bullet_timer = 0;
			bullets_fired = 0;
		}

		if (i >= PLAYER_BULLET_ARRAY_SIZE - 1) i = 0;
	}
}

//  playerBulletMovement():  Makes the player bullet move in the direction of the mouse

void playerBulletMovement(void) {
	for (int i = 0; i < PLAYER_BULLET_ARRAY_SIZE; i++) {
		if (player_bullet[i][3] == STATUS_EXISTS) {  //  Check if bullet with ID i exists
			float x_vel = PLAYER_BULLET_SPEED * cos(player_bullet_angle_holder[i][1]);  //  x-component of velocity
			float y_vel = -PLAYER_BULLET_SPEED * sin(player_bullet_angle_holder[i][1]);  //  y-component of velocity

			player_bullet[i][0] += (int)x_vel;
			player_bullet[i][1] += (int)y_vel;

			if ((player_bullet[i][0] < 0) || (player_bullet[i][0] > WINDOW_WIDTH)) player_bullet[i][3] = STATUS_NONEXISTENT;
			if ((player_bullet[i][1] < 0) || (player_bullet[i][1] > WINDOW_HEIGHT)) player_bullet[i][3] = STATUS_NONEXISTENT;
		}
	}
}

//  spawnEnemies(): Randomly spawns enemies from the top portion of the screen

void spawnEnemies(void) {

	//  Randomize the delay for spawning enemies

	int delay = ENEMY_SPAWN_DELAY + randomRange(-ENEMY_SPAWN_DELAY_MAX_DEVIATION, ENEMY_SPAWN_DELAY_MAX_DEVIATION);

	if (enemy_time_counter >= delay) {

		//  Randomize the number of enemies to spawn

		int num_of_enemies = ENEMY_NUM_AVG + randomRange(-ENEMY_NUM_MAX_DEVIATION, ENEMY_NUM_MAX_DEVIATION);

		if (difficulty_level == DIFFICULTY_LV_MAX) {  //  If maximum difficulty is reached...

			int time_upto_max = DIFFICULTY_LV1_DURATION + DIFFICULTY_LV2_DURATION + DIFFICULTY_LV3_DURATION;
			
			//  Increase enemies to spawn after every 8 sec at maximum difficulty uptil 40 enemies

			if (num_of_enemies < 40) {
				num_of_enemies += (time_elapsed_game - time_upto_max) / 8;
			}
			else {
				num_of_enemies = 40;
			}
		}

		//  Iterate over enemy array and spawn up to num_of_enemies

		for (int i = 0; i < num_of_enemies; i++) {
			if (enemy[i][6] == STATUS_NONEXISTENT) {  //  Spawn only when the enemy with ID i doesn't exist yet
				
				int enemy_w = 0, enemy_h = 0;

				determineEnemyType(i, &enemy_w, &enemy_h);

				enemy[i][1] = rand() % (WINDOW_WIDTH - 80) + 40;  //  Set random x position along window width
				enemy[i][2] = -20;  //  Set y position a little above the top portion of the screen
				enemy[i][4] = enemy_w;  //  Set width of enemy sprite
				enemy[i][5] = enemy_h;  //  Set height of enemy sprite
				enemy[i][6] = STATUS_EXISTS;  //  Set status of enemy with ID i to alive
				enemy[i][8] = 0;  //  Set counter that shows health bar upon hit to 0
				
				enemies_onscreen++;  //  Increment enemy counter
			}
			else {

				//  If enemy with ID i is alive, skip to the next ID but keep the number of 
				//  enemies that have to be spawned constant

				num_of_enemies++;
			}
		}

		enemy_time_counter = 0;  //  Reset enemy time counter
	}

	enemy_time_counter++;
}

//  

void determineEnemyType(int i, int *enemy_w, int *enemy_h) {
	int chance;

	switch (difficulty_level) {
	case DIFFICULTY_LV1:
		enemy[i][0] = ENEMY_TYPE_1;
		enemy[i][7] = ENEMY_TYPE_1_DEFAULT_HEALTH;
		*enemy_w = SPRITE_ENEMY_TYPE_1_W;
		*enemy_h = SPRITE_ENEMY_TYPE_1_H;
		enemy[i][3] = rand() % ENEMY_TYPE_1_VEL_MAX_DEVIATION + ENEMY_TYPE_1_VEL_AVG;
		break;

	case DIFFICULTY_LV2:
		if (rand() % 100 + 1 <= ENEMY_TYPE_2_SPAWN_CHANCE_PERCENT) {
			enemy[i][0] = ENEMY_TYPE_2;
			enemy[i][7] = ENEMY_TYPE_2_DEFAULT_HEALTH;
			*enemy_w = SPRITE_ENEMY_TYPE_2_W;
			*enemy_h = SPRITE_ENEMY_TYPE_2_H;
			enemy[i][3] = rand() % ENEMY_TYPE_2_VEL_MAX_DEVIATION + ENEMY_TYPE_2_VEL_AVG;
		}
		else {
			enemy[i][0] = ENEMY_TYPE_1;
			enemy[i][7] = ENEMY_TYPE_1_DEFAULT_HEALTH;
			*enemy_w = SPRITE_ENEMY_TYPE_1_W;
			*enemy_h = SPRITE_ENEMY_TYPE_1_H;
			enemy[i][3] = rand() % ENEMY_TYPE_1_VEL_MAX_DEVIATION + ENEMY_TYPE_1_VEL_AVG;
		}
		break;

	case DIFFICULTY_LV3:
	case DIFFICULTY_LV_MAX:
		chance = rand() % 100 + 1;

		if (chance <= ENEMY_TYPE_2_SPAWN_CHANCE_PERCENT) {
			enemy[i][0] = ENEMY_TYPE_2;
			enemy[i][7] = ENEMY_TYPE_2_DEFAULT_HEALTH;
			*enemy_w = SPRITE_ENEMY_TYPE_2_W;
			*enemy_h = SPRITE_ENEMY_TYPE_2_H;
			enemy[i][3] = rand() % ENEMY_TYPE_2_VEL_MAX_DEVIATION + ENEMY_TYPE_2_VEL_AVG;
		}
		else if ((chance > ENEMY_TYPE_2_SPAWN_CHANCE_PERCENT) && (chance <= ENEMY_TYPE_3_SPAWN_CHANCE_PERCENT)) {
			enemy[i][0] = ENEMY_TYPE_3;
			enemy[i][7] = ENEMY_TYPE_3_DEFAULT_HEALTH;
			*enemy_w = SPRITE_ENEMY_TYPE_3_W;
			*enemy_h = SPRITE_ENEMY_TYPE_3_H;
			enemy[i][3] = 2;
		}
		else {
			enemy[i][0] = ENEMY_TYPE_1;
			enemy[i][7] = ENEMY_TYPE_1_DEFAULT_HEALTH;
			*enemy_w = SPRITE_ENEMY_TYPE_1_W;
			*enemy_h = SPRITE_ENEMY_TYPE_1_H;
			enemy[i][3] = rand() % ENEMY_TYPE_1_VEL_MAX_DEVIATION + ENEMY_TYPE_1_VEL_AVG;
		}

		break;
	}
}

//  killEnemy(enemyArray, i): Kill an enemy with ID i

void killEnemy(int i) {
	if (enemy[i][6] == STATUS_EXISTS) {  //  Check if enemy with ID i is alive
		enemy[i][6] = STATUS_NONEXISTENT;  //  Set status to dead
		enemies_onscreen--;
		pe_distance = 600;  //  Reset player-enemy distance 
	}
}

//  playerEnemyCollision(i): Check for collision between player ship and enemy with ID i

int playerEnemyCollision(int i) {

	int x_collision, y_collision;

	x_collision = (enemy[i][1] + enemy[i][4] >= playership_x) && (enemy[i][1] <= playership_x + playership_w);
	y_collision = (enemy[i][2] + enemy[i][5] >= playership_y) && (enemy[i][2] <= playership_y + playership_h);

	if (x_collision && y_collision) return 1; else return 0;
}

//  playerUpgradeCollision(): Check for collision between player ship and upgrade item

int playerUpgradeCollision(void) {

	int x_collision, y_collision;

	x_collision = (upgrade_x + SPRITE_UPGRADE_W >= playership_x) && (upgrade_x <= playership_x + playership_w);
	y_collision = (upgrade_y + SPRITE_UPGRADE_H >= playership_y) && (upgrade_y <= playership_y + playership_h);

	if (x_collision && y_collision) return 1; else return 0;
}

//  bulletEnemyCollision(i): Check for collision between player bullet and enemy with ID i

int bulletEnemyCollision(int i) {

	for (int j = 0; j < PLAYER_BULLET_ARRAY_SIZE; j++) {
		if (player_bullet[j][3] == STATUS_NONEXISTENT) continue;

		int x_collision, y_collision;

		x_collision = (enemy[i][1] + enemy[i][4] >= player_bullet[j][0]) && (enemy[i][1] <= player_bullet[j][0] + SPRITE_BULLET_W);
		y_collision = (enemy[i][2] + enemy[i][5] >= player_bullet[j][1]) && (enemy[i][2] <= player_bullet[j][1] + SPRITE_BULLET_H);

		if (x_collision && y_collision) {
			player_bullet[j][3] = STATUS_NONEXISTENT;
			return 1;
			break;
		}
	}

	return 0;
}

//  playerBulletCollision(): Check for collision between player ship and enemy bullet

int playerBulletCollision(void) {
	if (playership_invisibility_counter == 0) {
		for (int j = 0; j < ENEMY_ARRAY_SIZE; j++) {
			if (enemy_bullet[j][3] == STATUS_NONEXISTENT) continue;

			int x_collision, y_collision;

			x_collision = (playership_x + playership_w >= enemy_bullet[j][0]) && (playership_x <= enemy_bullet[j][0] + SPRITE_BULLET_W);
			y_collision = (playership_y + playership_h >= enemy_bullet[j][1]) && (playership_y <= enemy_bullet[j][1] + SPRITE_BULLET_H);

			if (x_collision && y_collision) {
				enemy_bullet[j][3] = STATUS_NONEXISTENT;
				return 1;
				break;
			}
		}
	}

	return 0;
}

int helperBulletEnemyCollision(int i) {
	int x_collision, y_collision;

	x_collision = (enemy[i][1] + enemy[i][4] >= helper_bullet_x) && (enemy[i][1] <= helper_bullet_x + SPRITE_BULLET_W);
	y_collision = (enemy[i][2] + enemy[i][5] >= helper_bullet_y) && (enemy[i][2] <= helper_bullet_y + SPRITE_BULLET_H);

	if (x_collision && y_collision) return 1; else return 0;
}

void processEnemyMovement(void) {
	for (int i = 0; i < ENEMY_ARRAY_SIZE; i++) {
		if (enemy[i][6] == STATUS_NONEXISTENT) continue;  //  If enemy with ID i is dead, skip

		switch (enemy[i][0]) {
		case ENEMY_TYPE_1:
			processEnemyType1Movement(i);
			break;
		case ENEMY_TYPE_2:
			processEnemyType2Movement(i);
			break;
		case ENEMY_TYPE_3:
			processEnemyType3Movement(i);
			break;
		}

		//  If outside the game window, kill the enemy

		if (enemy[i][2] > WINDOW_HEIGHT) killEnemy(i);

		//  Collision detection with player ship

		if ((playerEnemyCollision(i)) && (playership_invisibility_counter == 0) && (!playership_is_dead)) {
			killEnemy(i);
			killPlayership();
		}

		if (bulletEnemyCollision(i)) {
			enemy[i][8] = 100;  //  Show health bar of enemy for some time
			enemy[i][7]--;
		}

		if (helperBulletEnemyCollision(i)) killEnemy(i);

		//  If health becomes 0, kill enemy

		if (enemy[i][7] <= 0) {
			Mix_Volume(2, 100);
			Mix_PlayChannel(2, snd_destroyed, 0);

			switch (enemy[i][0]) {
			case ENEMY_TYPE_1:
				game_score += SCORE_INCREASE_ENEMY_TYPE_1;
				break;
			case ENEMY_TYPE_2:
				game_score += SCORE_INCREASE_ENEMY_TYPE_2;
				break;
			case ENEMY_TYPE_3:
				game_score += SCORE_INCREASE_ENEMY_TYPE_3;
				break;
			}

			killEnemy(i);
		}

		//  Decrement counter for displaying health bar

		if (enemy[i][8] > 0) enemy[i][8]--;
	}
}

void processEnemyType1Movement(int i) {
	enemy[i][2] += enemy[i][3];
}

void processEnemyType2Movement(int i) {

	//  Type 2 enemy should move towards the player ship

	int dx = playership_x - enemy[i][1];
	int dy = playership_y - enemy[i][2];

	float angle_with_hor = atan((float)dy / dx);
	
	float angle = 0;

	if (angle_with_hor < 0) angle_with_hor *= -1;

	if ((dx >= 0) && (dy <= 0)) angle = angle_with_hor;  //  First quadrant
	if ((dx < 0) && (dy < 0)) angle = PI - angle_with_hor;  //  Second quadrant
	if ((dx < 0) && (dy > 0)) angle = PI + angle_with_hor;  //  Third quadrant
	if ((dx > 0) && (dy > 0)) angle = 2 * PI - angle_with_hor;  //  Fourth quadrant

	enemy[i][1] += enemy[i][3] * cos(angle);
	enemy[i][2] -= enemy[i][3] * sin(angle);

	enemy_type_2_additional_attributes[i][2] = angle;

	if (enemy_type_2_additional_attributes[i][1]) enemyFireBullet(i);

	if (!enemy_type_2_additional_attributes[i][1]) {
		enemy_type_2_additional_attributes[i][0]++;

		if (enemy_type_2_additional_attributes[i][0] >= ENEMY_TYPE_2_POSTATTACK_DELAY) {
			enemy_type_2_additional_attributes[i][0] = 0;
			enemy_type_2_additional_attributes[i][1] = 1;
		}
	}
}

void processEnemyType3Movement(int i) {

	//  Move downwards till the upper quarter of the screen and then stop

	if (enemy[i][2] < WINDOW_HEIGHT / 4) {
		enemy[i][2] += ENEMY_TYPE_3_INITIAL_Y_VEL;
	}
	else {
		if (enemy_type_3_additional_attributes[i][1]) {  //  If enemy is allowed to track, ...

			size_t relative_player_dir = (enemy[i][1] < playership_x) ? 0 : 1;  //  0 means enemy is left of playership, 1 means right
			
			float x_vel = ENEMY_TYPE_3_X_VEL;

			if (relative_player_dir == 1) x_vel *= -1;

			enemy[i][1] += (int)x_vel;

			//  If aligned with player ship, fire a bullet

			if ((relative_player_dir == 0) && (enemy[i][1] >= playership_x)) {
				enemyFireBullet(i);
			}
			else if ((relative_player_dir == 1) && (enemy[i][1] <= playership_x)) {
				enemyFireBullet(i);
			}
		}

		if (!enemy_type_3_additional_attributes[i][1]) {
			enemy_type_3_additional_attributes[i][0]++;

			if (enemy_type_3_additional_attributes[i][0] >= ENEMY_TYPE_3_POSTATTACK_DELAY) {
				enemy_type_3_additional_attributes[i][0] = 0;
				enemy_type_3_additional_attributes[i][1] = 1;
			}
		}
	}
}

void enemyFireBullet(int i) {
	for (int j = 0; j < PLAYER_BULLET_ARRAY_SIZE; j++) {
		if (enemy_bullet[j][3] == STATUS_NONEXISTENT) {

			Mix_Volume(3, 100);
			Mix_PlayChannel(3, snd_enemy_fire, 0);

			switch (enemy[i][0]) {
			case ENEMY_TYPE_2:
				if (enemy_type_2_additional_attributes[i][1] == 0) continue;
				enemy_bullet[j][3] = STATUS_EXISTS;
				enemy_bullet[j][0] = enemy[i][1] + enemy[i][4] / 2;
				enemy_bullet[j][1] = enemy[i][2] + enemy[i][5];
				enemy_bullet_angle_holder[j][1] = enemy_type_2_additional_attributes[i][2]; 
				enemy_type_2_additional_attributes[i][1] = 0;  //  Make type 2 enemy stop from following the player (temporarily)
				break;
			case ENEMY_TYPE_3:
				if (enemy_type_3_additional_attributes[i][1] == 0) continue;
				enemy_bullet[j][3] = STATUS_EXISTS;
				enemy_bullet[j][0] = enemy[i][1] + enemy[i][4] / 2;
				enemy_bullet[j][1] = enemy[i][2] + enemy[i][5];
				enemy_bullet_angle_holder[j][1] = 1.5 * PI;  //  Bullet should be directed vertically downwards
				enemy_type_3_additional_attributes[i][1] = 0;  //  Make type 3 enemy stop from following the player (temporarily)
				break;
			}

		}
	}
}

void enemyBulletMovement(void) {
	for (int i = 0; i < PLAYER_BULLET_ARRAY_SIZE; i++) {
		if (enemy_bullet[i][3] == STATUS_EXISTS) {  //  Check if bullet with ID i exists
			float x_vel = PLAYER_BULLET_SPEED / 2 * cos(enemy_bullet_angle_holder[i][1]);  //  x-component of velocity
			float y_vel = -PLAYER_BULLET_SPEED / 2 * sin(enemy_bullet_angle_holder[i][1]);  //  y-component of velocity

			enemy_bullet[i][0] += (int)x_vel;
			enemy_bullet[i][1] += (int)y_vel;

			if ((enemy_bullet[i][0] < 0) || (enemy_bullet[i][0] > WINDOW_WIDTH)) enemy_bullet[i][3] = STATUS_NONEXISTENT;
			if ((enemy_bullet[i][1] < 0) || (enemy_bullet[i][1] > WINDOW_HEIGHT)) enemy_bullet[i][3] = STATUS_NONEXISTENT;
		}
	}
}

//  initSDL_GraphicsLib(): Initialize SDL 2.0 Graphics and Image libraries

void initSDL_GraphicsLib(void) {

	//  Initialize SDL graphics library (SDL_Init() returns 1 upon failure to
	//  initialize SDL, otherwise returns 0 to communicate successful 
	//  initialization)

	int sdl_init_failed = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

	if (sdl_init_failed) {
		printf("ERROR! COULD NOT INITIALIZE SDL: %s\n", SDL_GetError());
		return 1;
	}

	//  Create a window

	window = SDL_CreateWindow(
		"Space Shooter (PROTOTYPE)",  //  Title of game window
		SDL_WINDOWPOS_CENTERED,  //  Center the x position of game window
		SDL_WINDOWPOS_CENTERED,  //  Center the y position of game window
		WINDOW_WIDTH,  //  Width of window in pixels
		WINDOW_HEIGHT,  //  Height of window in pixels
		0
	);

	//  If window cannot be created, return an error

	if (!window) {
		printf("ERROR! COULD NOT CREATE WINDOW: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	//  Create a renderer

	Uint32 render_flags = SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC;  //  Enable graphics hardware acceleration and VSYNC (VSYNC prevents screen-tearing)
	renderer = SDL_CreateRenderer(window, -1, render_flags);

	//  Return an error if renderer cannot be created

	if (!renderer) {
		printf("ERROR! COULD NOT CREATE RENDERER: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
}

//  loadSprites(): Load all sprites into the memory

void loadSprites(void) {

	//  Create a surface

	SDL_Surface *sprite_holder = NULL;

	//  Load player ship sprite to the surface

	sprite_holder = IMG_Load("resources/sprites/spr_playership.png");

	//  Return an error upon failure to load sprite

	if (!sprite_holder) {
		printf("ERROR! %s\n\n", SDL_GetError());
	}

	//  Create a texture from the existing surface (to transfer sprite to graphics hardware)

	tex_playership = SDL_CreateTextureFromSurface(renderer, sprite_holder);

	//  Clear surface

	SDL_FreeSurface(sprite_holder);

	//  Load player ship bullet sprite

	sprite_holder = IMG_Load("resources/sprites/spr_playerbullet_std.png");

	if (!sprite_holder) {
		printf("ERROR! %s\n\n", SDL_GetError());
	}

	tex_player_bullet = SDL_CreateTextureFromSurface(renderer, sprite_holder);

	SDL_FreeSurface(sprite_holder);

	//  Load player ship bullet sprite

	sprite_holder = IMG_Load("resources/sprites/spr_playerbullet_upgraded.png");

	if (!sprite_holder) {
		printf("ERROR! %s\n\n", SDL_GetError());
	}

	tex_player_bullet_upgraded = SDL_CreateTextureFromSurface(renderer, sprite_holder);

	SDL_FreeSurface(sprite_holder);

	//  Load enemy type 1 sprite

	sprite_holder = IMG_Load("resources/sprites/spr_enemyship.png");

	if (!sprite_holder) {
		printf("ERROR! %s\n\n", SDL_GetError());
	}

	tex_enemyship = SDL_CreateTextureFromSurface(renderer, sprite_holder);

	SDL_FreeSurface(sprite_holder);

	//  Load enemy type 2 sprite

	sprite_holder = IMG_Load("resources/sprites/spr_enemy_type2.png");

	if (!sprite_holder) {
		printf("ERROR! %s\n\n", SDL_GetError());
	}

	tex_enemyship_type2 = SDL_CreateTextureFromSurface(renderer, sprite_holder);

	SDL_FreeSurface(sprite_holder);

	//  Load enemy type 3 sprite

	sprite_holder = IMG_Load("resources/sprites/spr_enemy_type3.png");

	if (!sprite_holder) {
		printf("ERROR! %s\n\n", SDL_GetError());
	}

	tex_enemyship_type3 = SDL_CreateTextureFromSurface(renderer, sprite_holder);

	SDL_FreeSurface(sprite_holder);

	//  Load enemy type 2 bullet sprite

	sprite_holder = IMG_Load("resources/sprites/spr_enemybullet_type2.png");

	if (!sprite_holder) {
		printf("ERROR! %s\n\n", SDL_GetError());
	}

	tex_enemyship_type2_bullet = SDL_CreateTextureFromSurface(renderer, sprite_holder);

	SDL_FreeSurface(sprite_holder);

	//  Load enemy type 3 bullet sprite

	sprite_holder = IMG_Load("resources/sprites/spr_enemybullet_type3.png");

	if (!sprite_holder) {
		printf("ERROR! %s\n\n", SDL_GetError());
	}

	tex_enemyship_type3_bullet = SDL_CreateTextureFromSurface(renderer, sprite_holder);

	SDL_FreeSurface(sprite_holder);

	//  Load attack upgrade item sprite

	sprite_holder = IMG_Load("resources/sprites/spr_upgrade_attack.png");

	if (!sprite_holder) {
		printf("ERROR! %s\n\n", SDL_GetError());
	}

	tex_upgrade_attack = SDL_CreateTextureFromSurface(renderer, sprite_holder);

	SDL_FreeSurface(sprite_holder);

	//  Load helper upgrade item sprite

	sprite_holder = IMG_Load("resources/sprites/spr_upgrade_helper.png");

	if (!sprite_holder) {
		printf("ERROR! %s\n\n", SDL_GetError());
	}

	tex_upgrade_helper = SDL_CreateTextureFromSurface(renderer, sprite_holder);

	SDL_FreeSurface(sprite_holder);

	//  Load helper sprite

	sprite_holder = IMG_Load("resources/sprites/spr_helper.png");

	if (!sprite_holder) {
		printf("ERROR! %s\n\n", SDL_GetError());
	}

	tex_helper = SDL_CreateTextureFromSurface(renderer, sprite_holder);

	SDL_FreeSurface(sprite_holder);
}

//  initSDL_TTFLib(): Initialize SDL truetype font library

void initSDL_TTFLib(void) {

	//  Initialize font library

	int ttf_init_failed = TTF_Init();

	if (ttf_init_failed) {
		printf("ERROR! COULD NOT INITIALIZE SDL TTF LIBRARY");
		return 1;
	}

	//  Load fonts

	main_font = TTF_OpenFont("resources/font.ttf", MAIN_FONT_SIZE);
	title_font = TTF_OpenFont("resources/font.ttf", TITLE_FONT_SIZE);
	titlescreen_options_font = TTF_OpenFont("resources/font.ttf", TITLESCREEN_SELECTIONS_FONT_SIZE);
}

//  initSDL_MixerLib(): Initialize SDL Mixer library (for playing music
//                      files)

void initSDL_MixerLib(void) {

	//  Initialize SDL Mixer library

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		printf("Could not load music library! ERROR: %s\n", Mix_GetError());
	}

	//  Load titlescreen background music

	titlescreen_bgm = Mix_LoadMUS("resources/sound/bgm_titlescreen.mp3");

	snd_playership_bullet_std = Mix_LoadWAV("resources/sound/snd_playership_bullet_std_fire.wav");
	snd_select = Mix_LoadWAV("resources/sound/snd_select.wav");
	snd_destroyed = Mix_LoadWAV("resources/sound/snd_destroyed.mp3");
	snd_enemy_fire = Mix_LoadWAV("resources/sound/snd_enemy_fire.mp3");

	//if ((titlescreen_bgm == NULL) || (snd_playership_bullet_std == NULL)) {
	//	printf("Could not load music file! ERROR: %s\n", Mix_GetError());
	//}

	//  Play the background music indefinitely
	//  Second argument is the number of times to loop the music (-1 means infinite times)

	Mix_PlayMusic(titlescreen_bgm, -1);  
}

//  drawGUI(): Draw graphical user interface

void drawGUI(void) {
	char score_text[100] = "";
	char time_text[30] = "";
	char maxlives_text[10] = "";

	sprintf(score_text, "Score: %d", game_score);
	sprintf(time_text, "Time: %d s", (int)time_elapsed_game);
	sprintf(maxlives_text, "Lives: %d", playership_lives);

	SDL_Color color = { 255, 255, 255 };

	SDL_Surface *message = TTF_RenderText_Blended_Wrapped(main_font, score_text, color, 300);
	SDL_Texture *message_tex = SDL_CreateTextureFromSurface(renderer, message);
	SDL_Rect text_container = { 20, 20, message->w, message->h };
	SDL_RenderCopy(renderer, message_tex, NULL, &text_container);

	SDL_Surface *message2 = TTF_RenderText_Blended_Wrapped(main_font, maxlives_text, color, 300);
	SDL_Texture *message2_tex = SDL_CreateTextureFromSurface(renderer, message2);
	SDL_Rect text_container2 = { 20, 20 + message->h + 5, message2->w, message2->h };
	SDL_RenderCopy(renderer, message2_tex, NULL, &text_container2);

	SDL_Surface *message3 = TTF_RenderText_Blended_Wrapped(main_font, time_text, color, 300);
	SDL_Texture *message3_tex = SDL_CreateTextureFromSurface(renderer, message3);
	SDL_Rect text_container3 = { 20, 20 + message->h + message2->h + 5 + 5, message3->w, message3->h };
	SDL_RenderCopy(renderer, message3_tex, NULL, &text_container3);

	SDL_FreeSurface(message);
}

//  renderGraphics(): Process and display images in the game window

void renderGraphics(void) {

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);  //  Set the background to black

	SDL_RenderClear(renderer);  //  Remove the previous frame 

	//  Draw enemy sprites

	for (int i = 0; i < ENEMY_ARRAY_SIZE; i++) {
		if (enemy[i][6] == STATUS_NONEXISTENT) continue;
		SDL_Rect enemyRect = { enemy[i][1], enemy[i][2], enemy[i][4], enemy[i][5] };
		
		switch (enemy[i][0]) {
		case ENEMY_TYPE_1:
			SDL_RenderCopy(renderer, tex_enemyship, NULL, &enemyRect);
			break;

		case ENEMY_TYPE_2:
			SDL_RenderCopyEx(renderer, tex_enemyship_type2, NULL, &enemyRect, 90 - radToDegree(enemy_type_2_additional_attributes[i][2]), NULL, SDL_FLIP_NONE);
			break;

		case ENEMY_TYPE_3:
			SDL_RenderCopy(renderer, tex_enemyship_type3, NULL, &enemyRect);
			break;
		}

		if (enemy[i][8] > 0) {

			int healthbar_w = 30;
			int healthbar_h = 3;

			SDL_Rect enemyHealthBarBG = {enemy[i][1] + enemy[i][4], enemy[i][2], healthbar_w, healthbar_h};
			SDL_SetRenderDrawColor(renderer, 63, 63, 63, 1);
			SDL_RenderFillRect(renderer, &enemyHealthBarBG);

			SDL_Rect enemyCurrentHealth = { enemy[i][1] + enemy[i][4], enemy[i][2], healthbar_w * ((float)enemy[i][7] / ENEMY_TYPE_1_DEFAULT_HEALTH), healthbar_h };
			SDL_SetRenderDrawColor(renderer, 196, 23, 23, 1);
			SDL_RenderFillRect(renderer, &enemyCurrentHealth);
		}
	}

	//  Create a rectangle for the playership that will contain the playership sprite and act as a mask for collisions

	if ((!playership_is_dead) && (playership_invisibility_counter % 10 == 0)) {
		SDL_Rect playerRect = { playership_x, playership_y, playership_w, playership_h };
		SDL_RenderCopyEx(renderer, tex_playership, NULL, &playerRect, 90 - radToDegree(playership_angle), NULL, SDL_FLIP_NONE);
	}

	//  Draw bullet sprites

	for (int i = 0; i < PLAYER_BULLET_ARRAY_SIZE; i++) {
		if (player_bullet[i][3] == STATUS_NONEXISTENT) continue;

		SDL_Rect playerBulletRect = { player_bullet[i][0], player_bullet[i][1], SPRITE_BULLET_W, SPRITE_BULLET_H };

		if (current_upgrade != UPGRADE_TYPE_ATTACK) {
			SDL_RenderCopyEx(renderer, tex_player_bullet, NULL, &playerBulletRect, 90 - radToDegree(player_bullet_angle_holder[i][1]), NULL, SDL_FLIP_NONE);
		}
		else {
			SDL_RenderCopyEx(renderer, tex_player_bullet_upgraded, NULL, &playerBulletRect, 90 - radToDegree(player_bullet_angle_holder[i][1]), NULL, SDL_FLIP_NONE);
		}
	}

	//  Draw enemy bullet sprites

	if (!difficulty_level == DIFFICULTY_LV1) {  //  No need to run this for loop at difficulty level 1
		for (int i = 0; i < PLAYER_BULLET_ARRAY_SIZE; i++) {
			if (enemy_bullet[i][3] == STATUS_NONEXISTENT) continue;

			SDL_Rect enemyBulletRect = { enemy_bullet[i][0], enemy_bullet[i][1], SPRITE_BULLET_W, SPRITE_BULLET_H };
			SDL_RenderCopyEx(renderer, tex_enemyship_type3_bullet, NULL, &enemyBulletRect, 90 - radToDegree(enemy_bullet_angle_holder[i][1]), NULL, SDL_FLIP_NONE);
		}
	}

	//  Draw upgrade sprites

	if (upgrade_exists) {

		SDL_Rect upgradeRect = { upgrade_x, upgrade_y, SPRITE_UPGRADE_W, SPRITE_UPGRADE_H };

		switch (upgrade_type) {
		case UPGRADE_TYPE_ATTACK:
			SDL_RenderCopy(renderer, tex_upgrade_attack, NULL, &upgradeRect);
			break;

		case UPGRADE_TYPE_HELPER:
			SDL_RenderCopy(renderer, tex_upgrade_helper, NULL, &upgradeRect);
			break;
		}
	}

	//  Draw upgrade bar when upgrade is active

	if (current_upgrade != UPGRADE_NONE) {
		int upgradebar_w = 200;
		int upgradebar_h = 10;

		SDL_Rect upgradeBarBG = { (WINDOW_WIDTH - upgradebar_w) / 2, WINDOW_HEIGHT - 50, upgradebar_w, upgradebar_h };
		SDL_SetRenderDrawColor(renderer, 63, 63, 63, 1);
		SDL_RenderFillRect(renderer, &upgradeBarBG);

		SDL_Rect upgradeBar = { (WINDOW_WIDTH - upgradebar_w) / 2, WINDOW_HEIGHT - 50, upgradebar_w * ((float)upgrade_time_left / UPGRADE_DURATION), upgradebar_h};
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
		SDL_RenderFillRect(renderer, &upgradeBar);
	}

	//  Draw helper sprite

	if (current_upgrade == UPGRADE_TYPE_HELPER) {
		SDL_Rect helperRect = {helper_x, helper_y, SPRITE_HELPER_W, SPRITE_HELPER_H};
		SDL_RenderCopy(renderer, tex_helper, NULL, &helperRect);
	}

	//  Draw helper bullet

	if ((helper_bullet_exists) && (current_upgrade = UPGRADE_TYPE_HELPER)) {
		SDL_Rect helperBulletRect = { helper_bullet_x, helper_bullet_y, SPRITE_BULLET_W, SPRITE_BULLET_H };
		SDL_RenderCopyEx(renderer, tex_player_bullet, NULL, &helperBulletRect, 90 - radToDegree(helper_bullet_angle), NULL, SDL_FLIP_NONE);
	}

	//  Draw graphical user interface

	drawGUI();
		
	char text[400] = "";

	sprintf(text, "Press ESC to pause the game");

	drawText(text, 255, 255, 255, 10, WINDOW_HEIGHT - 30);

	//  Show everything that has been processed on-screen

	SDL_RenderPresent(renderer);

	//  SDL_Delay(t) takes the number of milliseconds t as input
	//  This sets the fps (frames per second) / refresh rate of the game to the default value 60
	//  By setting t to 1000 / FPS, we can force the game to run at our desired fps
	//  For instance, if FPS = 60, 1000 / 60 = 16.6667 milliseconds which means that each frame 
	//  stays on the screen for 16.6667 milliseconds, resulting in 16.66667 * 1000 = 60 frames
	//  per second

	SDL_Delay(1000 / FPS);
}

//  clearMemory(): Unload SDL libraries before quiting the game

void clearMemory(void) {
	SDL_DestroyTexture(tex_playership);
	SDL_DestroyTexture(tex_enemyship);
	SDL_DestroyTexture(tex_player_bullet);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	SDL_Quit();
}

//  drawText(text, r, g, b, text_x, text_y): Draw a line of text from the string 'text' with
//                                           color {r, g, b} at (text_x, text_y) position

void drawText(char text[], int r, int g, int b, int text_x, int text_y) {
	SDL_Color color = { r, g, b };

	SDL_Surface *message = TTF_RenderText_Blended_Wrapped(main_font, text, color, 300);

	SDL_Texture *message_tex = SDL_CreateTextureFromSurface(renderer, message);

	SDL_Rect text_container = { text_x, text_y, message->w, message->h };

	SDL_FreeSurface(message);

	SDL_RenderCopy(renderer, message_tex, NULL, &text_container);
}

//  processUpgrades(): Makes the upgrade items appear randomly from the top portion of the screen
//                     and activates an upgrade if the item collides with the player ship

void processUpgrades(void) {

	static int created;
	static int timer;
	int max_count = 300;

	if (timer == max_count) {
		created = 1;
		timer = 0;
	}

	if ((upgrade_exists == 0) && (current_upgrade == UPGRADE_NONE)) timer++;

	if (created == 1) {
		upgrade_exists = 1;

		int type = rand() % 2;

		if (type == 0) {
			upgrade_type = UPGRADE_TYPE_ATTACK;
		}
		else {
			upgrade_type = UPGRADE_TYPE_HELPER;
		}

		upgrade_x = rand() % (WINDOW_WIDTH - 60);
		upgrade_y = -20;

		created = 0;
	}

	if (upgrade_exists == 1) {
		upgrade_y += 2;

		if (upgrade_y > WINDOW_HEIGHT) {
			upgrade_exists = 0;
			created = 0;
			timer = 0;
		}
	}

	if (playerUpgradeCollision()) {
		upgrade_exists = 0;
		created = 0;
		upgrade_time_left = UPGRADE_DURATION;

		switch (upgrade_type) {
		case UPGRADE_TYPE_ATTACK:
			current_upgrade = UPGRADE_TYPE_ATTACK;
			break;
		case UPGRADE_TYPE_HELPER:
			current_upgrade = UPGRADE_TYPE_HELPER;
		}
	}

	if (upgrade_time_left > 0) {
		upgrade_time_left--;
	}
	else {
		current_upgrade = UPGRADE_NONE;
	}
}

//  processHelperUpgrade(): Code for the helper upgrade

void processHelperUpgrade(void) {

	if (current_upgrade == UPGRADE_TYPE_HELPER) {
		static float theta;

		theta += HELPER_SPEED;

		if (theta >= 2 * PI) theta = 0;

		helper_x = helper_radius * cos(theta) + playership_x + playership_w / 2;
		helper_y = helper_radius * sin(theta) + playership_y + playership_h / 2;

		//  Create helper bullet

		if (!helper_bullet_exists) {
			helper_bullet_x = helper_x + SPRITE_HELPER_W / 2;
			helper_bullet_y = helper_y + SPRITE_HELPER_H / 2;
			helper_bullet_angle = playership_angle;
			helper_bullet_exists = 1;
		}

		//  Helper bullet movement

		int dx = HELPER_BULLET_SPEED * cos(helper_bullet_angle);
		int dy = HELPER_BULLET_SPEED * sin(helper_bullet_angle);

		helper_bullet_x += dx;
		helper_bullet_y -= dy;

		if ((helper_bullet_x > WINDOW_WIDTH) || (helper_bullet_x < 0)) {
			helper_bullet_exists = 0;
		}

		if ((helper_bullet_y > WINDOW_HEIGHT) || (helper_bullet_y < 0)) {
			helper_bullet_exists = 0;
		}
	}
}

//  radToDegree(x): Converts input x from radians to degrees and returns the value

float radToDegree(float x) {
	return x * ((float)180.00000 / PI);
}

//  randomRange(a, b): Returns a random value in the range a to b (inclusive)
//                     Also works for negative integer combinations e.g -1 to 9, 
//                     -3 to -11...

int randomRange(int a, int b) {

	if (a >= b) {
		printf("ERROR: INVALID RANGE OF random_range FUNCTION\n");
		return 0;
	}
	else {
		if ((a >= 0) && (b >= 0)) {
			return rand() % (b - a + 1) + a;
		}
		else if ((a < 0) && (b > 0)) {
			return rand() % (b - a + 1) + a;
		}
		else if ((a < 0) && (b < 0)) {
			a *= -1;
			b *= -1;
			return (-1) * ((rand() % (a - b + 1) + b));
		}
	}
}

//  ***  MAIN FUNCTION  ***  //

int main(void) {

	//  Randomize rand() function with system time as seed

	srand(time(NULL));

	//  Initialize all global variables

	initVariables();

	//  Initialize all SDL libraries

	initSDL_GraphicsLib();
	initSDL_TTFLib();
	initSDL_MixerLib();

	//  Load all sprites (images)

	loadSprites();

	//  Game loop

	while (!game_quit) {
		
		processControls();  //  Get user keyboard and mouse inputs

		switch (game_stage) {

		case STAGE_ABOUT:
			aboutScreen();
			break;

		case STAGE_TITLESCREEN:
			titleScreen();
			break;

		case STAGE_GAME:
			time_elapsed_game = (SDL_GetTicks() - time_elapsed_beforegame) / 1000;

			processDifficulty();
			processPlayershipMovement();
			processPlayershipRotation();
			playerBulletMovement();
			enemyBulletMovement();
			playershipFireBullets();
			spawnEnemies();
			processEnemyMovement();
			processUpgrades();
			renderGraphics();
			processHelperUpgrade();

			if (playership_lives == 0) {
				game_stage = STAGE_GAMEOVER;
			}
			break;

		case STAGE_GAMEOVER:
			gameOver();
			break;

		case STAGE_PAUSESCREEN:
			pauseGame();
			break;
		}
	}

	//  Clear memory before closing the game

	clearMemory();

	return 0;
}