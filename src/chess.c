/*
    Writen by: Oscar Bergström
    https://github.com/OSCARJFB
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "chess_prototypes.h"
#include "chess_macros.h"
#include "chess_structs.h"

void failed_allocation(void)
{
    printf("Memory allocation failed:\nError exit with return code 1");
    exit(EXIT_FAILURE);
}

int main(void)
{
    char chessBoard[SIZE_EIGHT][SIZE_EIGHT] =
        {
            'R', 'K', 'B', 'Q', 'W', 'B', 'K', 'R',
            'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
            'r', 'k', 'b', 'q', 'w', 'b', 'k', 'r'};

    logic L = initGame();
    castling castling_data = initCastling();

    runGame(chessBoard, L, castling_data);

    return EXIT_SUCCESS;
}

logic initGame(void)
{
    logic L;
    L.x_sel = L.y_sel = -1;
    L.x_mov = L.y_mov = -1;
    L.is_running = L.playerTurn = true;

    return L;
}

castling initCastling(void)
{
    castling castling_data;
    castling_data.movedP1 = castling_data.movedP2 = false;
    castling_data.shortCast = castling_data.longCast = false; 
    
    return castling_data;
}

void runGame(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], 
            logic L, castling castling_data)
{
    while (L.is_running == true)
    {
        drawConsole(chessBoard);
        L = getUserInput(chessBoard, L);
        L = isPathBlocked(chessBoard, L);
        castling_data = castlingControl(chessBoard, L, castling_data);
        L = executeMove(chessBoard, L, castling_data);
        if (checkmate(chessBoard, L))
        {
            break;
        }
    }

    printf("\n###### Checkmate ######\n");
}

void drawConsole(char chessBoard[SIZE_EIGHT][SIZE_EIGHT])
{
    int board_numbers = 1;

    system("clear");

    for (int i = 0; i < SIZE_EIGHT; ++i)
    {
        printf("%d.", board_numbers++);
        for (int j = 0; j < SIZE_EIGHT; ++j)
        {
            printf("[%c]", chessBoard[i][j]);
        }
        printf("\n");
    }
    printf("   A  B  C  D  E  F  G  H\n");
}

logic getUserInput(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L)
{
    char *userInput = malloc(sizeof(char));
    if (userInput == NULL)
    {
        failed_allocation();
    }

    int key_pressed = 0, sizeOfArray = 0;

    L.playerTurn == true ? printf("Player one enter a move:") : printf("Player two enter a move:");

    while (key_pressed != ENTER)
    {
        key_pressed = getchar();

        if (key_pressed != ENTER)
        {
            userInput[sizeOfArray++] = (char)key_pressed;
            userInput = realloc(userInput, (sizeOfArray + 1) * sizeof(char));
            if (userInput == NULL)
            {
                failed_allocation();
            }
        }
        else
        {
            userInput[sizeOfArray] = '\0';
        }
    }

    if (strlen(userInput) > 4 || strlen(userInput) < 4)
    {
        printf("\nWrong input format, it should be:\n[LETTER][SINGLE DIGIT][LETTER][SINGLE DIGIT]\nPress any key to continue...");
        free(userInput);
        getchar();
        return L;
    }

    sizeOfArray = 0;
    while (userInput[sizeOfArray] != '\0')
    {
        switch (sizeOfArray)
        {
        case 0:
            if (((int)userInput[sizeOfArray] >= 97 && (int)userInput[sizeOfArray] <= 104) ||
                ((int)userInput[sizeOfArray] >= 65 && (int)userInput[sizeOfArray] <= 72))
            {
                L.x_sel = translateLetter(userInput[sizeOfArray]);
            }
            break;
        case 1:
            if (userInput[sizeOfArray] - '0' >= 1 && userInput[sizeOfArray] - '0' <= 9)
            {
                L.y_sel = userInput[sizeOfArray] - '0' - 1;
            }
            break;
        case 2:
            if (((int)userInput[sizeOfArray] >= 97 && (int)userInput[sizeOfArray] <= 104) ||
                ((int)userInput[sizeOfArray] >= 65 && (int)userInput[sizeOfArray] <= 72))
            {
                L.x_mov = translateLetter(userInput[sizeOfArray]);
            }
            break;
        case 3:
            if (userInput[sizeOfArray] - '0' >= 1 && userInput[sizeOfArray] - '0' <= 9)
            {
                L.y_mov = userInput[sizeOfArray] - '0' - 1;
            }
            break;
        }
        sizeOfArray++;
    }

    free(userInput);
    return L;
}

castling castlingControl(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L, castling castling_data)
{
    const int shortC = 7, longC = 0;
    castling_data.row = L.playerTurn == true ? 0 : 7;

    if ((L.playerTurn && castling_data.movedP1 == false) ||
        (!L.playerTurn && castling_data.movedP2 == false))
    {
        if (L.x_mov == shortC && L.y_mov == castling_data.row)
        {
            castling_data.col = 5;
            castling_data.shortCast = true;
            return castling_data;
        }
        else if (L.x_mov == longC && L.y_mov == castling_data.row)
        {
            castling_data.col = 3;
            castling_data.longCast = true;
            return castling_data;
        }
    }

    return castling_data;
}

logic executeMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], 
                  logic L, castling castling_data)
{
    int kingX = 0, kingY = 0;
    if (!findTheKing(chessBoard, &kingX, &kingX, L.playerTurn))
    {
        puts("checkmate: Error Couldn't find the king.");
        exit(EXIT_FAILURE);
    }

    if (L.blocked == false || chessBoard[L.y_sel][L.x_sel] == 'k' || chessBoard[L.y_sel][L.x_sel] == 'K')
    {
        printf("executeMove: %c\n", chessBoard[L.y_sel][L.x_sel]);
        if (gameRules(chessBoard, L))
        {
            if (castling_data.shortCast || castling_data.longCast)
            {
                L = executeCastlingMove(chessBoard, L, 
                                        kingX, kingY, castling_data);
            }
            else
            {
                L = executeRegularMove(chessBoard, L, kingX, kingY);
            }
        }
    }

    return L;
}

logic executeCastlingMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L, 
                          int kingX, int kingY, castling castling_data)
{
    char piece_in_hand = L.playerTurn == true ? 'R' : 'r';

    if (castling_data.shortCast)
    {
        chessBoard[castling_data.row][7] = ' ';
    }
    else
    {
        chessBoard[castling_data.row][0] = ' ';
    }

    chessBoard[castling_data.row][castling_data.col] = piece_in_hand;

    piece_in_hand = L.playerTurn == true ? 'W' : 'w';
    chessBoard[L.y_sel][L.x_sel] = ' ';

    piece_in_hand = castling_data.shortCast == true ? chessBoard[castling_data.row][castling_data.col + 1] : chessBoard[castling_data.row][castling_data.col - 1];

    L.playerTurn = L.playerTurn == true ? false : true;

    if (isTargetUnderThreat(chessBoard, L, kingX, kingY))
    {
        L.playerTurn = L.playerTurn == true ? false : true;
        return L;
    }

    return L;
}

logic executeRegularMove(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
                         int kingX, int kingY)
{
    char piece_in_hand = chessBoard[L.y_mov][L.x_mov];

    chessBoard[L.y_mov][L.x_mov] = chessBoard[L.y_sel][L.x_sel];
    chessBoard[L.y_sel][L.x_sel] = ' ';

    L.playerTurn = L.playerTurn == true ? false : true;

    if (isTargetUnderThreat(chessBoard, L, kingX, kingY))
    {
        L.playerTurn = L.playerTurn == true ? false : true;
        return L;
    }

    return L;
}

int translateLetter(char letter)
{
    if (letter == 'a' || letter == 'A')
    {
        return 0;
    }
    else if (letter == 'b' || letter == 'B')
    {
        return 1;
    }
    else if (letter == 'c' || letter == 'C')
    {
        return 2;
    }
    else if (letter == 'd' || letter == 'D')
    {
        return 3;
    }
    else if (letter == 'e' || letter == 'E')
    {
        return 4;
    }
    else if (letter == 'f' || letter == 'F')
    {
        return 5;
    }
    else if (letter == 'g' || letter == 'G')
    {
        return 6;
    }
    else if (letter == 'h' || letter == 'H')
    {
        return 7;
    }
    else
    {
        return -1;
    }
}

logic isPathBlocked(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L)
{
    int index = 0;
    int x = L.x_sel, y = L.y_sel;
    int pathY[8], pathX[8];

    if (chessBoard[L.y_sel][L.x_sel] == 'K' || chessBoard[L.y_sel][L.x_sel] == 'k')
    {
        L.blocked = false;
        return L;
    }

    /*
     *  Important 2d arrays are reversed from the regular conception of x and y in algebra.
     *  x, y thus become y, x when indexed. hence pathY must be iterated before pathX.
     */

    while (x != L.x_mov || y != L.y_mov)
    {
        pathY[index] = y;
        pathX[index] = x;
        ++index;

        if (L.x_mov > x)
        {
            ++x;
        }
        else if (L.x_mov < x)
        {
            --x;
        }

        if (L.y_mov > y)
        {
            ++y;
        }
        else if (L.y_mov < y)
        {
            --y;
        }
    }

    for (int i = 1; i + 1 < index; ++i)
    {
        if (chessBoard[pathY[i]][pathX[i]] != ' ')
        {
            L.blocked = true;
            return L;
        }
    }

    L.blocked = false;
    return L;
}

bool gameRules(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L)
{
    if ((L.playerTurn && isUpperOrLower(chessBoard[L.y_sel][L.x_sel])) ||
        (!L.playerTurn && !isUpperOrLower(chessBoard[L.y_sel][L.x_sel])))
    {
        if (chessBoard[L.y_sel][L.x_sel] == 'P' || chessBoard[L.y_sel][L.x_sel] == 'p')
        {
            return pawn(chessBoard, L);
        }
        else if (chessBoard[L.y_sel][L.x_sel] == 'R' || chessBoard[L.y_sel][L.x_sel] == 'r')
        {
            return rook(chessBoard, L);
        }
        else if (chessBoard[L.y_sel][L.x_sel] == 'K' || chessBoard[L.y_sel][L.x_sel] == 'k')
        {
            return knight(chessBoard, L);
        }
        else if (chessBoard[L.y_sel][L.x_sel] == 'B' || chessBoard[L.y_sel][L.x_sel] == 'b')
        {
            return bishop(chessBoard, L);
        }
        else if (chessBoard[L.y_sel][L.x_sel] == 'Q' || chessBoard[L.y_sel][L.x_sel] == 'q')
        {
            return queen(chessBoard, L);
        }
        else if (chessBoard[L.y_sel][L.x_sel] == 'W' || chessBoard[L.y_sel][L.x_sel] == 'w')
        {
            return king(chessBoard, L);
        }
    }

    return false;
}

bool pawn(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L)
{
    if (L.playerTurn)
    {
        if (L.y_sel == 1 && L.y_mov == L.y_sel + 2 && L.x_sel == L.x_mov)
        {
            if (chessBoard[L.y_mov][L.x_mov] == ' ')
            {
                return true;
            }
        }
        else if (L.y_mov == L.y_sel + 1 && L.x_sel == L.x_mov)
        {
            if (chessBoard[L.y_mov][L.x_mov] == ' ')
            {
                return true;
            }
        }
        else if (L.y_mov == L.y_sel + 1 && (L.x_mov == L.x_sel + 1 || L.x_mov == L.x_sel - 1) &&
                 chessBoard[L.y_mov][L.x_mov] != ' ')
        {
            if (isUpperOrLower(chessBoard[L.y_mov][L.x_mov]) == false)
            {
                return true;
            }
        }
    }
    else if (!L.playerTurn)
    {

        if (L.y_sel == 6 && L.y_mov == L.y_sel - 2 && L.x_sel == L.x_mov)
        {
            if (chessBoard[L.y_mov][L.x_mov] == ' ')
            {
                return true;
            }
        }
        else if (L.y_mov == L.y_sel - 1 && L.x_sel == L.x_mov)
        {
            if (chessBoard[L.y_mov][L.x_mov] == ' ')
            {
                return true;
            }
        }
        else if (L.y_mov == L.y_sel - 1 && L.x_sel != L.x_mov && (L.x_mov == L.x_sel + 1 || L.x_mov == L.x_sel - 1) &&
                 chessBoard[L.y_mov][L.x_mov] != ' ')
        {
            if (isUpperOrLower(chessBoard[L.y_mov][L.x_mov]))
            {
                return true;
            }
        }
    }

    return false;
}

bool rook(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L)
{
    bool moveIsOK = false;

    if (L.x_sel > L.x_mov && L.y_sel == L.y_mov)
    {
        moveIsOK = true;
    }
    else if (L.x_sel < L.x_mov && L.y_sel == L.y_mov)
    {
        moveIsOK = true;
    }
    else if (L.y_sel > L.y_mov && L.x_sel == L.x_mov)
    {
        moveIsOK = true;
    }
    else if (L.y_sel < L.y_mov && L.x_sel == L.x_mov)
    {
        moveIsOK = true;
    }

    if (targetStatus(chessBoard, L) && moveIsOK)
    {
        return true;
    }

    return false;
}

bool knight(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L)
{
    if (L.y_mov == L.y_sel + 2 && (L.x_mov == L.x_sel + 1 || L.x_mov == L.x_sel - 1))
    {
        return targetStatus(chessBoard, L);
    }
    else if (L.y_mov == L.y_sel - 2 && (L.x_mov == L.x_sel + 1 || L.x_mov == L.x_sel - 1))
    {
        return targetStatus(chessBoard, L);
    }
    else if (L.x_mov == L.x_sel + 2 && (L.y_mov == L.y_sel + 1 || L.y_mov == L.y_sel - 1))
    {
        return targetStatus(chessBoard, L);
    }
    else if (L.x_mov == L.x_sel - 2 && (L.y_mov == L.y_sel + 1 || L.y_mov == L.y_sel - 1))
    {
        return targetStatus(chessBoard, L);
    }

    return false;
}

bool bishop(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L)
{
    int differenceX = 0, differenceY = 0;

    if (L.x_sel < L.x_mov)
    {
        differenceX = L.x_mov - L.x_sel;
    }
    else
    {
        differenceX = L.x_sel - L.x_mov;
    }

    if (L.y_sel < L.y_mov)
    {
        differenceY = L.y_mov - L.y_sel;
    }
    else
    {
        differenceY = L.y_sel - L.y_mov;
    }

    if (differenceX != differenceY)
    {
        return false;
    }

    return targetStatus(chessBoard, L);
}

bool queen(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L)
{
    int differenceX = 0, differenceY = 0;

    if (L.x_sel < L.x_mov)
    {
        differenceX = L.x_mov - L.x_sel;
    }
    else
    {
        differenceX = L.x_sel - L.x_mov;
    }

    if (L.y_sel < L.y_mov)
    {
        differenceY = L.y_mov - L.y_sel;
    }
    else
    {
        differenceY = L.y_sel - L.y_mov;
    }

    if (differenceX == differenceY)
    {
        return targetStatus(chessBoard, L);
    }

    if (L.x_sel > L.x_mov && L.y_sel == L.y_mov)
    {
        return targetStatus(chessBoard, L);
    }
    else if (L.x_sel < L.x_mov && L.y_sel == L.y_mov)
    {
        return targetStatus(chessBoard, L);
    }
    else if (L.y_sel > L.y_mov && L.x_sel == L.x_mov)
    {
        return targetStatus(chessBoard, L);
    }
    else if (L.y_sel < L.y_mov && L.x_sel == L.x_mov)
    {
        return targetStatus(chessBoard, L);
    }

    return false;
}

bool king(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L)
{
    bool moveIsOK = false;

    if (L.y_mov == L.y_sel - 1 && L.x_mov == L.x_sel)
    {
        moveIsOK = true;
    }
    else if (L.y_mov == L.y_sel - 1 && L.x_mov == L.x_sel + 1)
    {
        moveIsOK = true;
    }
    else if (L.y_mov == L.y_sel && L.x_mov == L.x_sel + 1)
    {
        moveIsOK = true;
    }
    else if (L.y_mov == L.y_sel + 1 && L.x_mov == L.x_sel + 1)
    {
        moveIsOK = true;
    }
    else if (L.y_mov == L.y_sel + 1 && L.x_mov == L.x_sel)
    {
        moveIsOK = true;
    }
    else if (L.y_mov == L.y_sel + 1 && L.x_mov == L.x_sel - 1)
    {
        moveIsOK = true;
    }
    else if (L.y_mov == L.y_sel && L.x_mov == L.x_sel - 1)
    {
        moveIsOK = true;
    }
    else if (L.y_mov == L.y_sel - 1 && L.x_mov == L.x_sel - 1)
    {
        moveIsOK = true;
    }

    if (targetStatus(chessBoard, L) && moveIsOK)
    {
        return true;
    }

    return false;
}

bool isTargetUnderThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
                         int x, int y)
{
    L.x_mov = x, L.y_mov = y;
    L.blocked = true;

    for (int i = 0; i < SIZE_EIGHT; ++i)
    {
        for (int j = 0; j < SIZE_EIGHT; ++j)
        {
            L.y_sel = i, L.x_sel = j;

            if (chessBoard[i][j] != ' ')
            {
                if (gameRules(chessBoard, L) == true)
                {
                    L = isPathBlocked(chessBoard, L);

                    if (L.blocked == false)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool targetStatus(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L)
{
    if (L.playerTurn == true)
    {
        if (chessBoard[L.y_mov][L.x_mov] == ' ' || isUpperOrLower(chessBoard[L.y_mov][L.x_mov]) == false)
        {
            return true;
        }
    }
    else if (L.playerTurn == false)
    {
        if (chessBoard[L.y_mov][L.x_mov] == ' ' || isUpperOrLower(chessBoard[L.y_mov][L.x_mov]) == true)
        {
            return true;
        }
    }

    return false;
}

bool isUpperOrLower(char letter)
{
    bool result = false;

    if (letter >= 'A' && letter <= 'Z')
    {
        result = true;
    }

    if (letter >= 'a' && letter <= 'z')
    {
        result = false;
    }

    return result;
}

bool checkmate(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L)
{
    int x = 0, y = 0;

    if (!findTheKing(chessBoard, &x, &y, L.playerTurn))
    {
        puts("checkmate: Error Couldn't find the king.");
        exit(EXIT_FAILURE);
    }

    if (!isKingInCheck(chessBoard, L, x, y))
    {
        return false;
    }

    if (!isKingLocked(chessBoard, L, x, y))
    {
        return false;
    }

    if (!isThreatRemoveable(chessBoard, L, x, y))
    {
        return false;
    }

    return true;
}

bool findTheKing(char chessBoard[SIZE_EIGHT][SIZE_EIGHT],
                 int *kingX, int *kingY, bool playerTurn)
{
    char king = playerTurn == true ? 'W' : 'w';

    for (int i = 0; i < SIZE_EIGHT; ++i)
    {
        for (int j = 0; j < SIZE_EIGHT; ++j)
        {
            if (chessBoard[i][j] == king)
            {
                *kingX = j;
                *kingY = i;

                return true;
            }
        }
    }

    return false;
}

bool isKingInCheck(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
                   int kingX, int kingY)
{
    return isTargetUnderThreat(chessBoard, L, kingX, kingY);
}

bool isKingLocked(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
                  int kingX, int kingY)
{
    for (int i = 0; i < SIZE_EIGHT; ++i)
    {
        switch (i)
        {
        case 0:
            --kingY;
            break;
        case 1:
            ++kingX;
            break;
        case 2:
            ++kingY;
            break;
        case 3:
            ++kingY;
            break;
        case 4:
            --kingX;
            break;
        case 5:
            --kingX;
            break;
        case 6:
            --kingY;
            break;
        case 7:
            --kingY;
            break;
        }

        if (L.playerTurn == true)
        {
            if ((chessBoard[kingY][kingX] == ' ' || !isUpperOrLower(chessBoard[kingY][kingX])) &&
                kingY >= 0 && kingY <= 7 && kingX >= 0 && kingX <= 7)
            {
                if (!isTargetUnderThreat(chessBoard, L, kingX, kingY))
                {
                    return false;
                }
            }
        }
        else
        {
            if ((chessBoard[kingY][kingX] == ' ' || isUpperOrLower(chessBoard[kingY][kingX])) &&
                kingY >= 0 && kingY <= 7 && kingX >= 0 && kingX <= 7)
            {
                if (!isTargetUnderThreat(chessBoard, L, kingX, kingY))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool isThreatRemoveable(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
                        int kingX, int kingY)
{
    int threatX = 0, threatY = 0;
    int pathY[SIZE_EIGHT], pathX[SIZE_EIGHT];
    int pathSize = 0;

    findThreat(chessBoard, L.playerTurn, kingX, kingY,
               &threatX, &threatY);

    L.x_sel = threatX, L.y_sel = threatY;

    pathSize = getPath(chessBoard, L, pathX, pathY);

    return removalofThreat(chessBoard, L,
                           pathX, pathY, pathSize);
}

void findThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], bool playerTurn, int kingX, int kingY,
                int *threatX, int *threatY)
{
    logic L;

    L.blocked = false;
    L.playerTurn = playerTurn == true ? true : false;
    L.x_mov = kingX;
    L.y_mov = kingY;

    for (int i = 0; i < SIZE_EIGHT; ++i)
    {
        for (int j = 0; j < SIZE_EIGHT; ++j)
        {
            L.x_sel = j;
            L.y_sel = i;

            if (gameRules(chessBoard, L))
            {
                L = isPathBlocked(chessBoard, L);
                if (!L.blocked)
                {
                    *threatX = j;
                    *threatX = i;
                    return;
                }
            }
        }
    }
}

int getPath(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
            int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT])
{
    int pathSize = 0;
    int x = L.x_sel, y = L.y_sel;

    if (chessBoard[L.y_sel][L.x_sel] == 'K' || chessBoard[L.y_sel][L.x_sel] == 'k')
    {
        return pathSize;
    }

    /*
     *  Important 2d arrays are reversed from the regular conception of x and y in algebra.
     *  x, y thus become y, x when indexed. hence pathY must be iterated before pathX.
     */

    while (x != L.x_mov || y != L.y_mov)
    {
        pathY[pathSize] = y;
        pathX[pathSize] = x;
        ++pathSize;

        if (L.x_mov > x)
        {
            ++x;
        }
        else if (L.x_mov < x)
        {
            --x;
        }

        if (L.y_mov > y)
        {
            ++y;
        }
        else if (L.y_mov < y)
        {
            --y;
        }
    }

    return pathSize;
}

bool removalofThreat(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], logic L,
                     int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT], int pathSize)
{
    for (int i = 0; i < SIZE_EIGHT; ++i)
    {
        for (int j = 0; j < SIZE_EIGHT; ++j)
        {
            if (isUpperOrLower(chessBoard[i][j]) == L.playerTurn)
            {
                tryMoveAtPath(chessBoard, pathX, pathY,
                              pathSize, i, j, L);
            }
        }
    }
    return true;
}

bool tryMoveAtPath(char chessBoard[SIZE_EIGHT][SIZE_EIGHT], int pathX[SIZE_EIGHT], int pathY[SIZE_EIGHT],
                   int pathSize, int x, int y, logic L)
{
    L.playerTurn = L.playerTurn == true ? true : false;
    L.x_sel = x;
    L.y_sel = y;

    for (int i = 0; i < pathSize; ++i)
    {
        L.x_mov = pathX[i];
        L.y_mov = pathY[i];

        if (gameRules(chessBoard, L))
        {
            return true;
        }
    }

    return false;
}