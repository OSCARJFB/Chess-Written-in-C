/*
    Writen by: Oscar Bergström
    https://github.com/OSCARJFB
*/

#ifndef CHESS_PROTOTYPES_H
#define CHESS_PROTOTYPES_H
#include "chess_macros.h"
#include "chess_structs.h"

void runGame(char[SIZE][SIZE], logic);
void drawConsole(char[SIZE][SIZE]);

logic initGame(logic);
logic getUserInput(char[SIZE][SIZE], logic);
logic executeMove(char chessBoard[SIZE][SIZE], logic);

int translateLetter(char);
int *scanBoard(char[SIZE][SIZE], logic,
               bool *, bool, int *);
bool isUpperOrLower(char);

bool checkmate(char[SIZE][SIZE], logic);
bool lookForMoveAtTarget(char[SIZE][SIZE], logic,
                        int, int, int *, int *);

bool gameRules(char[SIZE][SIZE], logic);
bool pawn(char[SIZE][SIZE], logic);
bool rook(char[SIZE][SIZE], logic);
bool knight(char[SIZE][SIZE], logic);
bool bishop(char[SIZE][SIZE], logic);
bool queen(char[SIZE][SIZE], logic);
bool king(char[SIZE][SIZE], logic);

bool castling(char[SIZE][SIZE], logic);
bool targetStatus(char[SIZE][SIZE], logic);

#endif // CHESS_PROTOTYPES_H