/*
   Writen by: Oscar Bergström
https://github.com/OSCARJFB

MIT License
Copyright (c) 2023 Oscar Bergström
*/

#include "chess_graphics.h"

/** 
 * Basic graphics setup for the screen. 
 */
void initGraphics(void)
{	
	const int width = 600; 
	const int height = 600; 
	InitWindow(width, height, "Chess Game"); 
	SetTargetFPS(30); 
}

/**
 * This function will draw the board including the pieces at their location.
 * At the moment it will also handle movement and game logic. This should be split later on.
 */
static inline void drawBoard(drawData draw, const int j)
{
	static bool squareColor = true; 

	squareColor == true ? DrawRectangle(draw.x, draw.y, draw.size, draw.size, CHESS_WHITE) : DrawRectangle(draw.x, draw.y, draw.size, draw.size, CHESS_BLACK);
	squareColor = squareColor == true ? false : true;
	if(j == 7)
	{
		squareColor = squareColor == true ? false : true;
	}	
}

/**
 * This function will draw all the pieces.
 */
static inline void drawPieces(const char piece, drawData draw)
{
	switch(piece)
	{

		case 'P':
			DrawText("P", draw.x + 5, draw.y + 5, draw.size - 5, CHESS_P1); 
			break; 
		case 'p':
			DrawText("P", draw.x + 5, draw.y + 5, draw.size - 5, CHESS_P2); 
			break; 
		case 'R':
			DrawText("R", draw.x + 5, draw.y + 5, draw.size - 5, CHESS_P1); 
			break; 
		case 'r':
			DrawText("r", draw.x + 5, draw.y + 5, draw.size - 5, CHESS_P2); 
			break; 
		case 'K':
			DrawText("K", draw.x + 5, draw.y + 5, draw.size - 5, CHESS_P1); 
			break;
		case 'k':
			DrawText("k", draw.x + 5, draw.y + 5, draw.size - 5, CHESS_P2); 
			break;
		case 'B':
			DrawText("B", draw.x + 5, draw.y + 5, draw.size - 5, CHESS_P1); 
			break; 
		case 'b':
			DrawText("b", draw.x + 5, draw.y + 5, draw.size - 5, CHESS_P2); 
			break; 
		case 'Q':
			DrawText("Q", draw.x + 5, draw.y + 5, draw.size - 5, CHESS_P1); 
			break; 
		case 'q':
			DrawText("q", draw.x + 5, draw.y + 5, draw.size - 5, CHESS_P2); 
			break;
		case 'W':
			DrawText("W", draw.x + 5, draw.y + 5, draw.size - 5, CHESS_P1); 
			break; 
		case 'w':
			DrawText("w", draw.x + 5, draw.y + 5, draw.size - 5, CHESS_P2); 
			break;
	}
}

/**
 * Read for a mouse click, store the result of the position once a click is made in a v2. 
 */
static inline Vector2 readMouseClick(void)
{
	if(IsMouseButtonPressed(0))
	{
		return GetMousePosition(); 
	}

	return (Vector2){-1, -1}; // No valid selection. 
}

/**
 * Get mouse position on click, store the value in a v2 pointer.
 * First adraw data to source then to destination. 
 */
static void getMove(Vector2 **source, Vector2 **destination, Vector2 mouseClick)
{
	if(mouseClick.x == -1 && mouseClick.y == -1)
	{
		return; 
	}

	if(*source == NULL)
	{
		*source = malloc(sizeof(Vector2)); 
		if(*source == NULL)
		{
			return; 
		}
		(*source)->x = mouseClick.x; 
		(*source)->y = mouseClick.y;
		return; 
	}

	if(*destination == NULL)
	{
		*destination = malloc(sizeof(Vector2)); 
		if(*destination == NULL)
		{
			return; 
		}
		(*destination)->x = mouseClick.x; 
		(*destination)->y = mouseClick.y;
	}
}

/**
 * Once a move is done, free allocataded space and set the pointer to NULL. 
 */
static void deleteChessMove(Vector2 **source, Vector2 **destination)
{
	if(*source != NULL && *destination != NULL)
	{
		free(*source);
		free(*destination); 
		*source = *destination = NULL; 
	}
}

/**
 * Creates a 2d copy of the chessBoard.
 */
static inline void copyBoard(char chessBoard[8][8], char chessBoardcpy[8][8])
{
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			chessBoardcpy[i][j] = chessBoard[i][j];
		}
	}
}

/**
 * Executes a fake chess move, then checks the result.
 * If the move is valid paint the square at mouse position green else paint it red.
 */
static move mouseMove(char chessBoard[8][8], move mv, drawData draw)
{
	Vector2 mouse = GetMousePosition();
	char chessBoardcpy[8][8];

	copyBoard(chessBoard, chessBoardcpy); 
	move cpyMv = mv;

	cpyMv = isPathBlocked(chessBoardcpy, cpyMv); 
	cpyMv = executeMove(chessBoardcpy, cpyMv); 	

	bool isValidMove = mv.playerTurn != cpyMv.playerTurn ? true : false; 
	if(mouse.x > draw.x && mouse.x < draw.x + draw.size && mouse.y > draw.y && mouse.y < draw.y + draw.size)
	{

		isValidMove ? DrawRectangle(draw.x, draw.y, draw.size, draw.size, CHESS_GREEN) : DrawRectangle(draw.x, draw.y, draw.size, draw.size, CHESS_RED);
	}

	return mv; 
}

/**
 * Paint current selected chess square green and execute a chess move if valid.  
 */
static move drawAndExecuteMove(Vector2 *source, Vector2 *destination, move mv, 
		char chessBoard[8][8], drawData draw, int i, int j)
{
	if(source == NULL)
	{
		return mv; 
	}

	if (source->x > draw.x && source->x < draw.x + draw.size && 
			source->y > draw.y && source->y < draw.y + draw.size)
	{
		mv.y_sel = i; 
		mv.x_sel = j; 
		DrawRectangle(draw.x, draw.y, draw.size, draw.size, CHESS_GREEN);
	}

	if(mv.x_sel >= 0 && mv.x_sel <= 7 && mv.y_sel >= 0 && mv.y_sel <= 7)
	{
		mv.y_mov = i;
		mv.x_mov = j;
		mouseMove(chessBoard, mv, draw); 
	}

	if(destination != NULL) 
	{
		if (destination->x > draw.x && destination->x < draw.x + draw.size &&
				destination->y > draw.y && destination->y < draw.y + draw.size)
		{
			mv.y_mov = i;
			mv.x_mov = j; 
			mv = isPathBlocked(chessBoard, mv);
			mv = castlingController(chessBoard, mv);
			mv = executeMove(chessBoard, mv);
			mv = enPassant(chessBoard, mv);
		}
	}

	return mv; 
}

/**
 * This is the actual game loop.
 * Here the graphics are drawn and user input is handled.  
 */
static void gameLoop(void)
{
	char chessBoard[8][8] =

	{
		{'R', 'K', 'B', 'Q', 'W', 'B', 'K', 'R'},
		{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
		{'r', 'k', 'b', 'q', 'w', 'b', 'k', 'r'}
	};

	move mv = initMove(); 
	Vector2 *source = NULL, *destination = NULL;
	drawData draw = {0, 0, 75}; 

	while(!WindowShouldClose()) 
	{
		BeginDrawing();
		
		ClearBackground(CHESS_BLACK);

		deleteChessMove(&source, &destination);
		Vector2 mouseClick = readMouseClick();
		getMove(&source, &destination, mouseClick);

		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				drawBoard(draw, j);	
				mv = drawAndExecuteMove(source, destination, 
						mv, chessBoard, draw, i, j);
				drawPieces(chessBoard[i][j], draw);
				draw.x += draw.size;
			}
			draw.y += draw.size; 
			draw.x = 0;
		}
		draw.y = 0;
		 
		EndDrawing(); 
	}

	free(source); 
	free(destination); 
	source = destination = NULL;
}

/**
 * Here are the graphics initialized and the main menu rendered. 
 */
void mainMenu(void)
{
	initGraphics(); 

	bool donePlaying = false; 
	while(!donePlaying && !WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(CHESS_BLACK);

		DrawText("1. play", 200, 200, 30, CHESS_GREEN); 
		DrawText("2. console", 200, 230, 30, CHESS_GREEN); 
		DrawText("3. Exit", 200, 260, 30, CHESS_GREEN);

		switch(GetKeyPressed())
		{
			case play: 
				gameLoop();
				break;
			case console:
				runFromConsole();
				break;
			case exitGame:
				donePlaying = true; 
				break; 
		}

		EndDrawing(); 
	}
}
