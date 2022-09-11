#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define SIZE 8
#define ENTER 10

#ifdef linux
    #define SYSTEM "clear"
#elif  _WIN32
    #define SYSTEM "cls"
#elif __APPLE__
    #define SYSTEM "clear"
#endif
 
void failed_allocation() {printf("Memory allocation failed:\nError exit with code -1"); exit(-1);}

void getUserInput(int*,     int*, 
                  int*,     int*, bool);

int translateLetter(char);

int* scanBoard(int,              int, 
               int,              int,
               char[SIZE][SIZE], int*, bool*);

void executeMove(int,               int, 
                 int,               int,
                 char[SIZE][SIZE],  bool*);

void drawConsole(char[SIZE][SIZE]);

bool gameRules(int,              int,
               int,              int,
               char[SIZE][SIZE], bool);

bool pawn(int,              int,
          int,              int,
          char[SIZE][SIZE], bool);

bool rook(int,              int,
          int,              int,
          char[SIZE][SIZE], bool); 

bool knight(int,              int,
            int,              int,
            char[SIZE][SIZE], bool);

bool bishop(int,              int,
            int,              int,
            char[SIZE][SIZE], bool);

bool queen(int,              int,
           int,              int,
           char[SIZE][SIZE], bool);

bool king(int,              int,
          int,              int,
          char[SIZE][SIZE], bool);

int* check(int,               int,
            char[SIZE][SIZE], bool, bool*); 

bool targetStatus(int,  int, 
                  bool, char[8][8]); 

bool isUpperOrLower(char);
          
int main() 
{
    char chessBoard[SIZE][SIZE] = 
    {
        'R','K','B','W','Q','B','K','R',
        'P','P','P','P','P','P','P','P',
        ' ',' ',' ',' ',' ',' ',' ',' ',
        ' ',' ',' ',' ',' ',' ',' ',' ',
        ' ',' ',' ',' ',' ',' ',' ',' ',
        ' ',' ',' ',' ',' ',' ',' ',' ',
        'p','p','p','p','p','p','p','p',
        'r','k','b','w','q','b','k','r'
    };

    int x_sel = 0, y_sel = 0, x_mov = 0, y_mov = 0;
    int size = 0; 
    int* path;
    bool blocked = false;
    bool is_running = true, playerTurn = true;
    
    while(is_running) 
    {

        // Draw chess board in console. 
        drawConsole(chessBoard); 

        // Get data of next game move. 
        x_sel = 0, y_sel = 0, x_mov = 0, y_mov = 0;
        getUserInput(&x_sel, &y_sel,
                     &x_mov, &y_mov, playerTurn); 

        // Scan the chess board.
        blocked = false;
        path = scanBoard(x_sel,          y_sel,
                         x_mov,          y_mov,
                         chessBoard,     &size, &blocked);
        free(path);

        // Execute next game move. 
        if(blocked == false || chessBoard[y_sel][x_sel] == 'K' || chessBoard[y_sel][x_sel] == 'k') 
        {
            // But only if entered move is correct according to the rules.
            if(gameRules(x_sel,      y_sel,
                         x_mov,      y_mov,
                         chessBoard, playerTurn) == true) 
            {
                // Execute move and swap player turn.
                executeMove(x_sel,      y_sel,
                            x_mov,      y_mov,
                            chessBoard, &playerTurn); 
            }
        }
    }
    
    return EXIT_SUCCESS;
}

void getUserInput(int* x_sel,     int* y_sel, 
                  int* x_mov,     int* y_mov, bool playerTurn) 
{
    char* userInput = malloc(sizeof(char)); 
    if(userInput == NULL) failed_allocation();

    int key_pressed = 0, index = 0; 
    
    // Display message before input. 
    if (playerTurn == true) 
        printf("Player one enter a move:");
    else
        printf("Player two enter a move: ");
    
    // Get any character input untill ENTER is pressed. 
    while(key_pressed != ENTER) 
    {
        scanf("%c", &key_pressed); 
 
        // If not ENTER. 
        if(key_pressed != ENTER) 
        {
            userInput[index++] = (char)key_pressed;                      
            userInput = realloc(userInput, (index + 1) * sizeof(char));   
            if(userInput == NULL)  failed_allocation();
        }
        else 
            userInput[index] = '\0'; 
    }

    // Check that each index correspond to the correct format. 
    index = 0;
    while(userInput[index] != '\0') 
    {
        // Input should be of correct decimal value in the ASCII table.  
        switch (index)
        {
            case 0: // Case 0 must be of type letter.
                if(((int)userInput[index] >= 97 && (int)userInput[index] <= 104) || 
                   ((int)userInput[index] >= 65 && (int)userInput[index] <= 72)) 
                    *x_sel = translateLetter(userInput[index]);
                break;
            case 1: // Case 1 must be of type number. 
                if(userInput[index] - '0' >= 1 && userInput[index] - '0' <= 9) 
                    *y_sel = userInput[index] - '0'  - 1;
                break;
            case 2: // Case 2 must be of type letter. 
                if(((int)userInput[index] >= 97 && (int)userInput[index] <= 104) || 
                   ((int)userInput[index] >= 65 && (int)userInput[index] <= 72)) 
                    *x_mov = translateLetter(userInput[index]);
                break;
            case 3: // Case 4 must be of type number.  
                if(userInput[index] - '0' >= 1 && userInput[index] - '0' <= 9) 
                    *y_mov = userInput[index] - '0' - 1;
                break;
            default: 
                goto done;
        }
        index++;
    }

    done:
    free(userInput);
    userInput = NULL;

    return;
}

int translateLetter(char letter) 
{
    // Check if char letter correspond to any of the following:
    if(letter == 'a' || letter == 'A')
        return 0;
    else if(letter == 'b' || letter == 'B')
        return 1;
    else if(letter == 'c' || letter == 'C')
        return 2;
    else if(letter == 'd' || letter == 'D')
        return 3;
    else if(letter == 'e' || letter == 'E')
        return 4;
    else if(letter == 'f' || letter == 'F')
        return 5;
    else if(letter == 'g' || letter == 'G')
        return 6;
    else if(letter == 'h' || letter == 'H')
        return 7;

    return -1;
}

void executeMove(int x_sel,                   int y_sel, 
                 int x_mov,                   int y_mov,
                 char chessBoard[SIZE][SIZE], bool* playerTurn)
{
    // Set move target equal to select target.
    chessBoard[y_mov][x_mov] = chessBoard[y_sel][x_sel];
    
    // Selected target is set to empty. 
    chessBoard[y_sel][x_sel] = ' ';
    
    // Turn done, swap turn. 
    if(*playerTurn == true) *playerTurn = false;
    else *playerTurn = true; 

    return;
}

int* scanBoard(int x_sel,                     int y_sel, 
               int x_mov,                     int y_mov,
               char chessBoard[SIZE][SIZE],   int* size, bool* blocked) 
{   
    int index = 0, mult = 2;                                
    int* path = malloc(mult * sizeof(int));  
    if(path == NULL) failed_allocation();         

    // Loop untill selection is equal to target. 
    while(x_sel != x_mov || y_sel != y_mov)
    {   
        // Iterate x. 
        if(x_mov > x_sel) 
            x_sel++;
        else if(x_mov < x_sel)
            x_sel--;
        
        // Iterate y.
        if(y_mov > y_sel) 
            y_sel++;
        else if(y_mov < y_sel)
            y_sel--;

        // don't iterate on last part, this is instead controlled in the rules.
        if(x_sel == x_mov && y_sel == y_mov) 
            break;
        
        // Exectutes if path is not equal to empty and end target is not reached. 
        if(chessBoard[y_sel][x_sel] != ' ') 
            *blocked = true;

        // Add coordinates of each step and increase allocated data size. 
        path[index++] = y_sel;    
        path[index++] = x_sel;
        
        // increase amount of alocated bytes. 
        mult += 2;   
        path = realloc(path, mult * sizeof(int)); 
        if(path == NULL) failed_allocation();
    } 

    return path;
}

void drawConsole(char chessBoard[SIZE][SIZE]) 
{
    system(SYSTEM);

    // Print the chess board and all other display to console. 
    int board_numbers = 1; 
    for(int i = 0; i < SIZE; ++i) {      
        printf("%d.", board_numbers++);
        for(int j = 0; j < SIZE; ++j) 
        {   
            printf("[%c]", chessBoard[i][j]);
        }
        printf("\n");
    }
    printf("   A  B  C  D  E  F  G  H\n"); 

    return;
}

bool gameRules(int x_sel,                   int y_sel,
               int x_mov,                   int y_mov,
               char chessBoard[SIZE][SIZE], bool playerTurn) 
{
    // Action depending on piece being moved. 
    if(chessBoard[y_sel][x_sel] == 'P' || chessBoard[y_sel][x_sel] == 'p') 
    {
        return pawn(x_sel,      y_sel,
                    x_mov,      y_mov,
                    chessBoard, playerTurn);
    }
    else if(chessBoard[y_sel][x_sel] == 'R' || chessBoard[y_sel][x_sel] == 'r') 
    {
        return rook(x_sel,      y_sel, 
                    x_mov,      y_mov, 
                    chessBoard, playerTurn);
    }
    else if(chessBoard[y_sel][x_sel] == 'K' || chessBoard[y_sel][x_sel] == 'k') 
    {
        return knight(x_sel,      y_sel, 
                      x_mov,      y_mov, 
                      chessBoard, playerTurn);
    }
    else if(chessBoard[y_sel][x_sel] == 'B' || chessBoard[y_sel][x_sel] == 'b') 
    {
        return bishop(x_sel,      y_sel, 
                      x_mov,      y_mov, 
                      chessBoard, playerTurn);
    }
    else if(chessBoard[y_sel][x_sel] == 'Q' || chessBoard[y_sel][x_sel] == 'q') 
    {
        return queen(x_sel,      y_sel, 
                      x_mov,      y_mov, 
                      chessBoard, playerTurn);
    }
    else if(chessBoard[y_sel][x_sel] == 'W' || chessBoard[y_sel][x_sel] == 'w') 
    {
        return king(x_sel,        y_sel, 
                      x_mov,      y_mov, 
                      chessBoard, playerTurn);
    }

    return false;
}

bool pawn(int x_sel,                   int y_sel,
          int x_mov,                   int y_mov,
          char chessBoard[SIZE][SIZE], bool playerTurn) 
{

    if(playerTurn == true) {
        if(y_sel == 1 && y_mov == y_sel + 2 && x_sel == x_mov) 
        {
            if(chessBoard[y_mov][x_mov] == ' ')
                return true; 
        }
        else if(y_mov == y_sel + 1 && x_sel == x_mov) 
        {
            if(chessBoard[y_mov][x_mov] == ' ')
                return true;
        } 
        else if(y_mov == y_sel + 1 && x_sel != x_mov && chessBoard[y_mov][x_mov] != ' '){
            if(isUpperOrLower(chessBoard[y_mov][x_mov]) == false) // Is lower. 
                return true;
        }    
    }
    else if(playerTurn == false) {

        if(y_sel == 6 && y_mov == y_sel - 2 && x_sel == x_mov) 
        {
            if(chessBoard[y_mov][x_mov] == ' ')
                return true; 
        }
        else if(y_mov == y_sel - 1 && x_sel == x_mov) 
        {
            if(chessBoard[y_mov][x_mov] == ' ')
                return true;
        } 
        else if(y_mov == y_sel - 1 && x_sel != x_mov && chessBoard[y_mov][x_mov] != ' ')
        {
            if(isUpperOrLower(chessBoard[y_mov][x_mov]) == true) // Is upper. 
                return true;
        }    
    }

    return false; 
}

bool rook(int x_sel,                   int y_sel,
          int x_mov,                   int y_mov,
          char chessBoard[SIZE][SIZE], bool playerTurn) 
{
    // Vertical and horizontal movement.  
    if(x_sel > x_mov && y_sel == y_mov)
        goto next;
    else if(x_sel < x_mov && y_sel == y_mov)
        goto next;
    else if(y_sel > y_mov && x_sel == x_mov)
        goto next;
    else if(y_sel < y_mov && x_sel == x_mov)
        goto next;
    else
        return false;

    next: 
    return targetStatus(x_mov,      y_mov, 
                        playerTurn, chessBoard);
}

bool knight(int x_sel,                   int y_sel,
            int x_mov,                   int y_mov,
            char chessBoard[SIZE][SIZE], bool playerTurn)
{

    // Check move pattern, if correct return true.  
    if(y_mov == y_sel + 2 && (x_mov == x_sel + 1 || x_mov == x_sel - 1)) 
        goto next;
    else if(y_mov == y_sel - 2 && (x_mov == x_sel + 1 || x_mov == x_sel - 1))
        goto next;
    else if(x_mov == x_sel + 2 && (y_mov == y_sel + 1 || y_mov == y_sel - 1))
        goto next;
    else if(x_mov == x_sel - 2 && (y_mov == y_sel + 1 || y_mov == y_sel - 1))
        goto next;
    else 
        return false; 

    next:

    // Makes sure status of movment target is correct. 
    return targetStatus(x_mov,      y_mov,
                        playerTurn, chessBoard); 
}

bool bishop(int x_sel,                   int y_sel,
            int x_mov,                   int y_mov,
            char chessBoard[SIZE][SIZE], bool playerTurn) 
{
    int differenceX = 0, differenceY = 0; 

    // Diagonal movement. 
    if(x_sel < x_mov)
        differenceX = x_mov - x_sel;
    else 
        differenceX = x_sel - x_mov;

    if(y_sel < y_mov)
        differenceY = y_mov - y_sel;
    else 
        differenceY = y_sel - y_mov;

    if(differenceX != differenceY) 
        return false;
    
    // Makes sure status of movment target is correct. 
    return targetStatus(x_mov,      y_mov, 
                        playerTurn, chessBoard);
}

bool queen(int x_sel,                   int y_sel,
           int x_mov,                   int y_mov,
           char chessBoard[SIZE][SIZE], bool playerTurn)
{   
    int differenceX = 0, differenceY = 0; 

    // Diagonal movment. 
    if(x_sel < x_mov)
        differenceX = x_mov - x_sel;
    else 
        differenceX = x_sel - x_mov;

    if(y_sel < y_mov)
        differenceY = y_mov - y_sel;
    else 
        differenceY = y_sel - y_mov;

    if(differenceX == differenceY)
        return targetStatus(x_mov,      y_mov, 
                            playerTurn, chessBoard);

    // Vertical and horizontal movement. 
    if(x_sel > x_mov && y_sel == y_mov)
        goto next;
    else if(x_sel < x_mov && y_sel == y_mov)
        goto next;
    else if(y_sel > y_mov && x_sel == x_mov)
        goto next;
    else if(y_sel < y_mov && x_sel == x_mov)
        goto next;
    else
        return false;

    next:

    // Makes sure status of movment target is correct. 
    return targetStatus(x_mov,      y_mov, 
                        playerTurn, chessBoard);
}

bool king(int x_sel,                   int y_sel,
          int x_mov,                   int y_mov,
          char chessBoard[SIZE][SIZE], bool playerTurn)
{
    int* path;
    bool is_check = false;

    // Make sure the kings movment is never greater than 1 in any direction. 
    if(x_mov < x_sel + 2 && x_mov > x_sel - 2) 
    {
        if(y_mov < y_sel + 2 && y_mov > y_sel - 2) 
            goto next;
    }
    else 
       return false; 

    next: 

    // Make sure move doesn't result in check. 
    path = check(x_mov,      y_mov,
                chessBoard, playerTurn, &is_check);
    
    free(path);

    // Check! return false!
    if(is_check == true)
        return false; 

    // Makes sure status of movment target is correct. 
    return targetStatus(x_mov,      y_mov, 
                        playerTurn, chessBoard);
}

int* check(int x_mov,                    int  y_mov,
           char chessBoard[SIZE][SIZE],  bool playerTurn, bool* is_check) 
{
    int* path; 
    bool blocked = false; 
    int size = 0;

    // Check if location of king is under threat. 
    for(int i = 0; i < SIZE; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
        {
            if(playerTurn == true)
            {   // Try if any of player 2's pieces can move to target. 
                if(isUpperOrLower(chessBoard[i][j]) == false && chessBoard[i][j] != 'w') 
                {
                    if(gameRules(i,          j, 
                                 x_mov,      y_mov,
                                 chessBoard, false) == true) 
                    {
                        path = scanBoard(i,          j,
                                         x_mov,      y_mov, 
                                         chessBoard, &size, &blocked);
                        
                        if(blocked == false) 
                        {
                            *is_check = true;
                            return path;
                        }
                    }
                }
            }
            else 
            {   // Try if any of player 1's pieces can move to target. 
                if(isUpperOrLower(chessBoard[i][j]) == true && chessBoard[i][j] != 'W') 
                {
                    if(gameRules(i,          j, 
                                 x_mov,      y_mov,
                                 chessBoard, true) == true) 
                    {
                        path = scanBoard(i,          j,
                                         x_mov,      y_mov, 
                                         chessBoard, &size, &blocked);
                        
                        if(blocked == false) 
                        {   
                            *is_check = true;
                            return path;
                        }
                    }
                }
            }
        }
    }

    return path; 
}

bool targetStatus(int x_mov,       int y_mov,
                  bool playerTurn, char chessBoard[8][8]) 
{
    // Evaluate status of the movement target for player 1 and player 2. 
    if(playerTurn == true) 
    {
        if(chessBoard[y_mov][x_mov] == ' ' || isUpperOrLower(chessBoard[y_mov][x_mov]) == false)
            return true;
    }
    else if(playerTurn == false)
    {
        if(chessBoard[y_mov][x_mov] == ' ' || isUpperOrLower(chessBoard[y_mov][x_mov]) == true)
            return true;
    }

    return false;
}

bool isUpperOrLower(char letter) 
{
    // Possible because of ascii table value, range 101-132. 
    if(letter >= 'A' && letter <= 'Z') 
        return true;   // if upper case letter.
    else 
        return false;  // if lower case letter. 
}
