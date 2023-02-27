/*
    Writen by: Oscar Bergström
    https://github.com/OSCARJFB
*/

#ifndef CHESS_PROTOTYPES_H
#define CHESS_PROTOTYPES_H
#include "chess_macros.h"
#include "chess_structs.h"

logic initGame(void);

castling initCastling(void);

void runGame(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], 
             logic L, castling castling_data);

void drawConsole(char chessBoard[SIZE_EIGHT][SIZE_EIGHT]);

logic getUserInput(char[SIZE_EIGHT][SIZE_EIGHT], logic L);

logic isPathBlocked(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L); 

castling castlingControl(char[SIZE_EIGHT][SIZE_EIGHT], logic L, castling castling_data);

logic executeMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], 
                  logic L, castling castling_data);

logic executeCastlingMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L, 
                          int kingX, int kingY, castling castling_data);

logic executeRegularMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L, int kingX, int kingY);

int translateLetter(char letter);

bool isUpperOrLower(char letter);

bool isTargetUnderThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L, int targetX, int targetY);

bool gameRules(char[SIZE_EIGHT][SIZE_EIGHT], logic L);

bool pawn(char[SIZE_EIGHT][SIZE_EIGHT], logic L);

bool rook(char[SIZE_EIGHT][SIZE_EIGHT], logic L);

bool knight(char[SIZE_EIGHT][SIZE_EIGHT], logic L);

bool bishop(char[SIZE_EIGHT][SIZE_EIGHT], logic L);

bool queen(char[SIZE_EIGHT][SIZE_EIGHT], logic L);

bool king(char[SIZE_EIGHT][SIZE_EIGHT], logic L);

bool targetStatus(char[SIZE_EIGHT][SIZE_EIGHT], logic L);

bool checkmate(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L);

bool findTheKing(char chessBoard[SIZE_EIGHT][SIZE_EIGHT],
                 int *kingX, int *kingY, bool playerTurn); 

bool isKingInCheck(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
                   int kingX, int kingY);

bool isKingLocked(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
                  int kingX, int kingY);

bool isThreatRemoveable(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
                        int kingX, int kingY);

void findThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], bool playerTurn, int kingX, int kingY,
                int *foundThreatX, int *foundThreatY); 

bool removalofThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
                     int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT], int pathSize);

int getPath(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
            int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT]);

bool removalofThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
                     int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT], int pathSize);

bool tryMoveAtPath(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT],
                   int pathSize, int x, int y, logic L);


#endif // CHESS_PROTOTYPES_H