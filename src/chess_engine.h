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
#include "chess_mvData.h"
#include "chess_graphics.h"

move initMove(void);
move isPathBlocked(char chessBoard[8][8], move m_data);
move castlingController(char chessBoard[8][8], move m_data);
move executeMove(char chessBoard[8][8], move m_data);
move enPassant(char chessBoard[8][8], move m_data);
bool checkmate(char chessBoard[8][8], move m_data); 
void runFromConsole(void);

#endif // CHESS_ENGINE_H
