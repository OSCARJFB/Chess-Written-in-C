/*
    Writen by: Oscar Bergström
    https://github.com/OSCARJFB
*/

#ifndef CHESS_PROTOTYPES_H
#define CHESS_PROTOTYPES_H
#include "chess_macros.h"
#include "chess_structs.h"

move initGame(void);

castling initCastling(void);

void runGame(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], 
             move m_data, castling c_data);

void drawConsole(char chessBoard[SIZE_EIGHT][SIZE_EIGHT]);

move getUserInput(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

move isPathBlocked(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data); 

castling castlingController(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data, castling c_data);

castling isCastlingOk(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data, castling c_data);

bool isCastlingMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data);

bool isCastlingPathOk(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data);

bool castlingResultInCheck(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data);

castling setCanCastFlag(move m_data, castling c_data);

move executeMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], 
                  move m_data, castling c_data);

move executeCastlingMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data, 
                          int kingX, int kingY, castling c_data);

move executeRegularMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data, int kingX, int kingY);

int translateLetter(char letter);

bool isUpperOrLower(char letter);

bool isTargetUnderThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data, int targetX, int targetY);

bool gameRules(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

bool pawn(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

bool rook(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

bool knight(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

bool bishop(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

bool queen(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

bool king(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

bool targetStatus(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

bool checkmate(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data);

bool findTheKing(char chessBoard[SIZE_EIGHT][SIZE_EIGHT],
                 int *kingX, int *kingY, bool playerTurn); 

bool isKingInCheck(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data,
                   int kingX, int kingY);

bool isKingLocked(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data,
                  int kingX, int kingY);

bool isThreatRemoveable(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data,
                        int kingX, int kingY);

void findThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], bool playerTurn, int kingX, int kingY,
                int *foundThreatX, int *foundThreatY); 

bool removalofThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data,
                     int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT], int pathSize);

int getPath(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data,
            int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT]);

bool removalofThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data,
                     int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT], int pathSize);

bool tryMoveAtPath(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT],
                   int pathSize, int x, int y, move m_data);


#endif // CHESS_PROTOTYPES_H