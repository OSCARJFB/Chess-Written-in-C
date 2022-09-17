#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 8
#define ENTER 10
#define INPUT_SIZE = 4

#ifdef linux
    #define SYSTEM "clear"
#elif  _WIN32
    #define SYSTEM "cls"
#elif __APPLE__
    #define SYSTEM "clear"
#endif

struct logic
{
    int x_sel; 
    int y_sel; 
    int x_mov; 
    int y_mov;
    bool is_running;
    bool playerTurn;
    bool blocked; 
}; 

typedef struct kingTracker 
{
    int kingX;
    int kingY;
}kingXY;

kingXY kingP1;
kingXY kingP2;

void failed_allocation() {printf("Memory allocation failed:\nError exit with code -1"); exit(-1);}
void drawConsole(char[SIZE][SIZE]);

struct logic initGame(struct logic); 
struct logic getUserInput(char[SIZE][SIZE], struct logic);
struct logic executeMove(char[SIZE][SIZE], struct logic);

int* scanBoard(char[SIZE][SIZE], int*, struct logic);
int* check(char[SIZE][SIZE], bool*, struct logic); 
int* checkmate(char[SIZE][SIZE], bool, bool*); 

bool gameRules(char[SIZE][SIZE], struct logic);
bool pawn(char[SIZE][SIZE], struct logic);
bool rook(char[SIZE][SIZE], struct logic); 
bool knight(char[SIZE][SIZE], struct logic);
bool bishop(char[SIZE][SIZE], struct logic);
bool queen(char[SIZE][SIZE], struct logic);
bool king(char[SIZE][SIZE], struct logic);
bool targetStatus(char[8][8], struct logic); 
bool isUpperOrLower(char);

int translateLetter(char);
          
int main() 
{
    struct logic L;

    L = initGame(L);

    char chessBoard[SIZE][SIZE] = 
    {
        'R','K','B','W','Q','B','K','R',
        'P','P','P','P','P','P','P','P',
        ' ',' ',' ',' ',' ',' ',' ',' ',
        ' ',' ',' ',' ',' ',' ',' ',' ',
        ' ',' ',' ',' ','W',' ',' ',' ',
        ' ',' ',' ',' ',' ',' ',' ',' ',
        'p','p','p','p','p','p','p','p',
        'r','k','b','w','q','b','k','r'
    };

    int size_of_path = 0; 
    int* path;

    while(L.is_running == true) 
    {

        // Draw chess board in console. 
        drawConsole(chessBoard); 

        // Get data of next game move. 
        //L.x_sel = 0, L.y_sel = 0, L.x_mov = 0, L.y_mov = 0;
        L = getUserInput(chessBoard, L); 

        // Scan the chess board, free returned data, since it wont be used.
        L.blocked = false;
        path = scanBoard(chessBoard, &size_of_path, L);
        free(path);

        // Execute next game move. 
        if(L.blocked == false || chessBoard[L.y_sel][L.x_sel] == 'K' || chessBoard[L.y_sel][L.x_sel] == 'k') 
        {
            // But only if entered move is correct according to the rules.
            if(gameRules(chessBoard, L) == true) 
            {
                // Execute move and swap player turn.
                L = executeMove(chessBoard, L); 
            }
        }
    }
    
    return EXIT_SUCCESS;
}

struct logic initGame(struct logic L) 
{
    L.x_sel = 0;
    L.y_sel = 0; 
    L.x_mov = 0;
    L.y_mov = 0; 
    L.is_running = true;
    L.playerTurn = true;
    return L; 
}

struct logic getUserInput(char chessBoard[SIZE][SIZE], struct logic L) 
{
    char* userInput = malloc(sizeof(char)); 
    if(userInput == NULL) failed_allocation();

    int key_pressed = 0, index = 0; 
    
    // Display message before input. 
    if (L.playerTurn == true) 
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
            if(userInput == NULL) failed_allocation();
        }
        else 
            userInput[index] = '\0'; 
    }

    // Input should be of correct length. 
    if(strlen(userInput) > 4 || strlen(userInput) < 4)
    {
        printf("\nWrong input format, it should be:\nLETTER|SINGLE DIGIT|LETTER|SINGLE DIGIT\nPress any key to continue...");
        free(userInput);
        getchar();
        return L;
    }

    // Input should be of correct decimal value using the ASCII table. 
    index = 0; 
    while(userInput[index] != '\0') 
    {
        switch (index)
        {
            case 0: 
                if(((int)userInput[index] >= 97 && (int)userInput[index] <= 104) || 
                   ((int)userInput[index] >= 65 && (int)userInput[index] <= 72)) 
                    L.x_sel = translateLetter(userInput[index]);
                break;
            case 1:
                if(userInput[index] - '0' >= 1 && userInput[index] - '0' <= 9) 
                    L.y_sel = userInput[index] - '0'  - 1;
                break;
            case 2: 
                if(((int)userInput[index] >= 97 && (int)userInput[index] <= 104) || 
                   ((int)userInput[index] >= 65 && (int)userInput[index] <= 72)) 
                    L.x_mov = translateLetter(userInput[index]);
                break;
            case 3:  
                if(userInput[index] - '0' >= 1 && userInput[index] - '0' <= 9) 
                    L.y_mov = userInput[index] - '0' - 1;
                break;
        }
        index++;
    }

    free(userInput);
    userInput = NULL;

    return L;
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

struct logic executeMove(char chessBoard[SIZE][SIZE], struct logic L)
{
    // Set move target equal to select target.
    chessBoard[L.y_mov][L.x_mov] = chessBoard[L.y_sel][L.x_sel];
    
    // Selected target is set to empty. 
    chessBoard[L.y_sel][L.x_sel] = ' ';
    
    // Turn done, swap turn. 
    if(L.playerTurn == true) L.playerTurn = false;
    else L.playerTurn = true; 

    return L;
}

int* scanBoard(char chessBoard[SIZE][SIZE], int* size_of_path, struct logic L) 
{   
    int index = 0, mult = 2;     

    int* path = malloc(mult * sizeof(int));  
    if(path == NULL) failed_allocation();         

    // Loop untill selection is equal to target. 
    while(L.x_sel != L.x_mov || L.y_sel != L.y_mov)
    {   
        // Iterate x. 
        if(L.x_mov > L.x_sel) 
            L.x_sel++;
        else if(L.x_mov < L.x_sel)
            L.x_sel--;
        
        // Iterate y.
        if(L.y_mov > L.y_sel) 
            L.y_sel++;
        else if(L.y_mov < L.y_sel)
            L.y_sel--;

        // don't iterate on last part, this is instead controlled in the rules.
        if(L.x_sel == L.x_mov && L.y_sel == L.y_mov) 
            break;
        
        // Exectutes if path is not equal to empty and end target is not reached. 
        if(chessBoard[L.y_sel][L.x_sel] != ' ') 
            L.blocked = true;

        // Add coordinates of each step and increase allocated data size. 
        path[index++] = L.y_sel;    
        path[index++] = L.x_sel;
        
        // increase amount of alocated bytes. 
        mult += 2;   
        path = realloc(path, mult * sizeof(int)); 
        if(path == NULL) failed_allocation();
    } 

    *size_of_path = index;
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

bool gameRules(char chessBoard[SIZE][SIZE], struct logic L) 
{
    printf("%d %d %d %d \n", L.y_sel,  L.x_sel, L.y_mov, L.x_mov);
    system("pause");

    // Action depending on piece being moved. 
    if(chessBoard[L.y_sel][L.x_sel] == 'P' || chessBoard[L.y_sel][L.x_sel] == 'p') 
        return pawn(chessBoard, L);
    else if(chessBoard[L.y_sel][L.x_sel] == 'R' || chessBoard[L.y_sel][L.x_sel] == 'r') 
        return rook(chessBoard, L);
    else if(chessBoard[L.y_sel][L.x_sel] == 'K' || chessBoard[L.y_sel][L.x_sel] == 'k') 
        return knight(chessBoard, L);
    else if(chessBoard[L.y_sel][L.x_sel] == 'B' || chessBoard[L.y_sel][L.x_sel] == 'b') 
        return bishop(chessBoard, L);
    else if(chessBoard[L.y_sel][L.x_sel] == 'Q' || chessBoard[L.y_sel][L.x_sel] == 'q') 
        return queen(chessBoard, L);
    else if(chessBoard[L.y_sel][L.x_sel] == 'W' || chessBoard[L.y_sel][L.x_sel] == 'w') 
        return king(chessBoard, L);

    return false;
}

bool pawn(char chessBoard[SIZE][SIZE], struct logic L) 
{
    if(L.playerTurn == true) {
        if(L.y_sel == 1 && L.y_mov == L.y_sel + 2 && L.x_sel == L.x_mov) // Start move. 
        {
            if(chessBoard[L.y_mov][L.x_mov] == ' ')
                return true; 
        }
        else if(L.y_mov == L.y_sel + 1 && L.x_sel == L.x_mov) // Regular move. 
        {
            if(chessBoard[L.y_mov][L.x_mov] == ' ')
                return true;
        } 
        else if(L.y_mov == L.y_sel + 1 && L.x_sel != L.x_mov && chessBoard[L.y_mov][L.x_mov] != ' ') // Attack move. 
        {
            if(isUpperOrLower(chessBoard[L.y_mov][L.x_mov]) == false) // Is lower. 
                return true;
        }    
    }
    else if(L.playerTurn == false) {

        if(L.y_sel == 6 && L.y_mov == L.y_sel - 2 && L.x_sel == L.x_mov) // Start move.
        {
            if(chessBoard[L.y_mov][L.x_mov] == ' ')
                return true; 
        }
        else if(L.y_mov == L.y_sel - 1 && L.x_sel == L.x_mov) // Regular move. 
        {
            if(chessBoard[L.y_mov][L.x_mov] == ' ')
                return true;
        } 
        else if(L.y_mov == L.y_sel - 1 && L.x_sel != L.x_mov && chessBoard[L.y_mov][L.x_mov] != ' ') // Attack move. 
        {
            if(isUpperOrLower(chessBoard[L.y_mov][L.x_mov]) == true) // Is upper. 
                return true;
        }    
    }

    return false; 
}

bool rook(char chessBoard[SIZE][SIZE], struct logic L) 
{
    // Vertical and horizontal movement.  
    if(L.x_sel > L.x_mov && L.y_sel == L.y_mov)
        goto next;
    else if(L.x_sel < L.x_mov && L.y_sel == L.y_mov)
        goto next;
    else if(L.y_sel > L.y_mov && L.x_sel == L.x_mov)
        goto next;
    else if(L.y_sel < L.y_mov && L.x_sel == L.x_mov)
        goto next;
    else
        return false;

    next: 
    return targetStatus(chessBoard, L);
}

bool knight(char chessBoard[SIZE][SIZE], struct logic L)
{
    // Check move pattern, if correct continue else return false.  
    if(L.y_mov == L.y_sel + 2 && (L.x_mov == L.x_sel + 1 || L.x_mov == L.x_sel - 1)) 
        goto next;
    else if(L.y_mov == L.y_sel - 2 && (L.x_mov == L.x_sel + 1 || L.x_mov == L.x_sel - 1))
        goto next;
    else if(L.x_mov == L.x_sel + 2 && (L.y_mov == L.y_sel + 1 || L.y_mov == L.y_sel - 1))
        goto next;
    else if(L.x_mov == L.x_sel - 2 && (L.y_mov == L.y_sel + 1 || L.y_mov == L.y_sel - 1))
        goto next;
    else 
        return false; 

    next:

    // Makes sure status of movment target is correct. 
    return targetStatus(chessBoard, L);
}

bool bishop(char chessBoard[SIZE][SIZE], struct logic L) 
{
    int differenceX = 0, differenceY = 0; 

    // Diagonal movement. 
    if(L.x_sel < L.x_mov)
        differenceX = L.x_mov - L.x_sel;
    else 
        differenceX = L.x_sel - L.x_mov;

    if(L.y_sel < L.y_mov)
        differenceY = L.y_mov - L.y_sel;
    else 
        differenceY = L.y_sel - L.y_mov;

    if(differenceX != differenceY) 
        return false;
    
    // Makes sure status of movment target is correct. 
    return targetStatus(chessBoard, L);
}

bool queen(char chessBoard[SIZE][SIZE], struct logic L)
{   
    int differenceX = 0, differenceY = 0; 

    // Diagonal movment. 
    if(L.x_sel < L.x_mov)
        differenceX = L.x_mov - L.x_sel;
    else 
        differenceX = L.x_sel - L.x_mov;

    if(L.y_sel < L.y_mov)
        differenceY = L.y_mov - L.y_sel;
    else 
        differenceY = L.y_sel - L.y_mov;

    if(differenceX == differenceY)
        return targetStatus(chessBoard, L);     

    // Vertical and horizontal movement. 
    if(L.x_sel > L.x_mov && L.y_sel == L.y_mov)
        goto next;
    else if(L.x_sel < L.x_mov && L.y_sel == L.y_mov)
        goto next;
    else if(L.y_sel > L.y_mov && L.x_sel == L.x_mov)
        goto next;
    else if(L.y_sel < L.y_mov && L.x_sel == L.x_mov)
        goto next;
    else
        return false;

    next:

    // Makes sure status of movment target is correct. 
    return targetStatus(chessBoard, L); 
}

bool king(char chessBoard[SIZE][SIZE], struct logic L)
{
    int* path;
    bool is_check = false;

    // Make sure the kings movment is never greater than 1 in any direction. 
    if(L.x_mov < L.x_sel + 2 && L.x_mov > L.x_sel - 2) 
    {
        if(L.y_mov < L.y_sel + 2 && L.y_mov > L.y_sel - 2) 
            goto next;
    }
    else 
       return false; 

    next: 

    // Set king temp at move pos. 
    if(L.playerTurn == true ) chessBoard[L.y_mov][L.x_mov] = 'W';
    else chessBoard[L.y_mov][L.x_mov] = 'W';

    // Make sure move doesn't result in check, free the returned value. 
    path = check(chessBoard, &is_check, L);
    free(path);

    // Remove temp.
    chessBoard[L.y_mov][L.x_mov] = ' ';

    // Check! return false!
    if(is_check == true) 
        return false; 

    // Makes sure status of movment target is correct. 
    if(targetStatus(chessBoard, L) == true) 
    {

        // Keep track of the kings new position. 
        if(L.playerTurn == true)
        {
            kingP1.kingY = L.y_mov;
            kingP1.kingX = L.x_mov;
        }
        else
        {
            kingP2.kingY = L.y_mov;
            kingP2.kingX = L.x_mov;
        }

        return true; 
    }
}

int* check(char chessBoard[SIZE][SIZE], bool* is_check, struct logic L) 
{
    int* path;
    int size_of_path = 0;
    bool blocked = false; 

    // This will store selected piece values while testing for check. 
    //int temp_selY = L.y_mov, temp_selX = L.x_sel;

    // Check if location of king is under threat.
    for(int i = 0; i < SIZE; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
        {
            if(L.playerTurn == true)
            {    
                // Try if any of player 2's pieces can move to target. 
                if(isUpperOrLower(chessBoard[i][j]) == false && chessBoard[i][j] != 'w' && chessBoard[i][j] != ' ') 
                {
                    // Values need for movement tests.
                    L.y_sel = i;
                    L.x_sel = j;
                    L.playerTurn = false; 
                    
                    // Run test by checking movement constraints. 
                    if(gameRules(chessBoard, L) == true) 
                    {
                        path = scanBoard(chessBoard, &size_of_path, L);
                        
                        if(blocked == false) 
                        {
                            *is_check = true;
                            return path;
                        }
                    }
                }
            }
            if(L.playerTurn == false)
            {   
                // Try if any of player 1's pieces can move to target. 
                if(isUpperOrLower(chessBoard[i][j]) == true && chessBoard[i][j] != 'W' && chessBoard[i][j] != ' ') 
                {
                    // Values need for movement tests.
                    L.y_sel = i;
                    L.x_sel = j;
                    L.playerTurn = true; 
                    
                    // Run test by checking movement constraints. 
                    if(gameRules(chessBoard, L) == true) 
                    {
                        path = scanBoard(chessBoard, &size_of_path, L);
                        
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

bool targetStatus(char chessBoard[8][8], struct logic L) 
{
    // Evaluate status of the movement target for player 1 and player 2. 
    if(L.playerTurn == true) 
    {
        if(chessBoard[L.y_mov][L.x_mov] == ' ' || isUpperOrLower(chessBoard[L.y_mov][L.x_mov]) == false)
            return true;
    }
    else if(L.playerTurn == false)
    {
        if(chessBoard[L.y_mov][L.x_mov] == ' ' || isUpperOrLower(chessBoard[L.y_mov][L.x_mov]) == true)
            return true;
    }

    return false;
}

bool isUpperOrLower(char letter) 
{
    // Possible because of ascii table value, range 101-132. 
    if(letter >= 'A' && letter <= 'Z') 
        return true;   // if upper case letter.
    else if(letter >= 'a' && letter <= 'z')
        return false;  // if lower case letter. 
}
