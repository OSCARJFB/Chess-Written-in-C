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

void failed_allocation() {printf("Memory allocation failed:\nError exit with code 1"); exit(EXIT_FAILURE);}
void runGame(char[SIZE][SIZE], struct logic);
void drawConsole(char[SIZE][SIZE]);

struct logic initGame(struct logic); 
struct logic getUserInput(char[SIZE][SIZE], struct logic);
struct logic executeMove(char[SIZE][SIZE], struct logic);

int translateLetter(char);
int* scanBoard(char[SIZE][SIZE], bool*, struct logic);
bool isUpperOrLower(char);

bool checkmate(char chessBoard[SIZE][SIZE], int*, struct logic L);
bool lookForMoveAtTarget(char[SIZE][SIZE], struct logic,
                         int,              int);

bool gameRules(char[SIZE][SIZE], struct logic);
bool pawn(char[SIZE][SIZE], struct logic);
bool rook(char[SIZE][SIZE], struct logic); 
bool knight(char[SIZE][SIZE], struct logic);
bool bishop(char[SIZE][SIZE], struct logic);
bool queen(char[SIZE][SIZE], struct logic);
bool king(char[SIZE][SIZE], struct logic);
bool targetStatus(char[SIZE][SIZE], struct logic); 
       
int main(void) 
{
    struct logic L;
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

    L = initGame(L);
    runGame(chessBoard, L);

    return EXIT_SUCCESS;
}

struct logic initGame(struct logic L) 
{
    // Initialize movement variables, set game turn status equal running with player1 making the first move. 
    L.x_sel = -1;
    L.y_sel = -1; 
    L.x_mov = -1;
    L.y_mov = -1; 
    L.is_running = true;
    L.playerTurn = true;
    
    // Track kings position. 
    kingP1.kingX = 3;
    kingP1.kingY = 0;
    kingP1.kingX = 3;
    kingP1.kingY = 7;

    return L; 
}

void runGame(char chessBoard[SIZE][SIZE], struct logic L) 
{
    int* path;

    while(L.is_running == true) 
    {   
        // Draw chess board in console. 
        drawConsole(chessBoard); 

        // Get user input data. 
        L = getUserInput(chessBoard, L); 

        // Scan the chess board, free return data, since it won't be used.
        L.blocked = false;
        path = scanBoard(chessBoard, &L.blocked, L);
        free(path);

        // Execute next game move. But only if the entered move is correct according to game rules.
        if(L.blocked == false || chessBoard[L.y_sel][L.x_sel] == 'K' || chessBoard[L.y_sel][L.x_sel] == 'k') 
        {   
            // Game rules...
            if(gameRules(chessBoard, L) == true) 
            {
                // Set move "target" to kings position.
                if(L.playerTurn == true)
                {
                    if(lookForMoveAtTarget(chessBoard,   L, 
                                           kingP1.kingX, kingP1.kingY) == false)
                    {
                        L = executeMove(chessBoard, L); 
                    }
                }
                else
                {
                    if(lookForMoveAtTarget(chessBoard,   L, 
                                           kingP2.kingX, kingP2.kingY) == false)
                    {
                        L = executeMove(chessBoard, L); 
                    }
                }
            }
        }
    }
    return;
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
    
    // Game turn is done done, swap turn. 
    if(L.playerTurn == true) L.playerTurn = false;
    else L.playerTurn = true; 

    return L;
}

int* scanBoard(char chessBoard[SIZE][SIZE], bool* blocked, struct logic L) 
{   
    int index = 0, mult = 2; 
    int x = L.x_sel, y = L.y_sel;    

    int* path = malloc(mult * sizeof(int));  
    if(path == NULL) failed_allocation(); 

    // If selected piece is a knight, do special case. 
    if(chessBoard[L.y_sel ][L.x_sel]== 'K' || chessBoard[L.y_sel ][L.x_sel]== 'k')
    {
        path[index++] = y;    
        path[index] = x;
        return path;
    }        

    // Loop untill selection is equal to target. 
    while(x != L.x_mov || y != L.y_mov)
    {   
        // Add coordinates of each step and increase allocated data size. 
        path[index++] = y;    
        path[index++] = x;
        
        // increase amount of alocated bytes. 
        mult += 2;   
        path = realloc(path, mult * sizeof(int)); 
        if(path == NULL) failed_allocation();

        // Iterate x. 
        if(L.x_mov > x) 
            x++;
        else if(L.x_mov < x)
            x--;
        
        // Iterate y.
        if(L.y_mov > y) 
            y++;
        else if(L.y_mov < y)
            y--;

        // don't iterate on last part, this is instead controlled in the rules section.
        if(x == L.x_mov && y == L.y_mov) 
            break;
        
        // Execute if path is not equal to empty and end target is not reached. 
        if(chessBoard[y][x] != ' ') 
            *blocked = L.blocked = true;
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

bool gameRules(char chessBoard[SIZE][SIZE], struct logic L) 
{
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
    // Check move pattern, if it's correct, continue, else return false.  
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
    if(L.y_mov == L.y_sel - 1 && L.x_mov == L.x_sel)
        goto next;
    else if(L.y_mov == L.y_sel - 1 && L.x_mov == L.x_sel + 1)
        goto next;
    else if(L.y_mov == L.y_sel && L.x_mov == L.x_sel + 1)
        goto next;
    else if(L.y_mov == L.y_sel + 1 && L.x_mov == L.x_sel + 1)
        goto next;
    else if(L.y_mov == L.y_sel + 1 && L.x_mov == L.x_sel)
        goto next;
    else if(L.y_mov == L.y_sel + 1 && L.x_mov == L.x_sel - 1)
        goto next;
    else if(L.y_mov == L.y_sel && L.x_mov == L.x_sel - 1)
        goto next;
    else if(L.y_mov == L.y_sel - 1 && L.x_mov == L.x_sel - 1)
        goto next;
    else 
        return false; 

    next:

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
    }

    return true;
}

bool lookForMoveAtTarget(char chessBoard[SIZE][SIZE], struct logic L, 
                 int targetX,                 int targetY) 
{ 
    int* temp;

    L.y_mov = targetX;
    L.y_mov = targetY;

    for(int i = 0; i < SIZE; ++i) 
    {
        for(int j = 0; j < SIZE; ++j) 
        {
            L.y_sel = i;
            L.x_sel = j;

            if(L.playerTurn == true && isUpperOrLower(chessBoard[i][j]) == false) 
            {   
                if(gameRules(chessBoard, L) == true) 
                {
                    temp = scanBoard(chessBoard, &L.blocked,  L);
                    free(temp);

                    if(L.blocked == false)
                        return true;
                }
            } 
            else if(L.playerTurn == false && isUpperOrLower(chessBoard[i][j]) == true) 
            {
                if(gameRules(chessBoard, L) == true) 
                {
                    temp = scanBoard(chessBoard, &L.blocked, L);
                    free(temp);

                    if(L.blocked == false)
                        return true;
                }
            }
        }
    }

    free(temp);

    return false; 
}

bool targetStatus(char chessBoard[SIZE][SIZE], struct logic L) 
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
    // Check if upper or lower case, by ASCII value between the different letters. 
    if(letter >= 'A' && letter <= 'Z') 
        return true;   
    else if(letter >= 'a' && letter <= 'z')
        return false;   
}
