/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#define ENTER 10

#ifdef __linux__
#define SYSTEM "clear"
#elif _WIN32
#define SYSTEM "cls"
#elif __APPLE__
#define SYSTEM "clear"
#else 
#error "Unknown platform"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct move
{   
    // Regular move.
    int x_sel, y_sel;
    int x_mov, y_mov;
    bool playerTurn, blocked;
    
    // Castling. 
    bool isCastlingFlow; 
    bool p1_shortCast, p1_longCast;
    bool p2_shortCast, p2_longCast;
    
    // EnPassant.
    bool enPassant;
    int pawnX, pawnY; 
} move;

void runGame(void);

#endif // CHESS_ENGINE_H
