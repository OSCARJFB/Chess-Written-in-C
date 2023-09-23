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
	const int width = GetScreenWidth(); 
	const int height = GetScreenHeight(); 
	InitWindow(width, height, "Chess Game"); 
	SetTargetFPS(30); 
}

/**
 * This function will draw the board including the pieces at their location.
 * At the moment it will also handle movement and game logic. This should be split later on.
 */
static inline void drawBoard(drawData dd, const int j)
{
	static bool squareColor = true; 

	squareColor == true ? DrawRectangle(dd.x, dd.y, dd.size, dd.size, CHESS_WHITE) : DrawRectangle(dd.x, dd.y, dd.size, dd.size, CHESS_BLACK);
	squareColor = squareColor == true ? false : true;
	if(j == 7)
	{
		squareColor = squareColor == true ? false : true;
	}	
}

/**
 * This function will print all the pieces.
 */
static inline void drawPieces(const char piece, drawData dd)
{
	switch(piece)
	{

		case 'P':
			DrawText("P", dd.x, dd.y, dd.size, CHESS_P1); 
			break; 
		case 'p':
			DrawText("P", dd.x, dd.y, dd.size, CHESS_P2); 
			break; 
		case 'R':
			DrawText("R", dd.x, dd.y, dd.size, CHESS_P1); 
			break; 
		case 'r':
			DrawText("r", dd.x, dd.y, dd.size, CHESS_P2); 
			break; 
		case 'K':
			DrawText("K", dd.x, dd.y, dd.size, CHESS_P1); 
			break;
		case 'k':
			DrawText("k", dd.x, dd.y, dd.size, CHESS_P2); 
			break;
		case 'B':
			DrawText("B", dd.x, dd.y, dd.size, CHESS_P1); 
			break; 
		case 'b':
			DrawText("b", dd.x, dd.y, dd.size, CHESS_P2); 
			break; 
		case 'Q':
			DrawText("Q", dd.x, dd.y, dd.size, CHESS_P1); 
			break; 
		case 'q':
			DrawText("q", dd.x, dd.y, dd.size, CHESS_P2); 
			break;
		case 'W':
			DrawText("W", dd.x, dd.y, dd.size, CHESS_P1); 
			break; 
		case 'w':
			DrawText("w", dd.x, dd.y, dd.size, CHESS_P2); 
			break;
	}
}

/**
 * Read for a mouse click, store the result of the position once the click is made in a v2. 
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
 * First add data to source then to destination. 
 */
void getMove(Vector2 **source, Vector2 **destination, Vector2 mouseClick)
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
 * Executes a fake chess move, then checks the result.
 * If the move is valid paint the square at mouse position green else paint it red.
 * TODO This function will currently not work on enpassant and castling.  
 */
static inline move mouseMove(char chessBoard[8][8], move m_data, Vector2 *source,
		drawData dd, int i, int j)
{
	if(source == NULL)
	{
		return m_data; 
	}

	Vector2 mouse = GetMousePosition();
	move temp_m_data = m_data;
	char temp_chessBoard[8][8];

	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			temp_chessBoard[i][j] = chessBoard[i][j]; 
		}
	}

	m_data.y_mov = i;
	m_data.x_mov = j;
	temp_m_data = isPathBlocked(temp_chessBoard, temp_m_data); 
	temp_m_data = executeMove(temp_chessBoard, m_data); 	
	bool isValidMove = m_data.playerTurn != temp_m_data.playerTurn ? true : false; 
	
	if(mouse.x > dd.x && mouse.x < dd.x + dd.size && mouse.y > dd.y && mouse.y < dd.y + dd.size)
	{
	 
		isValidMove ? DrawRectangle(dd.x, dd.y, dd.size, dd.size, CHESS_GREEN) : DrawRectangle(dd.x, dd.y, dd.size, dd.size, CHESS_RED);
	}
	
	return m_data; 
}

/**
 * Paint current selected chess square green and execute a chess move if valid.  
 */
static inline move drawAndExecuteMove(Vector2 *source, Vector2 *destination, move m_data, 
		char chessBoard[8][8], drawData dd, int i, int j)
{
	if(source != NULL)
	{
		if (source->x > dd.x && source->x < dd.x + dd.size && source->y > dd.y && source->y < dd.y + dd.size)
		{
			m_data.y_sel = i; 
			m_data.x_sel = j; 
			DrawRectangle(dd.x, dd.y, dd.size, dd.size, CHESS_GREEN);
		}
	}

	if(destination != NULL)
	{
		if (destination->x > dd.x && destination->x < dd.x + dd.size && destination->y > dd.y && destination->y < dd.y + dd.size)
		{
			m_data.y_mov = i;
			m_data.x_mov = j; 
			m_data = isPathBlocked(chessBoard, m_data);
			m_data = castlingController(chessBoard, m_data);
			m_data = executeMove(chessBoard, m_data);
			m_data = enPassant(chessBoard, m_data);
		}
	}

	return m_data; 
}

/**
 * Main game loop. 
 */
void gameLoop(char chessBoard[8][8], move m_data)
{
	Vector2 *source = NULL, *destination = NULL;
	drawData dd = {0, 0, 40}; 
	initGraphics(); 
	while(!WindowShouldClose())
	{
		BeginDrawing();
		deleteChessMove(&source, &destination);
		Vector2 mouseClick = readMouseClick();
		getMove(&source, &destination, mouseClick);
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				drawBoard(dd, j);	
				m_data = drawAndExecuteMove(source, destination, m_data, 
						chessBoard, dd, i, j);
				mouseMove(chessBoard, m_data, source,
						dd, i, j); 
				drawPieces(chessBoard[i][j], dd);
				dd.x += dd.size;
			}
			dd.y += dd.size; 
			dd.x = 0;
		}
		dd.y = 0; 
		EndDrawing(); 
	}
}

