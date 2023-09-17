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
#include "chess_term_mvData.h"

void runGame(void);

#endif // CHESS_ENGINE_H
