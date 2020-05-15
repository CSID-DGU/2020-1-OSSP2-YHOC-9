#include "game2_chase.h"
#include "animation.h"
#include "board.h"
#include "fps.h"
#include "input.h"
#include "pacman.h"
#include "pellet.h"
#include "physics.h"
#include "renderer.h"
#include "sound.h"
#include "text.h"
#include "window.h"
#include "highscore.h"
#include <stdlib.h>
#include <time.h>



static void process_player2(Pacman *pacman, Board *board, Player2 Player2);
//static void process_fruit(PacmanGame2 *game);
static void process_pellets2(PacmanGame2 *game);
static void process_missiles2(PacmanGame2 *game);
static bool check_pacghost_collision2(PacmanGame2 *game);     //return true if pacman collided with any ghosts
static bool check_ghomissile_collision2(PacmanGame2 *game);
static void enter_state2(PacmanGame2 *game, GameState2 state); //transitions to/ from a state
static bool resolve_telesquare2(PhysicsBody *body);          //wraps the body around if they've gone tele square

static Player2 death_player;

void game_tick2(PacmanGame2 *game)
{
	Pacman *pac = &game->pacman;
	Pacman *pac2 = &game->pacman_enemy;

	// printf("life: %d / %d\n",game->pacman.livesLeft,game->pacman_enemy.livesLeft);

	unsigned dt = ticks_game() - game->ticksSinceModeChange;

	switch (game->gameState2)
	{
		case GameBeginState2:
			// plays the sound, has the "Player2 1" image, has the "READY!" thing

			break;
		case LevelBeginState2:
			// similar to game begin mode except no sound, no "Player2 1", and slightly shorter duration

			break;
		case GamePlayState2:
			// everyone can move and this is the standard 'play' game mode
	
			if(game->pacman.livesLeft != -1) process_player2(&game->pacman, &game->board[game->stageLevel], One1);
			if(game->mode != SoloState && game->pacman_enemy.livesLeft != -1) process_player2(&game->pacman_enemy, &game->board[game->stageLevel], Two2);
			
			//process_player2(&game->pacman, &game->board, One1);
			//if(game->mode != SoloState) process_player2(&game->pacman_enemy, &game->board, Two2);
			
	
			if(game->pacman.missile == 1) process_missiles2(game);
			if(game->mode != SoloState && game->pacman_enemy.missile == 1) process_missiles2(game);
			
			process_pellets2(game);
			
			if (game->pacman.score > game->highscore) game->highscore = game->pacman.score;
			if (game->mode != SoloState && (game->pacman_enemy.score > game->highscore) ) game->highscore = game->pacman_enemy.score;

			break;
		case WinState2:
			//pacman eats last pellet, immediately becomes full circle
			//everything stays still for a second or so
			//monsters + pen gate disappear
			//level flashes between normal color and white 4 times
			//screen turns dark (pacman still appears to be there for a second before disappearing)
			//full normal map appears again
			//pellets + ghosts + pacman appear again
			//go to start level mode

			break;
		case DeathState2:
			// pacman has been eaten by a ghost and everything stops moving
			// he then does death animation and game starts again

			//everything stops for 1ish second

			//ghosts disappear
			//death animation starts
			//empty screen for half a second

			break;
		case GameoverState2:
			// pacman has lost all his lives
			//it displays "game over" briefly, then goes back to main menu
			break;
		case ClearState2:

			break;
	}

	//p
	// State Transitions - refer to gameflow for descriptions
	//

	//bool allPelletsEaten = game->pelletHolder[game->stageLevel].numLeft == 0;
	//bool collidedWithGhost = check_pacghost_collision2(game);
	bool collidedWithMissile = check_ghomissile_collision2(game);
	
	int lives = game->pacman.livesLeft;
	int player2_lives = -1;
	if(game->mode != SoloState) player2_lives = game->pacman_enemy.livesLeft;
	
	switch (game->gameState2)
	{
		case GameBeginState2:
			if (dt > 2200) enter_state2(game, LevelBeginState2);

			break;
		case LevelBeginState2:
			if (dt > 1800) enter_state2(game, GamePlayState2);
			game->pacman.godMode = false;
			if(game->mode != SoloState) {
				game->pacman_enemy.godMode = false;
				enter_state2(game, GamePlayState2);
			}
			break;
		case GamePlayState2:

			//TODO: remove this hacks
			if (key_held(SDLK_k)) enter_state2(game, DeathState2);
			//else if (allPelletsEaten) enter_state2(game, WinState2);
			//else if (collidedWithGhost) enter_state2(game, DeathState2);
			break;
		case WinState2:
			//if (transitionLevel) //do transition here
			if (dt > 4000){
				 enter_state2(game, LevelBeginState2);
				 if(game->currentLevel == 6) enter_state2(game,ClearState2);
			}
			break;
		case DeathState2:
			if (dt > 4000)
			{
				if(game->mode == SoloState && lives == 0) enter_state2(game, GameoverState2);
				else if(game->mode != SoloState && ((lives == -1 && player2_lives == 0) || (lives == 0 && player2_lives == -1)) ) 
				{ printf("ok\n"); enter_state2(game, GameoverState2); }
				//if (lives == 0 && death_player == One1) enter_state(game, GameoverState2);
				//else if (player2_lives == 0 && death_player == Two2) enter_state(game, GameoverState2);
				else enter_state2(game, LevelBeginState2);
			}

			break;
		case GameoverState2:
			if (dt > 2000)
			{
				//TODO: go back to main menu

			}
			break;
		case ClearState2:
			if(dt > 2000)
			{
			}
			break;
	}
}

void game_render2(PacmanGame2 *game, int tick)
{
	unsigned dt = tick - game->ticksSinceModeChange;
	static unsigned godDt = 0;
	static bool godChange = false;
	Pacman *pac = &game->pacman;
	Pacman *pac2 = &game->pacman_enemy;
	//common stuff that is rendered in every mode:
	// 1up + score, highscore, base board, lives, small pellets, fruit indicators
	draw_common_oneup(true, game->pacman.score);
	if(game->mode != SoloState) draw_common_twoup(true, game->pacman_enemy.score);
	
	draw_common_highscore(game->highscore);
	
	draw_pacman_lives(game->pacman.livesLeft);
	if(game->mode != SoloState) draw_pacman2_lives(game->pacman_enemy.livesLeft);
	
	draw_small_pellets(&game->pelletHolder[game->stageLevel]);

	//in gameover state big pellets don't render
	//in gamebegin + levelbegin big pellets don't flash
	//in all other states they flash at normal rate
	switch (game->gameState2)
	{
		case GameBeginState2:
			draw_game_playerone_start();
			draw_game_ready();

			draw_large_pellets(&game->pelletHolder[game->stageLevel], false);
			draw_board(&game->board[game->stageLevel]);
			break;
		case LevelBeginState2:
			draw_game_ready();

			//we also draw pacman and ghosts (they are idle currently though)
			
			if(game->pacman.livesLeft != -1) draw_pacman_static(&game->pacman);
			if(game->mode != SoloState && game->pacman_enemy.livesLeft != -1) draw_pacman2_static(&game->pacman_enemy);
			
			//draw_pacman_static(&game->pacman);
			//if(game->mode != SoloState) draw_pacman2_static(&game->pacman_enemy);
			
			draw_large_pellets(&game->pelletHolder[game->stageLevel], false);
			draw_board(&game->board[game->stageLevel]);
			break;
		case GamePlayState2:
			//stage 표시
			draw_stage(game->currentLevel);
			draw_large_pellets(&game->pelletHolder[game->stageLevel], true);
			draw_board(&game->board[game->stageLevel]);

			if(game->pacman.livesLeft != -1) draw_pacman(&game->pacman);
			
			//draw_pacman(&game->pacman);

			if(game->pacman.godMode == false) {

				if(pac->missile == 1)	
					for (int i = 0; i < 2; i++) draw_missile(&game->missiles[i]);
			}

			else {
				if(godChange == false) {
					game->pacman.originDt = tick;
					godChange = true;
				}
				godDt = tick - game->pacman.originDt;


				if(pac->missile == 1)					
					for (int i = 0; i < 2; i++) draw_missile(&game->missiles[i]);				

			}
			
			if(game->mode != SoloState) {
					pac = &game->pacman_enemy;
					
					if(game->mode != SoloState && game->pacman_enemy.livesLeft != -1) draw_pacman2(&game->pacman_enemy);
					//draw_pacman2(&game->pacman_enemy);
			
					if(game->pacman_enemy.godMode == false) {

						if(pac->missile == 1)					
							for (int i = 0; i < 2; i++) draw_missile(&game->missiles[i]);				
							
					} else {

						if(godChange == false) {
							game->pacman_enemy.originDt = tick;
							godChange = true;
						}
						godDt = tick - game->pacman_enemy.originDt;


						if(pac->missile == 1)					
							for (int i = 0; i < 2; i++) draw_missile(&game->missiles[i]);				

					}
			}
			break;
		case WinState2:
			
			if(game->pacman.livesLeft != -1) draw_pacman_static(&game->pacman);
			if(game->mode != SoloState && game->pacman_enemy.livesLeft != -1) draw_pacman2_static(&game->pacman_enemy);
			
			//draw_pacman_static(&game->pacman);
			//if(game->mode != SoloState) draw_pacman2_static(&game->pacman_enemy);
			
			
			if (dt < 2000)
			{
				draw_board(&game->board[game->stageLevel]);
			}
			else
			{
				//stop rendering the pen, and do the flash animation
				draw_board_flash(&game->board[game->stageLevel]);
			}
			
			if(game->currentLevel == 6) {
				enter_state2(game,ClearState2);
			}

			break;
		case DeathState2:
			//draw everything the same for 1ish second
			if (dt < 1000)
			{
				//draw everything normally

				//TODO: this actually draws the last frame pacman was on when he died
				if(game->pacman.livesLeft != -1) draw_pacman_static(&game->pacman);
				if(game->mode != SoloState && game->pacman_enemy.livesLeft != -1) draw_pacman2_static(&game->pacman_enemy);


			}
			else
			{
				//draw the death animation
				if(game->pacman.livesLeft != -1) {
					if(game->death_player == One1) draw_pacman_death(&game->pacman, dt - 1000);
					else draw_pacman_static(&game->pacman);
				}
				if(game->mode != SoloState && game->pacman_enemy.livesLeft != -1) {
					if(game->mode != SoloState && game->death_player == Two2) draw_pacman2_death(&game->pacman_enemy, dt - 1000);
					else draw_pacman2_static(&game->pacman_enemy);
				}
				
				
				/*
				if(game->death_player == One1) draw_pacman_death(&game->pacman, dt - 1000);
				else draw_pacman_static(&game->pacman);
				if(game->mode != SoloState && game->death_player == Two2) draw_pacman2_death(&game->pacman_enemy, dt - 1000);
				else draw_pacman2_static(&game->pacman_enemy);
				*/
			}

			draw_large_pellets(&game->pelletHolder[game->stageLevel], true);
			draw_board(&game->board[game->stageLevel]);
			break;
		case GameoverState2:;
			draw_game_gameover();
			draw_board(&game->board[game->stageLevel]);
			draw_credits(num_credits());
			break;
		case ClearState2:
			draw_game_clear();

			draw_board(&game->board[game->stageLevel]);
			draw_credits(num_credits());
			break;
	}
}

static void enter_state2(PacmanGame2 *game, GameState2 state)
{
	//process leaving a state
	switch (game->gameState2)
	{
		case GameBeginState2:
			game->pacman.livesLeft--;
			if(game->mode != SoloState) game->pacman_enemy.livesLeft--;

			break;
		case WinState2:
			game->currentLevel++;
			if(game->stageLevel < STAGE_COUNT -1 ){
				game->stageLevel++;
			}
			game->gameState2 = LevelBeginState2;
			level_init2(game);
			
			break;
		case DeathState2:
			// Player2 died and is starting a new game, subtract a life
			if (state == LevelBeginState2)
			{
				if(death_player == Two2) game->pacman_enemy.livesLeft--;
				else game->pacman.livesLeft--;
				//printf("1: %d / 2: %d\n",game->pacman.livesLeft,game->pacman_enemy.livesLeft);
				pacdeath_init2(game);
			}
		case GameoverState2:
			// gamestart_init(&game, menuSystem.mode);
			break;
		case ClearState2:
			break;
		default: ; //do nothing
	}

	//save highscore to local file(resources/highscore.txt).
	int origin_highscore = readScoreFromFile();
	if (game->pacman.score > origin_highscore) writeScoreToFile(game->pacman.score);

	//process entering a state
	
	bool allPelletsEaten = game->pelletHolder[game->stageLevel].numLeft == 0;

	switch (state)
	{
		case GameBeginState2:
			stop_sound(IntrobgmSound);
			play_sound(LevelStartSound);

			break;
		case LevelBeginState2:
			
			break;
		case GamePlayState2:
			if(game->stageLevel == 0){
				play_sound(Stage1Sound);
				//if(allPelletsEaten) stop_sound(Stage1Sound);
			}
			if(game->stageLevel == 1){
				play_sound(Stage2Sound);
				//if(allPelletsEaten) stop_sound(Stage2Sound);
			}
			if(game->stageLevel == 2){
				play_sound(Stage3Sound);
				//if(allPelletsEaten) stop_sound(Stage3Sound);
			}
			if(game->stageLevel == 3){
				play_sound(Stage4Sound);
				//if(allPelletsEaten) stop_sound(Stage4Sound);
			}
			if(game->stageLevel == 4){
				play_sound(BossSound);
				//if(allPelletsEaten) stop_sound(Stage4Sound);
			}
			break;
		case WinState2:
			stop_sound(Stage1Sound);	
			stop_sound(Stage2Sound);	
			stop_sound(Stage3Sound);	
			stop_sound(Stage4Sound);	
			stop_sound(BossSound);	
			play_sound(GameoverSound);
			break;
		case DeathState2:
			play_sound(PacmanDeathSound);
			stop_sound(Stage1Sound);	
			stop_sound(Stage2Sound);	
			stop_sound(Stage3Sound);	
			stop_sound(Stage4Sound);	
			stop_sound(BossSound);
			break;
		case GameoverState2:
			play_sound(GameoverSound);
			game->currentLevel = 1;
			game->stageLevel = 0;
			//invalidate the state so it doesn't effect the enter_state function
			game->gameState2 = -1;
			if(game->stageLevel == 1){
				stop_sound(Stage1Sound);
				//if(allPelletsEaten) stop_sound(Stage2Sound);
			}

			stop_sound(Stage2Sound);	
			stop_sound(Stage3Sound);	
			stop_sound(Stage4Sound);	
			stop_sound(BossSound);

			level_init2(game);
			break;
		case ClearState2:
			//stop_sound(BossSound);
			play_sound(EndingbgmSound);
			break;
	}

	game->ticksSinceModeChange = ticks_game();
	game->gameState2 = state;
}

//checks if it's valid that pacman could move in this direction at this point in time
bool can_move2(Pacman *pacman, Board *board, Direction dir)
{
	//easy edge cases, tile has to be parallal with a direction to move it
	if ((dir == Up   || dir == Down ) && !on_vert(&pacman->body)) return false;
	if ((dir == Left || dir == Right) && !on_horo(&pacman->body)) return false;

	//if pacman wants to move on an axis and he is already partially on that axis (not 0)
	//it is always a valid move
	if ((dir == Left || dir == Right) && !on_vert(&pacman->body)) return true;
	if ((dir == Up   || dir == Down ) && !on_horo(&pacman->body)) return true;

	//pacman is at 0/0 and moving in the requested direction depends on if there is a valid tile there
	int x = 0;
	int y = 0;
	
	dir_xy(dir, &x, &y);	

	int newX = pacman->body.x + x;
	int newY = pacman->body.y + y;

	return is_valid_square(board, newX, newY) || is_tele_square(newX, newY);
}

static void process_player2(Pacman *pacman, Board *board, Player2 Player2)
{
	
	if (pacman->missedFrames != 0)
	{
		pacman->missedFrames--;
		return;
	}

	Direction oldLastAttemptedDir = pacman->lastAttemptedMoveDirection;
	
	Direction newDir;

	bool dirPressed;
	if(Player2 == One1) {
		dirPressed = dir_pressed_now(&newDir);
	}
	else {
		dirPressed = dir_pressed_now_player2(&newDir);
	}
	// 눌려진 버튼에따라 움직이게함
	//if(!Player2) dirPressed = false;
	
	if (dirPressed)
	{
		//user wants to move in a direction
		pacman->lastAttemptedMoveDirection = newDir;

		//if Player2 holds opposite direction to current walking dir
		//we can always just switch current walking direction
		//since we're on parallel line
		if (newDir == dir_opposite(pacman->body.curDir))
		{
			pacman->body.curDir = newDir;
			pacman->body.nextDir = newDir;
		}

		//if pacman was stuck before just set his current direction as pressed
		if (pacman->movementType == Stuck)
		{
			pacman->body.curDir = newDir;
		}

		pacman->body.nextDir = newDir;
	}
	else if (pacman->movementType == Stuck)
	{
		//pacman is stuck and Player2 didn't move - Player2 should still be stuck.
		//don't do anything
		return;
	}
	else
	{
		//user doesn't want to change direction and pacman isn't stuck
		//pacman can move like normal

		//just set the next dir to current dir
		pacman->body.nextDir = pacman->body.curDir;
	}

	pacman->movementType = Unstuck;

	int curDirX = 0;
	int curDirY = 0;
	int nextDirX = 0;
	int nextDirY = 0;

		dir_xy(pacman->body.curDir, &curDirX, &curDirY);
		dir_xy(pacman->body.nextDir, &nextDirX, &nextDirY);
	
	int newCurX = pacman->body.x + curDirX;
	int newCurY = pacman->body.y + curDirY;
	int newNextX = pacman->body.x + nextDirX;
	int newNextY = pacman->body.y + nextDirY;

	bool canMoveCur =  is_valid_square(board, newCurX, newCurY) || is_tele_square(newCurX, newCurY);
	bool canMoveNext = is_valid_square(board, newNextX, newNextY) || is_tele_square(newNextX, newNextY);

	//if pacman is currently on a center tile and can't move in either direction
	//don't move him
	if (on_center(&pacman->body) && !canMoveCur && !canMoveNext)
	{
		pacman->movementType = Stuck;
		pacman->lastAttemptedMoveDirection = oldLastAttemptedDir;

		return;
	}

	move_pacman(&pacman->body, canMoveCur, canMoveNext);

	//if pacman is on the center, and he couldn't move either of  his last directions
	//he must be stuck now
	if (on_center(&pacman->body) && !canMoveCur && !canMoveNext)
	{
		pacman->movementType = Stuck;
		return;
	}

	resolve_telesquare2(&pacman->body);
}

static void process_missiles2(PacmanGame2 *game)
{
	for (int i = 0; i < 2; i++)
	{
		Missile *m = &game->missiles[i];

		if (m->m_movementMode == InPen)
		{
			bool moved = move_missile(&m->body);

			if (moved && (m->body.y == 13 || m->body.y == 15))
			{
				m->body.nextDir = m->body.curDir;
				m->body.curDir = dir_opposite(m->body.curDir);
			}

			continue;
		}

		if (m->m_movementMode == LeavingPen)
		{

			//move em to the center of the pen (in x axis)
			//then more em up out the gate
			//when they are out of the gate, set them to be in normal chase mode then set them off on their way

			continue;
		}

		//all other modes can move normally (I think)
		MovementResult result = move_missile(&m->body);
		resolve_telesquare2(&m->body);

		if (result == NewSquare)
		{
			m->nextDirection = next_direction(m, &game->board[game->stageLevel]);
		}
		else if (result == OverCenter)
		{
			//they've hit the center of a tile, so change their current direction to the new direction
			m->body.curDir = m->transDirection;
			m->body.nextDir = m->nextDirection;
			m->transDirection = m->nextDirection;
		}
	}
}

static void process_pellets2(PacmanGame2 *game)
{
	int j = 0;
	//if pacman and pellet collide
	//give pacman that many points
	//set pellet to not be active
	//decrease num of alive pellets
	
	PelletHolder *holder = &game->pelletHolder[game->stageLevel];
	

	for (int i = 0; i < holder->totalNum; i++)
	{
		Pellet *p = &holder->pellets[i];

		//skip if we've eaten this one already
		if (p->eaten) continue;
		bool flag = false;
		if (collides_obj(&game->pacman.body, p->x, p->y))
		{
			flag = true;
			holder->numLeft--;
			// printf("numLeft : %d\n", holder->numLeft);

			p->eaten = true;
			game->pacman.score += pellet_points(p);
			if(pellet_check(p)) {
				game->pacman.godMode = true;
				game->pacman.originDt = ticks_game();
				game->pacman_enemy.godMode = true;
				game->pacman_enemy.originDt = ticks_game();

			}

			//play eat sound
			play_sound(SmallSound);
			//eating a small pellet makes pacman not move for 1 frame
			//eating a large pellet makes pacman not move for 3 frames
			game->pacman.missedFrames = pellet_nop_frames(p);
			game->pacman_enemy.missedFrames = pellet_nop_frames(p);
			//can only ever eat 1 pellet in a frame, so return
			// return;
		}
		if (collides_obj(&game->pacman_enemy.body, p->x, p->y) && flag == false)
		{

			holder->numLeft--;
			// printf("numLeft : %d\n", holder->numLeft);
			p->eaten = true;
			game->pacman_enemy.score += pellet_points(p);
			if(pellet_check(p)) {
				game->pacman.godMode = true;
				game->pacman.originDt = ticks_game();
				game->pacman_enemy.godMode = true;
				game->pacman_enemy.originDt = ticks_game();

			}

			//play eat sound
			
			play_sound(SmallSound);
			//eating a small pellet makes pacman not move for 1 frame
			//eating a large pellet makes pacman not move for 3 frames
			game->pacman.missedFrames = pellet_nop_frames(p);
			game->pacman_enemy.missedFrames = pellet_nop_frames(p);

			//can only ever eat 1 pellet in a frame, so return
			// return;
		}
	}
	
	//maybe next time, poor pacman
}
//change must
static bool check_pacghost_collision2(PacmanGame2 *game)
{
	return false;
}

static bool check_ghomissile_collision2(PacmanGame2 *game)
{
	for (int i = 0; i < 2; i++)
	{
		for(int j= 0; j<4; j++) {
			Missile *m = &game->missiles[i];
		

	}
}
	return false;
}

void gamestart_init2(PacmanGame2 *game, int mode)
{
	// play mode 저장
	if(mode == SoloState) game->mode = SoloState;
	else if(mode == MultiState) game->mode = MultiState;
	else game->mode = RemoteState;
	
	level_init2(game);

	// mode가 1(multi) 라면 상대방 팩맨도 생성한다.
	pacman_init(&game->pacman);
	if(game->mode != SoloState) pacman_chaser_init(&game->pacman_enemy);////////////////////////////////////////here
	
	//we need to reset all fruit
	//fuit_init();
	// game->highscore = 0; //TODO maybe load this in from a file..?
	game->highscore = readScoreFromFile();
	game->currentLevel = 1;
	game->stageLevel = 0;

	//invalidate the state so it doesn't effect the enter_state function
	game->gameState2 = -1;
	enter_state2(game, GameBeginState2);
}

void level_init2(PacmanGame2 *game)
{
	//reset pacmans position
	pacman_level_init(&game->pacman);
	if(game->mode != SoloState) pacman_level_init(&game->pacman_enemy);
	
	//reset pellets
	pellets_init(&game->pelletHolder[game->stageLevel]);
	missiles_init(game->missiles);
}

void pacdeath_init2(PacmanGame2 *game)
{
	pacman_level_init(&game->pacman);
	if(game->mode != SoloState) pacman_level_init(&game->pacman_enemy);
	missiles_init(game->missiles);
}

//TODO: make this method based on a state, not a conditional
//or make the menu system the same. Just make it consistant
bool is_game_over2(PacmanGame2 *game, int tick)
{
	//unsigned dt = ticks_game() - game->ticksSinceModeChange;
	unsigned dt = tick - game->ticksSinceModeChange;
	
	return dt > 2000 && game->gameState2 == GameoverState2;
}

int int_length2(int x)
{
    if (x >= 1000000000) return 10;
    if (x >= 100000000)  return 9;
    if (x >= 10000000)   return 8;
    if (x >= 1000000)    return 7;
    if (x >= 100000)     return 6;
    if (x >= 10000)      return 5;
    if (x >= 1000)       return 4;
    if (x >= 100)        return 3;
    if (x >= 10)         return 2;
    return 1;
}

static bool resolve_telesquare2(PhysicsBody *body)
{
	//TODO: chuck this back in the board class somehow

	if (body->y != 14) return false;

	if (body->x == -1) { body->x = 27; return true; }
	if (body->x == 28) { body->x =  0; return true; }

	return false;
}
