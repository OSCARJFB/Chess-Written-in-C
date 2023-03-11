/*
    Writen by: Oscar Bergström
    https://github.com/OSCARJFB
*/

#ifndef CHESS_PROTOTYPES_H
#define CHESS_PROTOTYPES_H
#include "chess_macros.h"
#include "chess_structs.h"

// Struct stup! 
move initMove(void);

// Initiates a loop running the game untill break condition is meet. 
void runGame(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Draw chessboard to the console. 
void drawConsole(char chessBoard[SIZE_EIGHT][SIZE_EIGHT]);

// Ask the user for input which is store in move data structure. 
move getUserInput(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Scan the path of a chessmove and to verify if it's blocked or not. 
move isPathBlocked(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data); 

// This conroller validate castling moves by several function calls. 
move castlingController(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Validates the position of the pieces involed in castling to determine if short/long cast is possible. 
move isCastlingOk(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Tracks if user input corresponds to a castling move.
bool isCastlingMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Scan if the path is open between the king and rook. 
bool isCastlingPathOk(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Make sure the castling if done does not result in the king being in check.
bool tryCastlingMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Executes a chessmove. 
move executeMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Keeps track of EnPassant logic. 
move enPassant(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data); 

// This function will translate letter input from the user to a numeric value. 
int translateLetter(char letter);

// Am I UPPERor lower!! case?
bool isUpperOrLower(char letter);

// Find out if a piece is under threat. 
bool isTargetUnderThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data, int targetX, int targetY);

// Game rules checking different pieces momvent patterns. 
bool gameRules(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Pawn rules. 
bool pawn(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// If EnPassant was performed. 
bool executeEnPassant(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data); 

// Rook rules. 
bool rook(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Knight rules. 
bool knight(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Bishop rules. 
bool bishop(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Queen rules. 
bool queen(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// King rules.
bool king(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Constrains attacking of own pieces. 
bool targetStatus(char[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Checkmate -> game over! 
bool checkmate(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data);

// Find the position of the king. 
bool findTheKing(char chessBoard[SIZE_EIGHT][SIZE_EIGHT],
                 int *kingX, int *kingY, bool playerTurn); 

// Finds out if the king is in check. 
bool isKingInCheck(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data,
                   int kingX, int kingY);

// Can the king move?
bool isKingLocked(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data,
                  int kingX, int kingY);

// Can the threat be removed?
bool isThreatRemoveable(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data,
                        int kingX, int kingY);

// Who is threating the king?
void findThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], bool playerTurn, int kingX, int kingY,
                int *foundThreatX, int *foundThreatY); 

// Gets the path of a pieces movement. 
int getPath(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data,
            int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT]);

// Try to remove a threat!
bool removalofThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], move m_data,
                     int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT], int pathSize);

// Can another piece block the threat? 
bool tryMoveAtPath(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT],
                   int pathSize, int x, int y, move m_data);


#endif // CHESS_PROTOTYPES_H