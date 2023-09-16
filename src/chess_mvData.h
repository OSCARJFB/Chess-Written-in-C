#ifndef CHESS_MVDATA_H
#define CHESS_MVDATA_H

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

#endif // CHESS_MVDATA_H
