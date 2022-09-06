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

char* scanBoard(int,              int, 
                int,              int,
                char[SIZE][SIZE], bool*);

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

bool isUpperOrLower(char letter);
           
int main() {
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
    char* pieces_in_path;
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
        pieces_in_path = scanBoard(x_sel,          y_sel,
                                   x_mov,          y_mov,
                                   chessBoard,     &blocked);

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

        free(pieces_in_path);
    }
    
    return 0;
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

char* scanBoard(int x_sel,                     int y_sel, 
                int x_mov,                     int y_mov,
                char chessBoard[SIZE][SIZE],   bool* blocked) 
{   
    int found = 0;                                
    char* pieces_in_path = malloc(sizeof(char));  
    if(pieces_in_path == NULL) failed_allocation();         

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

        // Add found pieces and increase allocated data size. 
        pieces_in_path[found++] = chessBoard[y_sel][x_sel];              
        pieces_in_path = realloc(pieces_in_path, (found + 1) * sizeof(char)); 
        if(pieces_in_path == NULL) failed_allocation();
    } 

    // Update string with a null terminator. 
    pieces_in_path[found] = '\0';

    return pieces_in_path;
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
    if(playerTurn == true) 
    {
        // Rook is moving on x-axis.
        if(x_sel > x_mov && y_sel == y_mov && 
           isUpperOrLower(chessBoard[y_mov][x_mov]) != true) return true;
        else if(x_sel < x_mov && y_sel == y_mov && 
                isUpperOrLower(chessBoard[y_mov][x_mov]) != true) return true;

        // Rook is moving on y-axis.
        if(y_sel > y_mov && x_sel == x_mov && 
           isUpperOrLower(chessBoard[y_mov][x_mov]) != true) 
           return true;
        else if(y_sel < y_mov && x_sel == x_mov && 
                isUpperOrLower(chessBoard[y_mov][x_mov]) != true) return true;
    } 
    if(playerTurn == false) 
    {
        // Rook is moving on x-axis.
        if(x_sel > x_mov && y_sel == y_mov && 
           isUpperOrLower(chessBoard[y_mov][x_mov]) != false) return true;
        else if(x_sel < x_mov && y_sel == y_mov && 
                isUpperOrLower(chessBoard[y_mov][x_mov]) != false) return true;

        // Rook is moving on y-axis.
        if(y_sel > y_mov && x_sel == x_mov && 
           isUpperOrLower(chessBoard[y_mov][x_mov]) != false) return true;
        else if(y_sel < y_mov && x_sel == x_mov && 
                isUpperOrLower(chessBoard[y_mov][x_mov]) != false) return true;
    }

    return false;
}

bool knight(int x_sel,                   int y_sel,
            int x_mov,                   int y_mov,
            char chessBoard[SIZE][SIZE], bool playerTurn)
{
    
    char aKnight = ' ';
    if(playerTurn == true) aKnight = 'K';
    else aKnight = 'k'; 

    if(chessBoard[y_sel][x_sel] == aKnight) 
    {   
        // If target of move is equal to opponent or empty proceed else return false. 
        if(playerTurn == true && (isUpperOrLower(chessBoard[y_mov][x_mov]) == false || chessBoard[y_mov][x_mov] == ' '))
            goto target_is_t; 
        else if(playerTurn == false && (isUpperOrLower(chessBoard[y_mov][x_mov]) == true || chessBoard[y_mov][x_mov] == ' ')) 
            goto target_is_t;
        else return false;  

        // Move target condition was correct. 
        target_is_t: 

        // Check move pattern. If correct return true.  
        if(y_mov == y_sel + 2 && (x_mov == x_sel + 1 || x_mov == x_sel - 1)) 
            return true;
        else if(y_mov == y_sel - 2 && (x_mov == x_sel + 1 || x_mov == x_sel - 1))
            return true;
        else if(x_mov == x_sel + 2 && (y_mov == y_sel + 1 || y_mov == y_sel - 1))
            return true;
        else if(x_mov == x_sel - 2 && (y_mov == y_sel + 1 || y_mov == y_sel - 1))
            return true;
    }

    return false;
}

bool isUpperOrLower(char letter) 
{
    // Possible because of ascii table values. 
    if(letter >= 'A' && letter <= 'Z') // Range 101-132.
        return true;   // Upper case.
    else 
        return false;  // Lower case. 
}
