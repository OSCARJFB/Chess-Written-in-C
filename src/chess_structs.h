/*
	Writen by: Oscar Bergström
	https://github.com/OSCARJFB

	MIT License
	Copyright (c) 2023 Oscar Bergström
*/

#ifndef CHESS_STRUCTS_H
#define CHESS_STRUCTS_H

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

#endif // CHESS_STRUCTS_H