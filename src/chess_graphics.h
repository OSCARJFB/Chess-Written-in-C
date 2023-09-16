#ifndef CHESS_GRAPHICS_H
#define CHESS_GRAPHICS_H

#include <raylib.h>
#include <stdbool.h>
#include "chess_mvData.h"
#include "chess_engine.h"

void initGraphics(void); 
void drawBoard(char chessBoard[8][8], move m_data); 

#endif  // CHESS_GRAPHICS_H
