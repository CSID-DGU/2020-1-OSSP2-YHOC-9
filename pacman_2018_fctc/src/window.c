#include "window.h"

#define SCREEN_WID 448
#define SCREEN_HEI 640

static SDL_Surface *screen;
static SDL_ResizeEvent event_w;

bool init_window(const char* title, int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}

	screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE | SDL_RESIZABLE);

	if (screen == NULL)
	{
		return false;
	}

	SDL_WM_SetCaption(title, NULL);

	return true;
}

void dispose_window(void)
{
	SDL_FreeSurface(screen);
}

SDL_Surface *get_screen(void)
{
	return screen;
}

void clear_screen(int r, int g, int b, int a)
{
	Uint32 col = SDL_MapRGBA(screen->format, r, g, b, a);

	SDL_FillRect(screen, NULL, col);
}

void apply_surface(int x, int y, SDL_Surface* source)
{
	SDL_Rect offset;
	if(event_w.w != 0 || event_w.h != 0)
	{
		offset.x = (event_w.w / SCREEN_WID) * x;
		offset.y = (event_w.h / SCREEN_HEI) * y;
	}
	else
	{
		offset.x = x;
		offset.y = y;
	}
	
	SDL_BlitSurface(source, NULL, screen, &offset);
}

void flip_screen(void)
{
	SDL_Flip(screen);
}
