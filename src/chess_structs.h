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
} move;

typedef struct castling
{
    int row, col;
    bool shortCast, longCast;
} castling;

#endif // CHESS_STRUCTS_H