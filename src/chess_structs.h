/*
    Writen by: Oscar Bergström
    https://github.com/OSCARJFB
*/

#ifndef CHESS_STRUCTS_H
#define CHESS_STRUCTS_H

typedef struct move
{
    int x_sel, y_sel;
    int x_mov, y_mov;
    bool playerTurn, blocked;
    bool isCastlingFlow; 
} move;

typedef struct castling
{
    bool p1_shortCast, p1_longCast;
    bool p2_shortCast, p2_longCast;
} castling;

#endif // CHESS_STRUCTS_H