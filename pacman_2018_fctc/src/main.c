#include "main.h"

#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "socket.h"
#include "board.h"
#include "boardloader.h"
#include "fps.h"
#include "game.h"
#include "input.h"
#include "intermission.h"
#include "imageloader.h"
#include "menu.h"
#include "sound.h"
#include "text.h"
#include "window.h"
#include "pellet.h"

#include "game2_chase.h"

// Remote Play 모드에서 정보를 받아옴
static void copy_pacmanGame_info(void);
static void copy_pac_socket_info(void);
static void copy_pacmanGame_info2(void);
static void copy_pac_socket_info2(void);

//Initializes all resources.
static void resource_init(void);

//Initialize all the internal entities needed for the game at startup.
static void game_init(void);

//Called when a game is about to begin (player hits enter from main menu).
static void startgame_init(void);

//Frees all resources.
static void clean_up(void);

//Performs a loop, updating and rendering at 60hz.
static void main_loop(void);

//Defers to appropriate tick, based on current state.
static void internal_tick(void);

//Defers to appropriate render, based on current state.
static void internal_render(void);

//Processess and deals with all SDL events.
static void process_events(Player player);

//Performs specific actions on various keypresses. Used for testing.
static void key_down_hacks(int keycode);

static ProgramState state;
static MenuSystem menuSystem;
static PacmanGame pacmanGame;
static PacmanGame *pac;

static PacmanGame2 pacmanGame2;
static PacmanGame2 *pac2;

//2020 ADD GAME STOP
static STOP_GAME stopGame;
static int beforeState;
static int stopFlag;

// Socket value
static Socket_value *socket_info;
//game mode 1
static PacmanGame_socket *pac_socket;
//game mode 2
static PacmanGame_socket2 *pac_socket2;

static bool gameRunning = true;
static int numCredits = 0;

int main(void)
{
	resource_init();
	game_init();

	main_loop();

	clean_up();

	return 0;
}

static void main_loop(void)
{
	//while (gameRunning && !key_held(SDLK_ESCAPE))
	while (gameRunning && (stopGame != QUIT))
	{
		//2020 ADD
		if (key_held(SDLK_ESCAPE))
		{
			if (stopFlag == 0)
			{
				beforeState = state;
				stopFlag = 1;
			}
			state = CheckQuit;
		}

		if (menuSystem.gameMode == coorperate)
		{
			if (state == Game && pacmanGame.mode == MultiState)
			{
				process_events(Two);
			}
			else if (state == Game && pacmanGame.mode == RemoteState)
			{
				process_events(Two);
			}
			else
			{
				process_events(One);
			}
		}
		else
		{
			if (state == Game && pacmanGame2.mode == MultiState)
			{
				process_events(Two);
			}
			else if (state == Game && pacmanGame2.mode == RemoteState)
			{
				process_events(Two);
			}
			else
			{
				process_events(One);
			}
		}	

		internal_tick();
		internal_render();

		fps_sleep();
	}
}

static void copy_pac_socket_info()
{
	pac_socket->death_player = pacmanGame.death_player;
	pac_socket->tick = pacmanGame.tick;
	pac_socket->gameState = pacmanGame.gameState;
	pac_socket->ticksSinceModeChange = pacmanGame.ticksSinceModeChange;
	pac_socket->highscore = pacmanGame.highscore;
	pac_socket->stageLevel = pacmanGame.stageLevel;
	pac_socket->currentLevel = pacmanGame.currentLevel;
	pac_socket->mode = pacmanGame.mode;

	pac_socket->pacman = pacmanGame.pacman;
	pac_socket->pacman_enemy = pacmanGame.pacman_enemy;

	for (int i = 0; i < 4; i++)
	{
		pac_socket->ghosts[i] = pacmanGame.ghosts[i];
	}
	if (pac_socket->stageLevel == BOSS_STAGE)
		pac_socket->ghosts[4] = pacmanGame.ghosts[4];

	pac_socket->gameItem1 = pacmanGame.gameItem1[pacmanGame.stageLevel];
	pac_socket->gameItem2 = pacmanGame.gameItem2[pacmanGame.stageLevel];
	pac_socket->gameItem3 = pacmanGame.gameItem3[pacmanGame.stageLevel];
	pac_socket->gameItem4 = pacmanGame.gameItem4[pacmanGame.stageLevel];
	pac_socket->gameItem5 = pacmanGame.gameItem5[pacmanGame.stageLevel];

	pac_socket->pelletHolder.pelletNumOfCurrentMap = pacmanGame.pelletHolder[pacmanGame.stageLevel].pelletNumOfCurrentMap;
	pac_socket->pelletHolder.numLeft = pacmanGame.pelletHolder[pacmanGame.stageLevel].numLeft;
	pac_socket->pelletHolder.totalNum = pacmanGame.pelletHolder[pacmanGame.stageLevel].totalNum;

	int pellet_num = pac_socket->pelletHolder.pelletNumOfCurrentMap;
	for (int i = 0; i < NUM_PELLETS; i++)
	{
		pac_socket->pelletHolder.pellets[i].x = pacmanGame.pelletHolder[pacmanGame.stageLevel].pellets[i].x;
		pac_socket->pelletHolder.pellets[i].y = pacmanGame.pelletHolder[pacmanGame.stageLevel].pellets[i].y;
		pac_socket->pelletHolder.pellets[i].eaten = pacmanGame.pelletHolder[pacmanGame.stageLevel].pellets[i].eaten;
		pac_socket->pelletHolder.pellets[i].type = pacmanGame.pelletHolder[pacmanGame.stageLevel].pellets[i].type;
		if (pac_socket->pelletHolder.pellets[i].type == LargePellet)
			pac_socket->pelletHolder.pellets[i].image = large_pellet_image();
		else
			pac_socket->pelletHolder.pellets[i].image = small_pellet_image();
	}
}
static void copy_pac_socket_info2()
{
	//printf("copy_pac_socket2 start\n");
	pac_socket2->death_player = pacmanGame2.death_player;
	pac_socket2->tick = pacmanGame2.tick;
	pac_socket2->gameState = pacmanGame2.gameState2;
	pac_socket2->ticksSinceModeChange = pacmanGame2.ticksSinceModeChange;
	pac_socket2->highscore = pacmanGame2.highscore;
	pac_socket2->stageLevel = pacmanGame2.stageLevel;
	pac_socket2->currentLevel = pacmanGame2.currentLevel;
	pac_socket2->mode = pacmanGame2.mode;
	//puts("1");
	pac_socket2->pacman = pacmanGame2.pacman;
	pac_socket2->pacman_enemy = pacmanGame2.pacman_enemy;
	//puts("2");
	pac_socket2->pelletHolder.pelletNumOfCurrentMap = pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pelletNumOfCurrentMap;
	pac_socket2->pelletHolder.numLeft = pacmanGame2.pelletHolder[pacmanGame2.stageLevel].numLeft;
	pac_socket2->pelletHolder.totalNum = pacmanGame2.pelletHolder[pacmanGame2.stageLevel].totalNum;
	//puts("3");
	int pellet_num = pac_socket2->pelletHolder.pelletNumOfCurrentMap;
	//puts("4");
	for (int i = 0; i < NUM_PELLETS; i++)
	{
		pac_socket2->pelletHolder.pellets[i].x = pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pellets[i].x;
		pac_socket2->pelletHolder.pellets[i].y = pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pellets[i].y;
		pac_socket2->pelletHolder.pellets[i].eaten = pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pellets[i].eaten;
		pac_socket2->pelletHolder.pellets[i].type = pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pellets[i].type;
		if (pac_socket2->pelletHolder.pellets[i].type == LargePellet)
			pac_socket2->pelletHolder.pellets[i].image = large_pellet_image();
		else
			pac_socket2->pelletHolder.pellets[i].image = small_pellet_image();
	}
	//printf("copy_pac_socket2 end\n");
}

static void copy_pacmanGame_info(void)
{
	pacmanGame.death_player = pac_socket->death_player;
	pacmanGame.tick = pac_socket->tick;
	pacmanGame.gameState = pac_socket->gameState;
	pacmanGame.ticksSinceModeChange = pac_socket->ticksSinceModeChange;
	pacmanGame.highscore = pac_socket->highscore;

	pacmanGame.stageLevel = pac_socket->stageLevel;
	pacmanGame.currentLevel = pac_socket->currentLevel;

	pacmanGame.mode = pac_socket->mode;

	pacmanGame.pacman = pac_socket->pacman;
	pacmanGame.pacman_enemy = pac_socket->pacman_enemy;

	for (int i = 0; i < 4; i++)
	{
		pacmanGame.ghosts[i] = pac_socket->ghosts[i];
	}
	if (pacmanGame.stageLevel == BOSS_STAGE)
		pacmanGame.ghosts[4] = pac_socket->ghosts[4];

	pacmanGame.gameItem1[pacmanGame.stageLevel] = pac_socket->gameItem1;
	pacmanGame.gameItem2[pacmanGame.stageLevel] = pac_socket->gameItem2;
	pacmanGame.gameItem3[pacmanGame.stageLevel] = pac_socket->gameItem3;
	pacmanGame.gameItem4[pacmanGame.stageLevel] = pac_socket->gameItem4;
	pacmanGame.gameItem5[pacmanGame.stageLevel] = pac_socket->gameItem5;

	pacmanGame.pelletHolder[pacmanGame.stageLevel].pelletNumOfCurrentMap = pac_socket->pelletHolder.pelletNumOfCurrentMap;
	pacmanGame.pelletHolder[pacmanGame.stageLevel].numLeft = pac_socket->pelletHolder.numLeft;
	pacmanGame.pelletHolder[pacmanGame.stageLevel].totalNum = pac_socket->pelletHolder.totalNum;

	int pellet_num = pacmanGame.pelletHolder[pacmanGame.stageLevel].pelletNumOfCurrentMap;
	for (int i = 0; i < NUM_PELLETS; i++)
	{
		pacmanGame.pelletHolder[pacmanGame.stageLevel].pellets[i].x = pac_socket->pelletHolder.pellets[i].x;
		pacmanGame.pelletHolder[pacmanGame.stageLevel].pellets[i].y = pac_socket->pelletHolder.pellets[i].y;
		pacmanGame.pelletHolder[pacmanGame.stageLevel].pellets[i].eaten = pac_socket->pelletHolder.pellets[i].eaten;
		pacmanGame.pelletHolder[pacmanGame.stageLevel].pellets[i].type = pac_socket->pelletHolder.pellets[i].type;

		if (pacmanGame.pelletHolder[pacmanGame.stageLevel].pellets[i].type == LargePellet)
			pacmanGame.pelletHolder[pacmanGame.stageLevel].pellets[i].image = large_pellet_image();
		else
			pacmanGame.pelletHolder[pacmanGame.stageLevel].pellets[i].image = small_pellet_image();
	}
}

static void copy_pacmanGame_info2(void)
{
	pacmanGame2.death_player = pac_socket2->death_player;
	pacmanGame2.tick = pac_socket2->tick;
	pacmanGame2.gameState2 = pac_socket2->gameState;
	pacmanGame2.ticksSinceModeChange = pac_socket2->ticksSinceModeChange;
	pacmanGame2.highscore = pac_socket2->highscore;

	pacmanGame2.stageLevel = pac_socket2->stageLevel;
	pacmanGame2.currentLevel = pac_socket2->currentLevel;

	pacmanGame2.mode = pac_socket2->mode;

	pacmanGame2.pacman = pac_socket2->pacman;
	pacmanGame2.pacman_enemy = pac_socket2->pacman_enemy;

	pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pelletNumOfCurrentMap = pac_socket2->pelletHolder.pelletNumOfCurrentMap;
	pacmanGame2.pelletHolder[pacmanGame2.stageLevel].numLeft = pac_socket2->pelletHolder.numLeft;
	pacmanGame2.pelletHolder[pacmanGame2.stageLevel].totalNum = pac_socket2->pelletHolder.totalNum;

	int pellet_num = pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pelletNumOfCurrentMap;
	//printf("%d\n",pellet_num);
	for (int i = 0; i < NUM_PELLETS; i++)
	{
		pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pellets[i].x = pac_socket2->pelletHolder.pellets[i].x;
		pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pellets[i].y = pac_socket2->pelletHolder.pellets[i].y;
		pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pellets[i].eaten = pac_socket2->pelletHolder.pellets[i].eaten;
		pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pellets[i].type = pac_socket2->pelletHolder.pellets[i].type;

		if (pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pellets[i].type == LargePellet)
			pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pellets[i].image = large_pellet_image();
		else
			pacmanGame2.pelletHolder[pacmanGame2.stageLevel].pellets[i].image = small_pellet_image();

	}
}

static void internal_tick(void)
{
	switch (state)
	{
	case Menu:
		menu_tick(&menuSystem);
		if (menuSystem.action == GoToGame)
		{
			state = Game;
			startgame_init();
		}
		else if (menuSystem.action == ReadyConnect)
		{
			state = Remote;
			menuSystem.role = Server;
			socket_info = (Socket_value *)malloc(sizeof(Socket_value));
		}
		else if (menuSystem.action == ExplainGame)
		{ // 2020 ADD
			state = GameExplain;
		}

		break;
	case Game:
		if (pacmanGame.mode == RemoteState || pacmanGame2.mode == RemoteState)
		{
			if (menuSystem.gameMode == coorperate) // original
			{
				if (menuSystem.role == Server)
				{
					KeyState key_info;
					recv(socket_info->client_fd, (char *)&key_info, sizeof(KeyState), MSG_WAITALL);
					store_enemy_keysinfo(&key_info);

					game_tick(&pacmanGame);

					pacmanGame.tick = ticks_game();
					pac_socket = (PacmanGame_socket *)malloc(sizeof(PacmanGame_socket));
					copy_pac_socket_info();

					send(socket_info->client_fd, (char *)pac_socket, sizeof(PacmanGame_socket), 0);
				}
				else if (menuSystem.role == Client)
				{
					KeyState key_info;
					keyinfo_store(&key_info);
					send(socket_info->client_fd, (char *)&key_info, sizeof(KeyState), 0);

					pac_socket = (PacmanGame_socket *)malloc(sizeof(PacmanGame_socket));
					recv(socket_info->client_fd, (char *)pac_socket, sizeof(PacmanGame_socket), MSG_WAITALL);

					copy_pacmanGame_info();
				}

				int flag = 0;
				if (is_game_over(&pacmanGame, pacmanGame.tick))
				{
					menu_init(&menuSystem);
					state = Menu;
					pacmanGame.role = None;
					flag = 1;
				}
				if (flag == 1)
				{
					close(socket_info->client_fd);
					if (menuSystem.role == Server)
						close(socket_info->server_fd);
					free(socket_info);
				}
			}
			else // chasing
			{
				if (menuSystem.role == Server)
				{
					//printf("server\n");
					KeyState key_info;
					recv(socket_info->client_fd, (char *)&key_info, sizeof(KeyState), MSG_WAITALL);
					store_enemy_keysinfo(&key_info);
					//printf("enemy key info\n");
					game_tick2(&pacmanGame2);
					//printf("game_tick2\n");

					pacmanGame2.tick = ticks_game();
					//printf("tick game2\n");
					pac_socket2 = (PacmanGame_socket2 *)malloc(sizeof(PacmanGame_socket2));
					copy_pac_socket_info2();
					//printf("copy pac socket \n");
					send(socket_info->client_fd, (char *)pac_socket2, sizeof(PacmanGame_socket2), 0);
				}
				else if (menuSystem.role == Client)
				{
					//printf("client\n");
					KeyState key_info;
					keyinfo_store(&key_info);
					send(socket_info->client_fd, (char *)&key_info, sizeof(KeyState), 0);
					//printf("send client info\n");
					pac_socket2 = (PacmanGame_socket2 *)malloc(sizeof(PacmanGame_socket2));
					recv(socket_info->client_fd, (char *)pac_socket2, sizeof(PacmanGame_socket2), MSG_WAITALL);
					//printf("recieve server data\n");
					copy_pacmanGame_info2();
				}

				int flag = 0;
				if (is_game_over2(&pacmanGame2, pacmanGame2.tick))
				{
					menu_init(&menuSystem);
					state = Menu;
					pacmanGame2.role = None;
					flag = 1;
				}
				if (flag == 1)
				{
					close(socket_info->client_fd);
					if (menuSystem.role == Server)
						close(socket_info->server_fd);
					free(socket_info);
				}
			}
		}
		else
		{
			if (menuSystem.gameMode == coorperate)
			{
				game_tick(&pacmanGame);

				if (is_game_over(&pacmanGame, ticks_game()))
				{
					menu_init(&menuSystem);
					state = Menu;
				}
			}
			else
			{
				game_tick2(&pacmanGame2);

				if (is_game_over2(&pacmanGame2, ticks_game()))
				{
					menu_init(&menuSystem);
					state = Menu;
				}
			}
		}

		break;
	case Remote:
		if (menuSystem.action == ServerWait)
		{
			// listen client
			//2020 ADD
			int handler = connect_server(socket_info);
			if(handler == -1) printf("Failed connection\n");
			else if(handler == 1){ //pause by ESC

			}
			else
				menuSystem.action = GoToGame;
		}

		remote_tick(&menuSystem, socket_info, &state);
		if (menuSystem.action == GoToGame)
		{
			state = Game;
			startgame_init();
		}

		break;
	case Intermission:
		intermission_tick();
		break;
	//2020 ADD
	case GameExplain:
		explain_tick();
		break;
	case MakeGameRoom:
		makegame_tick(&menuSystem, socket_info, &state);
		break;

	case CheckQuit:
		check_quit_tick(&stopGame, &state, &beforeState, &stopFlag, &menuSystem);
		break;
	}
}

static void internal_render(void)
{
	clear_screen(0, 0, 0, 0);

	switch (state)
	{
	case Menu:
		menu_render(&menuSystem);
		break;
	case Game:
		if (menuSystem.gameMode == coorperate)
		{
			if (menuSystem.role == Client)
				game_render(&pacmanGame, pacmanGame.tick);
			else
				game_render(&pacmanGame, ticks_game());
		}
		else
		{
			if (menuSystem.role == Client)
				game_render2(&pacmanGame2, pacmanGame2.tick);
			else
				game_render2(&pacmanGame2, ticks_game());
		}
		break;
	case Remote:
		remote_render(&menuSystem);
		break;
	case Intermission:
		intermission_render();
		break;

	//2020 ADD
	case GameExplain: //explain game
		draw_explain_screen(&menuSystem);
		break;
	case CheckQuit:
		draw_checkquit_screen(stopGame);
		break;
	case MakeGameRoom:
		draw_makegame_screen(&menuSystem);
		break;
	}
	flip_screen();
}

static void game_init(void)
{
	char *mapList[6] = {"maps/stage1_map", "maps/stage2_map", "maps/stage3_map", "maps/stage4_map", "maps/boss_map", "maps/clear_map"};
	int i;
	//Load the board here. We only need to do it once

	for (i = 0; i < STAGE_COUNT; i++)
	{
		load_board(&pacmanGame.board[i], &pacmanGame.pelletHolder[i], mapList[i]);
		load_board(&pacmanGame2.board[i], &pacmanGame2.pelletHolder[i], mapList[i]);
	}

	//set to be in menu
	state = Menu;
	//2020 ADD
	stopGame = NONE;
	stopFlag = 0;

	play_sound(IntrobgmSound);
	//init the framerate manager
	fps_init(60);

	menu_init(&menuSystem);
}

static void startgame_init(void)
{
	if (menuSystem.gameMode == coorperate)
	{
		if (menuSystem.role == Server)
			pacmanGame.role = Server;
		else if (menuSystem.role == Client)
			pacmanGame.role = Client;
		gamestart_init(&pacmanGame, menuSystem.mode);
	}
	else
	{
		if (menuSystem.role == Server)
			pacmanGame2.role = Server;
		else if (menuSystem.role == Client)
			pacmanGame2.role = Client;
		gamestart_init2(&pacmanGame2, menuSystem.mode);
	}
}

static void resource_init(void)
{
	init_window(SCREEN_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
	load_images();
	load_sounds();
	load_text();

	//TODO: ensure all the resources loaded properly with some nice function calls
}

static void clean_up(void)
{
	dispose_window();
	dispose_images();
	dispose_sounds();
	dispose_text();

	SDL_Quit();
}

static void process_events(Player player)
{
	static SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gameRunning = false;

			break;
		case SDL_KEYDOWN:
			// if (pacmanGame.role == Server)
			// 	handle_keydown(event.key.keysym.sym);
			// else if (pacmanGame.role == Client)
			// 	handle_keydown_player2(event.key.keysym.sym);
			// else
			// {
			// 	handle_keydown(event.key.keysym.sym);
			// 	if (player == Two)
			// 		handle_keydown_player2(event.key.keysym.sym);
			// }
			// key_down_hacks(event.key.keysym.sym);
			if (menuSystem.gameMode == coorperate)
			{
				if (pacmanGame.role == Server)
					handle_keydown(event.key.keysym.sym);
				else if (pacmanGame.role == Client)
					handle_keydown_player2(event.key.keysym.sym);
				else
				{
					handle_keydown(event.key.keysym.sym);
					if (player == Two)
						handle_keydown_player2(event.key.keysym.sym);
				}
				key_down_hacks(event.key.keysym.sym);
			}
			else
			{
				if (pacmanGame2.role == Server)
					handle_keydown(event.key.keysym.sym);
				else if (pacmanGame2.role == Client)
					handle_keydown_player2(event.key.keysym.sym);
				else
				{
					handle_keydown(event.key.keysym.sym);
					if (player == Two)
						handle_keydown_player2(event.key.keysym.sym);
				}
				key_down_hacks(event.key.keysym.sym);
			}

			break;
		case SDL_KEYUP:
			if (menuSystem.gameMode == coorperate)
			{
				if (pacmanGame.role == Server)
					handle_keyup(event.key.keysym.sym);
				else if (pacmanGame.role == Client)
					handle_keyup_player2(event.key.keysym.sym);
				else
				{
					handle_keyup(event.key.keysym.sym);
					if (player == Two)
						handle_keyup_player2(event.key.keysym.sym);
				}
			}
			else
			{
				if (pacmanGame2.role == Server)
					handle_keyup(event.key.keysym.sym);
				else if (pacmanGame2.role == Client)
					handle_keyup_player2(event.key.keysym.sym);
				else
				{
					handle_keyup(event.key.keysym.sym);
					if (player == Two)
						handle_keyup_player2(event.key.keysym.sym);
				}
			}

			break;
			case SDL_VIDEORESIZE:
				{
					dispose_window();
					screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
					scr_temp_rec.w = event.resize.w;
					scr_temp_rec.h = event.resize.h;
				}
		}

	}

	keyevents_finished();
}

static void key_down_hacks(int keycode)
{
	if (keycode == SDLK_RETURN)
	{
		if (menuSystem.gameMode == coorperate)
			pacmanGame.currentLevel++;
		else
			pacmanGame2.currentLevel++;
	}

	if (keycode == SDLK_BACKSPACE)
		menuSystem.ticksSinceModeChange = SDL_GetTicks();
	static bool rateSwitch = false;

	//TODO: remove this hack and try make it work with the physics body
	if (keycode == SDLK_SPACE)
		fps_sethz((rateSwitch = !rateSwitch) ? 200 : 60);
	//if (keycode == SDLK_SPACE && state != Remote) fps_sethz((rateSwitch = !rateSwitch) ? 200 : 60);

	//TODO: move logic into the tick method of the menu
	if (state == Menu && keycode == SDLK_5 && numCredits < 99)
	{
		numCredits++;
	}

	if (state == Menu)
	{
		if (keycode == SDLK_DOWN)
		{
			menuSystem.mode++;
			if (menuSystem.mode > 3)
				menuSystem.mode = 0;
			play_sound(BeepSound);
		}

		if (keycode == SDLK_UP)
		{
			menuSystem.mode--;
			if (menuSystem.mode == -1)
				menuSystem.mode = 3;
			play_sound(BeepSound);
		}
	}
	else if (state == Remote)
	{

		if (keycode == SDLK_DOWN)
		{
			menuSystem.role++;
			if (menuSystem.role > 2)
				menuSystem.role = 1;
		}

		if (keycode == SDLK_UP)
		{
			menuSystem.role--;
			if (menuSystem.role == 0)
				menuSystem.role = 2;
		}
	}
	else if (state == GameExplain) //2020 ADD
	{
		menuSystem.explainPage++;
		if (menuSystem.explainPage > 1)
		{
			menuSystem.explainPage = 0;
			menuSystem.action = Nothing;
			menuSystem.action = Nothing;
			state = Menu;
		}
	}
	else if (state == CheckQuit)
	{
		if (keycode == SDLK_DOWN)
		{
			stopGame++;
			if (stopGame > 2)
				stopGame = 0;
		}

		if (keycode == SDLK_UP)
		{
			stopGame--;
			if (stopGame == -1)
				stopGame = 2;
		}
	}
	else if (state == MakeGameRoom)
	{
		if (keycode == SDLK_DOWN)
		{
			menuSystem.gameMode++;
			if (menuSystem.gameMode > 1)
				menuSystem.gameMode = 0;
		}

		if (keycode == SDLK_UP)
		{
			menuSystem.gameMode--;
			if (menuSystem.gameMode == -1)
				menuSystem.gameMode = 1;
		}
	}

	if (menuSystem.action == ConnectClient)
	{
		int len = strlen(menuSystem.severIP);

		if ((keycode == SDLK_BACKSPACE) && (len > 0))
			menuSystem.severIP[len - 1] = NULL;
		if (len < 20 && ((keycode >= SDLK_0 && keycode <= SDLK_9) || keycode == SDLK_PERIOD))
			menuSystem.severIP[len] = keycode;
	}
	//if (keycode == SDLK_9 && state != Remote)
	if (menuSystem.gameMode == coorperate)
	{
		if (keycode == SDLK_9)
		{
			for (int i = 0; i < 4; i++)
				pacmanGame.ghosts[i].body.velocity += 5;
		}
		//else if (keycode == SDLK_0 && state != Remote)
		else if (keycode == SDLK_0)
		{
			for (int i = 0; i < 4; i++)
				pacmanGame.ghosts[i].body.velocity -= 5;
		}
	}
}

int num_credits(void)
{
	return numCredits;
}
