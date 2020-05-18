#pragma once

#include <stdbool.h>

#include "board.h"
#include "item.h"
#include "ghost.h"
#include "pacman.h"
#include "pellet.h"
#include "text.h"
#include "missile.h"

#define SCREEN_W 448
#define SCREEN_H 640

//Holds data relating to a single rendered ghost-row.
typedef struct
{
	GhostType  type;
	const char *description;
	const char *name;
	TextColor color;
} GhostDisplayRow;

//Class handles the drawing of all entities in the game

//
// Vanity screen renderering
//

void draw_vanity_charnickname(void);
void draw_vanity_charnickname_resize(int resize_w, int resize_h);
void draw_vanity_text(const char *text, int x, int y);
void draw_vanity_text_resize(const char *text, int resize_w, int resize_h, int x, int y);
void draw_vanity_ghostline(GhostDisplayRow *row, int y, bool description, bool name);
void draw_vanity_ghostline_resize(GhostDisplayRow *row, int resize_w, int resize_h, int y, bool description, bool name);
void draw_vanity_corporate_info(void);
void draw_vanity_corporate_info_resize(int resize_w, int resize_h);
//void draw_vanity_corporate_info_resize(void);
void draw_vanity_pellet_info(bool flashing);
void draw_vanity_pellet_info_resize(bool flashing, int resize_w, int resize_h);
void draw_vanity_animation(int dt);

//
// Instructions screen renderering
//

void draw_instrc_info(void);
void draw_instrc_info_resize(int resize_w, int resize_h);
void draw_instrc_corporate_info(void);
void draw_instrc_corporate_info_resize(int resize_w, int resize_h);

//
// Common menu rendering
//

void draw_common_oneup(bool flashing, int score);
void draw_common_twoup(bool flashing, int score);
void draw_common_highscore(int highscore);
void draw_common_oneup_resize(bool flashing, int score, int resize_w, int resize_h);
void draw_common_twoup_resize(bool flashing, int score, int resize_w, int resize_h);
void draw_common_highscore_resize(int highscore, int resize_w, int resize_h);

void draw_stage(int curLvl);
void draw_stage_resize(int curLvl, int resize_w, int resize_h);

void draw_common_indicator(int mode, int x, int y);
void draw_common_indicator_resize(int mode, int resize_w, int resize_h, int x, int y);

void draw_credits(int numCredits);
void draw_credits(int numCredits, int resize_w, int resize_h);

//
// Game-only word rendering
//

void draw_game_playerone_start(void);
void draw_game_playertwo_start(void);
void draw_game_ready(void);
void draw_game_gameover(void);
void draw_game_playerone_start_resize(int resize_w, int resize_h);
void draw_game_playertwo_start_resize(int resize_w, int resize_h);
void draw_game_ready_resize(int resize_w, int resize_h);
void draw_game_gameover_resize(int resize_w, int resize_h);

//
// Fruit renderering
//

void draw_item_indicators(int currentLevel);
void draw_item_game(int currentLevel, GameItem *gameItem);
void draw_item_indicators_resize(int currentLevel, int resize_w, int resize_h);
void draw_item_game_resize(int currentLevel, GameItem *gameItem, int resize_w, int resize_h);

//
// Pellet renderering
//

void draw_pellet_small(Pellet *p);
void draw_pellet_power(Pellet *p, bool flashing);
void draw_pellet_small_resize(Pellet *p, int resize_w, int resize_h);
void draw_pellet_power_resize(Pellet *p, bool flashing, int resize_w, int resize_h);

void draw_small_pellets(PelletHolder *ph);
void draw_large_pellets(PelletHolder *ph, bool flashing);
void draw_small_pellets_resize(PelletHolder *ph, int resize_w, int resize_h);
void draw_large_pellets_resize(PelletHolder *ph, bool flashing, int resize_w, int resize_h);

//
// Pacman renderering
//

void draw_pacman(Pacman *pacman);
void draw_pacman_static(Pacman *pacman);
void draw_pacman_death(Pacman *pacman, unsigned int timeSinceDeath);
void draw_pacman_lives(int numLives);
void draw_pacman_resize(Pacman *pacman, int resize_w, int resize_h);
void draw_pacman_static_resize(Pacman *pacman, int resize_w, int resize_h);
void draw_pacman_death_resize(Pacman *pacman, unsigned int timeSinceDeath, int resize_w, int resize_h);
void draw_pacman_lives_resize(int numLives, int resize_w, int resize_h);

//
// Pacman2 renderering
//

void draw_pacman2(Pacman *pacman);
void draw_pacman2_static(Pacman *pacman);
void draw_pacman2_death(Pacman *pacman, unsigned int timeSinceDeath);
void draw_pacman2_lives(int numLives);
void draw_pacman2_resize(Pacman *pacman, int resize_w, int resize_h);
void draw_pacman2_static_resize(Pacman *pacman, int resize_w, int resize_h);
void draw_pacman2_death_resize(Pacman *pacman, unsigned int timeSinceDeath, int resize_w, int resize_h);
void draw_pacman2_lives_resize(int numLives, int resize_w, int resize_h);


//
// Ghost rendering
//

void draw_ghost(Ghost *ghost);
bool draw_scared_ghost(Ghost *ghost, unsigned int timeGhostScared);
void draw_eyes(Ghost *ghost);
void draw_ghost_resize(Ghost *ghost, int resize_w, int resize_h);
bool draw_scared_ghos_resize(Ghost *ghost, unsigned int timeGhostScared, int resize_w, intr resize_h);
void draw_eyes_resize(Ghost *ghost, int resize_w, int resize_h);


void draw_missile(Missile *missile);
void draw_missile_resize(Missile *missile, int resize_w, int resize_h);
//
// Points rendering
//

void draw_item_pts(GameItem *gameItem);
void draw_item_pts_resize(GameItem *gameItem, int resize_w, int resize_h);
//
// Board rendering
//

void draw_board(Board *board);
void draw_board_flash(Board *board);
void draw_board_resize(Board *board, int resize_w, int resize_h);
void draw_board_flash_resize(Board *board, int resize_w, int resize_h);
