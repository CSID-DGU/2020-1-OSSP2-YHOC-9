#pragma once

//28 across, 36 down, 16 pixels each square.
//28 across, 40 down, 16 pixels each square.
#define SCREEN_TITLE "PACMAN - ver.2020YHOC"
#define SCREEN_WIDTH 448
#define SCREEN_HEIGHT 640 //origin : 576

//Defines the main states the program can be in.
typedef enum
{
	Menu,
	Game,
	Remote,
	Intermission,

	GameExplain, //2020 ADD
	CheckQuit,
	MakeGameRoom
} ProgramState;

typedef enum
{
	SoloState,
	MultiState,
	RemoteState,

	ExplainState //2020 ADD
} ModeState;

typedef enum
{
	None,
	Server,
	Client
} RemoteRole;

//Returns the number of credits the user currently has.
int num_credits(void);

//2020 ADD
typedef enum
{
	NONE,
	GoMenu,
	GoBack,
	QUIT
} STOP_GAME;

//2020 ADD multi game mode
typedef enum
{
	coorperate,
	chase
} MultiGameMode;
