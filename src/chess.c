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
#include "loglib.h"

void failed_allocation(void)
{
    printf("Memory allocation failed:\nError exit with return code 1");
    exit(1);
}

int main(void)
{
    struct logic L;
    char chessBoard[SIZE][SIZE] =
        {
            'R', 'K', 'B', 'Q', 'W', 'B', 'K', 'R',
            'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
            'r', 'k', 'b', 'q', 'w', 'b', 'k', 'r'};

    L = initGame(L);
    runGame(chessBoard, L);

    return 0;
}

logic initGame(logic L)
{
    L.x_sel = L.y_sel = -1;
    L.x_mov = L.y_mov = -1;

    L.is_running = L.playerTurn = true;
    L_cast.movedP1 = L_cast.movedP2 = false;

    kingP1.kingX = 3, kingP1.kingY = 0;
    kingP2.kingX = 3, kingP2.kingY = 7;

    return L;
}

void runGame(char chessBoard[SIZE][SIZE], logic L)
{
    while (L.is_running == true)
    {
        drawConsole(chessBoard);
        L = getUserInput(chessBoard, L);
        L = isPathBlocked(chessBoard, L);
        L = executeMove(chessBoard, L);
    }

    printf("\n###### Checkmate ######\n");
}

logic getUserInput(char chessBoard[SIZE][SIZE], logic L)
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

logic executeMove(char chessBoard[SIZE][SIZE], logic L)
{
    int kingX = L.playerTurn == true ? kingP1.kingX : kingP2.kingX;
    int kingY = L.playerTurn == true ? kingP1.kingY : kingP2.kingY;

    if (L.blocked == false || chessBoard[L.y_sel][L.x_sel] == 'k' || chessBoard[L.y_sel][L.x_sel] == 'K')
    {
        if (gameRules(chessBoard, L))
        {
            if (L_cast.shortCast == true || L_cast.longCast == true)
            {
                L = executeCastlingMove(chessBoard, L, kingX, kingY);
            }
            else
            {
                L = executeRegularMove(chessBoard, L, kingX, kingY);
            }
        }
    }

    return L;
}

logic executeCastlingMove(char chessBoard[SIZE][SIZE], logic L, int kingX, int kingY)
{
    char piece_in_hand = L.playerTurn == true ? 'R' : 'r';

    if (L_cast.shortCast)
    {
        chessBoard[L_cast.row][7] = ' ';
    }
    else
    {
        chessBoard[L_cast.row][0] = ' ';
    }

    chessBoard[L_cast.row][L_cast.col] = piece_in_hand;

    piece_in_hand = L.playerTurn == true ? 'W' : 'w';
    chessBoard[L.y_sel][L.x_sel] = ' ';

    piece_in_hand = L_cast.shortCast == true ? chessBoard[L_cast.row][L_cast.col + 1] : chessBoard[L_cast.row][L_cast.col - 1];

    L.playerTurn = L.playerTurn == true ? false : true;

    if (isTargetUnderThreat(chessBoard, L, kingX, kingY))
    {
        L.playerTurn = L.playerTurn == true ? false : true;
        return L;
    }

    return L;
}

logic executeRegularMove(char chessBoard[SIZE][SIZE], logic L, int kingX, int kingY)
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

logic isPathBlocked(char chessBoard[SIZE][SIZE], logic L)
{
    int index = 0;
    int x = L.x_sel, y = L.y_sel;
    int pathY[8], pathX[8];

    if (chessBoard[L.y_sel][L.x_sel] == 'K' || chessBoard[L.y_sel][L.x_sel] == 'k')
    {
        L.blocked = false;
        return L;
    }

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

void drawConsole(char chessBoard[SIZE][SIZE])
{
    int board_numbers = 1;

    system(SYSTEM);

    for (int i = 0; i < SIZE; ++i)
    {
        printf("%d.", board_numbers++);
        for (int j = 0; j < SIZE; ++j)
        {
            printf("[%c]", chessBoard[i][j]);
        }
        printf("\n");
    }
    printf("   A  B  C  D  E  F  G  H\n");
}

bool gameRules(char chessBoard[SIZE][SIZE], logic L)
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

bool pawn(char chessBoard[SIZE][SIZE], logic L)
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

bool rook(char chessBoard[SIZE][SIZE], logic L)
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
        if (L.playerTurn)
        {
            L_cast.movedP1 = true;
        }
        else
        {
            L_cast.movedP2 = true;
        }

        return true;
    }

    return false;
}

bool knight(char chessBoard[SIZE][SIZE], logic L)
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

bool bishop(char chessBoard[SIZE][SIZE], logic L)
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

bool queen(char chessBoard[SIZE][SIZE], logic L)
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

bool king(char chessBoard[SIZE][SIZE], logic L)
{
    bool moveIsOK = false;

    if (castling(chessBoard, L))
    {
        return true;
    }

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
        if (L.playerTurn)
        {
            L_cast.movedP1 = true;
            kingP1.kingY = L.y_mov;
            kingP1.kingX = L.x_mov;
        }
        else
        {
            L_cast.movedP2 = true;
            kingP2.kingY = L.y_mov;
            kingP2.kingX = L.x_mov;
        }

        return true;
    }

    return false;
}

bool castling(char chessBoard[SIZE][SIZE], logic L)
{
    const int shortC = 7, longC = 0;
    L_cast.row = L.playerTurn == true ? 0 : 7;

    if ((L.playerTurn == true && L_cast.movedP1 == false) ||
        (L.playerTurn == false && L_cast.movedP2 == false))
    {
        if (L.x_mov == shortC && L.y_mov == L_cast.row)
        {
            L_cast.col = 5;
            L_cast.shortCast = true;
            return true;
        }
        else if (L.x_mov == longC && L.y_mov == L_cast.row)
        {
            L_cast.col = 3;
            L_cast.longCast = true;
            return true;
        }
    }

    return false;
}

bool isTargetUnderThreat(char chessBoard[SIZE][SIZE], logic L, int target_x, int target_y)
{
    L.x_mov = target_x, L.y_mov = target_y;
    L.blocked = true;

    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
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

bool targetStatus(char chessBoard[SIZE][SIZE], logic L)
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
