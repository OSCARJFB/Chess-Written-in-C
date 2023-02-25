/*
    Writen by: Oscar Bergström
    https://github.com/OSCARJFB
*/

#ifndef CHESS_STRUCTS_H
#define CHESS_STRUCTS_H

typedef struct logic
{
    int x_sel, y_sel;
    int x_mov, y_mov;
    bool is_running, playerTurn, blocked;
} logic;

typedef struct castling
{
    int row, col;
    bool shortCast, longCast;
    bool movedP1, movedP2;
} cast;

cast L_cast;

#endif // CHESS_STRUCTS_H