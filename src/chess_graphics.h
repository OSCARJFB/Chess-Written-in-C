#ifndef CHESS_GRAPHICS_H
#define CHESS_GRAPHICS_H

#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>
#include "chess_mvData.h"
#include "chess_engine.h"

#define CHESS_BLACK (Color){0, 0, 0, 255} 
#define CHESS_WHITE (Color){255, 255, 255, 255} 
#define CHESS_GREEN (Color){0, 255, 0, 255}
#define CHESS_RED (Color){255, 0, 0, 255}
#define CHESS_P1 (Color){150, 55, 0, 255}
#define CHESS_P1 (Color){150, 55, 0, 255}
#define CHESS_P2 (Color){255, 200, 150, 255}

enum options
{
	exitRound = 27,
	play = 49,
	console,
	exitGame,
}; 

typedef struct drawData
{
	int x, y; 
	int size;
} drawData; 

void mainMenu(void); 

#endif  // CHESS_GRAPHICS_H
