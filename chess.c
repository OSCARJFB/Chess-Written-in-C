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

struct logic initGame(logic L)
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
        scanBoard(chessBoard, L, &L.blocked, false, NULL);
        L = executeMove(chessBoard, L);
        if (checkmate(chessBoard, L) == true)
        {
            break;
        }
    }

    printf("\n###### Checkmate ######\n");
    return;
}

struct logic getUserInput(char chessBoard[SIZE][SIZE], logic L)
{
    char *userInput = malloc(sizeof(char));
    if (userInput == NULL)
    {
        failed_allocation();
    }

    int key_pressed = 0, sizeOfArray = 0;

    L.playerTurn == true ? printf("Player one enter a move:") : printf("Player two enter a move:");

    // Get any character until ENTER key is pressed.
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

    // Make sure the input is of correct length.
    if (strlen(userInput) > 4 || strlen(userInput) < 4)
    {
        printf("\nWrong input format, it should be:\n[LETTER][SINGLE DIGIT][LETTER][SINGLE DIGIT]\nPress any key to continue...");
        free(userInput);
        getchar();
        return L;
    }

    // Input should be of correct decimal value using the ASCII table.
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

struct logic executeMove(char chessBoard[SIZE][SIZE], logic L)
{
    char piece_in_hand = ' ';
    int kingX = L.playerTurn == true ? kingP1.kingX : kingP2.kingX;
    int kingY = L.playerTurn == true ? kingP1.kingY : kingP2.kingY;

    // A move can't be blocked unless the selected piece is of type kight.
    if (L.blocked == false || chessBoard[L.y_sel][L.x_sel] == 'k' || chessBoard[L.y_sel][L.x_sel] == 'K')
    {
        // Rules constrainsts can't be broken.
        if (gameRules(chessBoard, L) == true)
        {
            // Since the gamerules were not broken, either do a castling or execute a regular move.
            if (L_cast.shortCast == true || L_cast.longCast == true)
            {
                // Move rook.
                piece_in_hand = L.playerTurn == true ? 'R' : 'r';
                (L_cast.shortCast == true) ? (chessBoard[L_cast.row][7] = ' ') : (chessBoard[L_cast.row][0] = ' ');
                chessBoard[L_cast.row][L_cast.col] = piece_in_hand;

                // Move King.
                piece_in_hand = L.playerTurn == true ? 'W' : 'w';
                chessBoard[L.y_sel][L.x_sel] = ' ';
                (L_cast.shortCast == true) ? (chessBoard[L_cast.row][L_cast.col + 1] = piece_in_hand) : (chessBoard[L_cast.row][L_cast.col - 1] = piece_in_hand);

                // King can't be in check.
                L.playerTurn = L.playerTurn == true ? false : true;
                if (lookForMoveAtTarget(chessBoard, L,
                                        kingX, kingY,
                                        NULL, NULL) == false)
                    return L;
                {
                    L.playerTurn = L.playerTurn == false ? true : false;
                    piece_in_hand = L.playerTurn == true ? 'R' : 'r';
                    (L_cast.shortCast == true) ? (chessBoard[L_cast.row][7] = piece_in_hand) : (chessBoard[L_cast.row][0] = piece_in_hand);

                    piece_in_hand = L.playerTurn == true ? 'W' : 'w';
                    chessBoard[L.y_sel][L.x_sel] = piece_in_hand;
                }
            }
            else
            {
                // Make move.
                piece_in_hand = chessBoard[L.y_mov][L.x_mov];
                chessBoard[L.y_mov][L.x_mov] = chessBoard[L.y_sel][L.x_sel];
                chessBoard[L.y_sel][L.x_sel] = ' ';

                // King can't be in check.
                L.playerTurn = L.playerTurn == true ? false : true;
                if (lookForMoveAtTarget(chessBoard, L,
                                        kingX, kingY,
                                        NULL, NULL) == false)
                    return L;
                {
                    L.playerTurn = L.playerTurn == false ? true : false;
                    chessBoard[L.y_sel][L.x_sel] = chessBoard[L.y_mov][L.x_mov];
                    chessBoard[L.y_mov][L.x_mov] = piece_in_hand;
                }
            }
        }
    }

    return L;
}

int translateLetter(char letter)
{
    // Look if a letter correspond to any of the following:
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

int *scanBoard(char chessBoard[SIZE][SIZE], logic L,
               bool *blocked, bool allocateMem, int *sizeOfArray)
{
    int bytes_to_be_allocated = 2, index = 0;
    int x = L.x_sel, y = L.y_sel;
    int *path;

    *blocked = false;

    path = malloc(bytes_to_be_allocated * sizeof(int));
    if (path == NULL)
    {
        failed_allocation();
    }

    // If the selected piece is a knight, it's a special case.
    if (chessBoard[L.y_sel][L.x_sel] == 'K' || chessBoard[L.y_sel][L.x_sel] == 'k')
    {
        if (allocateMem == true)
        {
            return path;
        }
        else
        {
            return NULL;
        }
    }

    // Loop and scan the board untill selection is equal to target.
    while (x != L.x_mov || y != L.y_mov)
    {
        path[index++] = y;
        path[index++] = x;

        bytes_to_be_allocated += 2;
        path = realloc(path, bytes_to_be_allocated * sizeof(int));
        if (path == NULL)
        {
            failed_allocation();
        }

        if (L.x_mov > x)
        {
            x++;
        }
        else if (L.x_mov < x)
        {
            x--;
        }

        if (L.y_mov > y)
        {
            y++;
        }
        else if (L.y_mov < y)
        {
            y--;
        }
    }

    // Is the scanned path blocked?
    for (int i = 0; i < index; i += 2)
    {
        if (chessBoard[path[i]][path[i + 1]] != ' ' && i != 0 & i != index)
            *blocked = true;
    }

    // Action taken depending on request for allocated memory.
    if (allocateMem == true && sizeOfArray != NULL)
    {
        *sizeOfArray = index;
        return path;
    }
    else
    {
        free(path);
        return NULL;
    }
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
    // Action depending on piece being moved.
    if ((L.playerTurn == true && isUpperOrLower(chessBoard[L.y_sel][L.x_sel]) == true) ||
        (L.playerTurn == false && isUpperOrLower(chessBoard[L.y_sel][L.x_sel]) == false))
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
    if (L.playerTurn == true)
    {
        if (L.y_sel == 1 && L.y_mov == L.y_sel + 2 && L.x_sel == L.x_mov) // Start move.
        {
            if (chessBoard[L.y_mov][L.x_mov] == ' ')
            {
                return true;
            }
        }
        else if (L.y_mov == L.y_sel + 1 && L.x_sel == L.x_mov) // Regular move.
        {
            if (chessBoard[L.y_mov][L.x_mov] == ' ')
            {
                return true;
            }
        }
        else if (L.y_mov == L.y_sel + 1 &&
                 (L.x_mov == L.x_sel + 1 || L.x_mov == L.x_sel - 1) &&
                 chessBoard[L.y_mov][L.x_mov] != ' ') // Attack move.
        {
            if (isUpperOrLower(chessBoard[L.y_mov][L.x_mov]) == false) // Is lower.
            {
                return true;
            }
        }
    }
    else if (L.playerTurn == false)
    {

        if (L.y_sel == 6 && L.y_mov == L.y_sel - 2 && L.x_sel == L.x_mov) // Start move.
        {
            if (chessBoard[L.y_mov][L.x_mov] == ' ')
            {
                return true;
            }
        }
        else if (L.y_mov == L.y_sel - 1 && L.x_sel == L.x_mov) // Regular move.
        {
            if (chessBoard[L.y_mov][L.x_mov] == ' ')
            {
                return true;
            }
        }
        else if (L.y_mov == L.y_sel - 1 && L.x_sel != L.x_mov &&
                 (L.x_mov == L.x_sel + 1 || L.x_mov == L.x_sel - 1) &&
                 chessBoard[L.y_mov][L.x_mov] != ' ') // Attack move.
        {
            if (isUpperOrLower(chessBoard[L.y_mov][L.x_mov]) == true) // Is upper.
            {
                return true;
            }
        }
    }

    return false;
}

bool rook(char chessBoard[SIZE][SIZE], logic L)
{
    // Vertical and horizontal movement.
    if (L.x_sel > L.x_mov && L.y_sel == L.y_mov)
    {
        goto next;
    }
    else if (L.x_sel < L.x_mov && L.y_sel == L.y_mov)
    {
        goto next;
    }
    else if (L.y_sel > L.y_mov && L.x_sel == L.x_mov)
    {
        goto next;
    }
    else if (L.y_sel < L.y_mov && L.x_sel == L.x_mov)
    {
        goto next;
    }
    else
    {
        return false;
    }

next:

    if (targetStatus(chessBoard, L) == true)
    {
        (L.playerTurn == true) ? (L_cast.movedP1 = true) : (L_cast.movedP2 = true);
        return true;
    }
}

bool knight(char chessBoard[SIZE][SIZE], logic L)
{
    // Check move pattern, if it's correct, continue, else return false.
    if (L.y_mov == L.y_sel + 2 && (L.x_mov == L.x_sel + 1 || L.x_mov == L.x_sel - 1))
    {
        goto next;
    }
    else if (L.y_mov == L.y_sel - 2 && (L.x_mov == L.x_sel + 1 || L.x_mov == L.x_sel - 1))
    {
        goto next;
    }
    else if (L.x_mov == L.x_sel + 2 && (L.y_mov == L.y_sel + 1 || L.y_mov == L.y_sel - 1))
    {
        goto next;
    }
    else if (L.x_mov == L.x_sel - 2 && (L.y_mov == L.y_sel + 1 || L.y_mov == L.y_sel - 1))
    {
        goto next;
    }
    else
    {
        return false;
    }

next:

    return targetStatus(chessBoard, L);
}

bool bishop(char chessBoard[SIZE][SIZE], logic L)
{
    int differenceX = 0, differenceY = 0;

    // Diagonal movement.
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

    // Diagonal movment.
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

    // Vertical and horizontal movement.
    if (L.x_sel > L.x_mov && L.y_sel == L.y_mov)
    {
        goto next;
    }
    else if (L.x_sel < L.x_mov && L.y_sel == L.y_mov)
    {
        goto next;
    }
    else if (L.y_sel > L.y_mov && L.x_sel == L.x_mov)
    {
        goto next;
    }
    else if (L.y_sel < L.y_mov && L.x_sel == L.x_mov)
    {
        goto next;
    }
    else
    {
        return false;
    }

next:

    return targetStatus(chessBoard, L);
}

bool king(char chessBoard[SIZE][SIZE], logic L)
{
    if (castling(chessBoard, L) == true)
        return true;

    if (L.y_mov == L.y_sel - 1 && L.x_mov == L.x_sel)
    {
        goto next;
    }
    else if (L.y_mov == L.y_sel - 1 && L.x_mov == L.x_sel + 1)
    {
        goto next;
    }
    else if (L.y_mov == L.y_sel && L.x_mov == L.x_sel + 1)
    {
        goto next;
    }
    else if (L.y_mov == L.y_sel + 1 && L.x_mov == L.x_sel + 1)
    {
        goto next;
    }
    else if (L.y_mov == L.y_sel + 1 && L.x_mov == L.x_sel)
    {
        goto next;
    }
    else if (L.y_mov == L.y_sel + 1 && L.x_mov == L.x_sel - 1)
    {
        goto next;
    }
    else if (L.y_mov == L.y_sel && L.x_mov == L.x_sel - 1)
    {
        goto next;
    }
    else if (L.y_mov == L.y_sel - 1 && L.x_mov == L.x_sel - 1)
    {
        goto next;
    }
    else
    {
        return false;
    }

next:

    if (targetStatus(chessBoard, L) == true)
    {
        if (L.playerTurn == true)
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
    }

    return true;
}

bool castling(char chessBoard[SIZE][SIZE], logic L)
{
    const int shortC = 7, longC = 0;
    L_cast.row = L.playerTurn == true ? 0 : 7;

    // Determines if a short or long cast is possible.
    if ((L.playerTurn == true && L_cast.movedP1 == false) ||
        (L.playerTurn == false && L_cast.movedP2 == false))
    {
        // If castling was possible make sure the user move was correct.
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

bool lookForMoveAtTarget(char chessBoard[SIZE][SIZE], logic L,
                         int targetX, int targetY, int *attackerX, int *attackerY)
{
    L.x_mov = targetX, L.y_mov = targetY;
    L.blocked = true;

    // Scan the chess board an look if a move is possible against the target.
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            L.y_sel = i, L.x_sel = j;

            // Will only pick pieces of type lower(player one) or upper(player two) and then make a move against the target.
            if (L.playerTurn == true && isUpperOrLower(chessBoard[i][j]) == true && chessBoard[i][j] != ' ')
            {
                if (gameRules(chessBoard, L) == true)
                {
                    scanBoard(chessBoard, L, &L.blocked, false, NULL);

                    if (L.blocked == false)
                    {
                        if (attackerX != NULL && attackerY != NULL)
                        {
                            *attackerY = L.y_sel, *attackerX = L.x_sel;
                            return true;
                        }
                        else
                        {
                            return true;
                        }
                    }
                }
            }
            else if (L.playerTurn == false && isUpperOrLower(chessBoard[i][j]) == false && chessBoard[i][j] != ' ')
            {
                if (gameRules(chessBoard, L) == true)
                {
                    scanBoard(chessBoard, L, &L.blocked, false, NULL);

                    if (L.blocked == false)
                    {
                        if (attackerX != NULL && attackerY != NULL)
                        {
                            *attackerY = L.y_sel, *attackerX = L.x_sel;
                            return true;
                        }
                        else
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool targetStatus(char chessBoard[SIZE][SIZE], logic L)
{
    // Evaluate status of the movement target for player 1 and player 2.
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
    // Try if letter upper(true) or lower(false) case.
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

bool checkmate(char chessBoard[SIZE][SIZE], logic L)
{
    // Will get the kings position.
    int x = 0, y = 0;
    int sizeOfArray = 0;
    int attackerX = 0, attackerY = 0;

    L.x_sel = L.x_mov, L.y_sel = L.y_mov;

    // Scan if kings is in check or not.
    if (L.playerTurn == true)
    {
        L.playerTurn = false;
        if (lookForMoveAtTarget(chessBoard, L,
                                kingP1.kingX, kingP1.kingY,
                                &attackerX, &attackerY) == false)
        {
            return false;
        }

        y = kingP1.kingY, x = kingP1.kingX;
    }
    else
    {
        L.playerTurn = true;
        if (lookForMoveAtTarget(chessBoard, L,
                                kingP2.kingX, kingP2.kingY,
                                &attackerX, &attackerY) == false)
        {
            return false;
        }

        y = kingP2.kingY, x = kingP2.kingX;
    }

    // Scan the kings surroundings for a possiblity to move out of danger.
    for (int i = 0; i < SIZE; ++i)
    {
        switch (i)
        {
        case 0:
            y--;
            break;
        case 1:
            x++;
            break;
        case 2:
            y++;
            break;
        case 3:
            y++;
            break;
        case 4:
            x--;
            break;
        case 5:
            x--;
            break;
        case 6:
            y--;
            break;
        case 7:
            y--;
            break;
        }

        if (L.playerTurn == false)
        {
            if ((chessBoard[y][x] == ' ' || isUpperOrLower(chessBoard[y][x]) == false) &&
                y >= 0 && y <= 7 && x >= 0 && x <= 7)
            {
                if (lookForMoveAtTarget(chessBoard, L, x, y, NULL, NULL) == false)
                    return false;
            }
        }
        else
        {
            if ((chessBoard[y][x] == ' ' || isUpperOrLower(chessBoard[y][x]) == false) &&
                y >= 0 && y <= 7 && x >= 0 && x <= 7)
            {
                if (lookForMoveAtTarget(chessBoard, L, x, y, NULL, NULL) == false)
                    return false;
            }
        }
    }

    x = x + 1, y = y + 1;

    L.x_sel = attackerX, L.y_sel = attackerY;
    L.x_mov = x, L.y_mov = y;

    L.playerTurn = L.playerTurn == true ? false : true;

    // Get attackers path to the target.
    int *path = scanBoard(chessBoard, L, &L.blocked, true, &sizeOfArray);

    // Finally, see if the threat can be removed or blocked.
    chessBoard[L.y_mov][L.x_mov] = ' ';
    for (int i = 0; i < sizeOfArray; i += 2)
    {
        if (lookForMoveAtTarget(chessBoard, L, path[i + 1], path[i], &attackerX, &attackerY) == true)
        {
            (L.playerTurn == true) ? (chessBoard[L.y_mov][L.x_mov] = 'W') : (chessBoard[L.y_mov][L.x_mov] = 'w');

            free(path);
            drawConsole(chessBoard);
            return false;
        }
    }
    (L.playerTurn == true) ? (chessBoard[L.y_mov][L.x_mov] = 'W') : (chessBoard[L.y_mov][L.x_mov] = 'w');

    free(path);
    drawConsole(chessBoard);

    return true;
}
