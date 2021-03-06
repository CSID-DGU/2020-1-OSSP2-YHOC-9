#include "pacman.h"

void pacman_init(Pacman *pacman)
{
	pacman_location_init(pacman, 1);
	pacman->score = 0;
	pacman->livesLeft = 3;
	pacman->itemRemainTime = 0;
	pacman->missedFrames = 0;
	pacman->godMode = false;
	pacman->itemOn = false;
	printf("original\n");
}

void pacman_chaser_init(Pacman *pacman)
{
	pacman_location_init(pacman, 2);
	pacman->score = 0;
	pacman->livesLeft = 3;
	pacman->itemRemainTime = 0;
	pacman->missedFrames = 0;
	pacman->godMode = false;
	pacman->itemOn = false;
	printf("chaser\n");
}

void pacman_level_init(Pacman *pacman,int player)
{
	if(player == 1){
	pacman_location_init(pacman,1);

	}else{
	pacman_location_init(pacman,2);

	}
}

void pacman_location_init(Pacman *pacman, int player)
{
	if (player == 1)
	{
		pacman->body = (PhysicsBody){14, 23, -8, 0, Left, Left, 1, 0, 0};
		pacman->body.velocity = 80;
		pacman->movementType = Unstuck;
		pacman->lastAttemptedMoveDirection = Left;
	}
	else
	{
		pacman->body = (PhysicsBody){14, 11, -8, 0, Left, Left, 1, 0, 0};
		pacman->body.velocity = 80;
		pacman->movementType = Unstuck;
		pacman->lastAttemptedMoveDirection = Left;
	}
}

int pacman_speed_normal(int level)
{
	return 80;
}

int pacman_speed_fright(int level)
{
	if (level == 1)
		return 90;
	if (level <= 4)
		return 95;
	if (level <= 20)
		return 100;
	return 100;
}
