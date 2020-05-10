#pragma once

#include "main.h"
#include "socket.h"
//Defines what action we should take at a particular time while on the menu.
typedef enum
{
	Nothing,
	GoToGame,
	ReadyConnect,
	ServerWait,
	ConnectClient,
	ExplainGame			//2020 ADD
} MenuAction;

//Defines the menu system.
typedef struct
{
	char* severIP;
	RemoteRole role;
	ModeState mode;
	MenuAction action;
	unsigned int ticksSinceModeChange;

	int explainPage; //2020 ADD

} MenuSystem;

//Performs a single tick on the menu system.
void menu_tick(MenuSystem *menuSystem);
void remote_tick(MenuSystem *menuSystem, Socket_value *socket_info);

//Renders the menu system in its current state.
void menu_render(MenuSystem *menuSystem);
void remote_render(MenuSystem *menuSystem);

void menu_init(MenuSystem *menuSystem);

//2020 ADD
void draw_explain_screen(MenuSystem *menuSystem);
void explain_tick();
void draw_checkquit_screen(int check);
void check_quit_tick(int *check, int *state,int *beforeState, int* stopFlag, MenuSystem* menuSys);
