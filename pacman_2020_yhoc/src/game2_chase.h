#pragma once

#include "main.h"
#include "board.h"
#include "item.h"
#include "pacman.h"
#include "pellet.h"
#include "missile.h"

#define BOSS_STAGE 4

typedef enum
{
	GameBeginState2,
	LevelBeginState2,
	GamePlayState2,
	WinState2,
	DeathState2,
	GameoverState2,
	ClearState2
} GameState2;

typedef enum
{
	One1,
	Two2
} Player2;

//what you do in current state

//what causes you to transition from state A -> state B

//elevator stationary
	//action: do nothing
	//transition
		//-> moving up (someone presses a button upstairs)
		//-> moving down (someone presses a button downstairs)
		//-> door opening (someone presses button on current level)
		//-> go down/ up (timeout on a specific level)

typedef struct
{
	GameState2 gameState2;
	unsigned int ticksSinceModeChange;
	Pacman pacman;
	Pacman pacman_enemy;
	Missile missiles[2];
	Board board[STAGE_COUNT];
	PelletHolder pelletHolder[STAGE_COUNT];
	int highscore;
	int currentLevel;
	int stageLevel;
	ModeState mode;
	RemoteRole role;
	int tick;
	Player2 death_player;
} PacmanGame2;

typedef struct
{
	int tick;
	Player2 death_player;
	GameState2 gameState;
	unsigned int ticksSinceModeChange;
	int highscore;
	int currentLevel;
	int stageLevel;
	ModeState mode;
	Pacman pacman;
	Pacman pacman_enemy;
	PelletHolder pelletHolder;
	
} PacmanGame_socket2;

//Updates the game 1 tick, or 1/60th of a second.
void game_tick2(PacmanGame2 *game);

//Renders the game in its current state.
void game_render2(PacmanGame2 *game, int tick);

//Returns true if the game is finished and is ready to hand back to the menu system.
bool is_game_over2(PacmanGame2 *game, int tick);

//Call this at start of level 1 to initialize all entities and game objects.
void gamestart_init2(PacmanGame2 *game, int mode);

//Call this at the start of each level to reinitialize all entities to a default state.
void level_init2(PacmanGame2 *game);

//Call after pacman dies to reinitialize entities to continue the same level.
void pacdeath_init2(PacmanGame2 *game);

//Returns the length of the given integer.
int int_length2(int num);
