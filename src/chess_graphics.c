/*
   Writen by: Oscar Bergström
https://github.com/OSCARJFB

MIT License
Copyright (c) 2023 Oscar Bergström
*/

#include "chess_graphics.h"

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
void drawBoard(void)
{
	int x = 0, y = 0; 
	int size = 40; 
	bool squareColor = true; 

	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			squareColor == true ? DrawRectangle(x, y, size, size, CHESS_WHITE) : DrawRectangle(x, y, size, size, CHESS_BLACK);
			x += size; 
			squareColor = squareColor == true ? false : true;

		}
		squareColor = squareColor == true ? false : true;
		y += size; 
		x = 0; 
	}
}

/**
 * This function will print all the pieces.
 */
static void drawPieces(char chessBoard[8][8])
{
	int x = 0, y = 0;
	int size = 40; 

	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			switch(chessBoard[i][j])
			{

				case 'P':
					DrawText("P", x, y, size, CHESS_P1); 
					break; 
				case 'p':
					DrawText("P", x, y, size, CHESS_P2); 
					break; 
				case 'R':
					DrawText("R", x, y, size, CHESS_P1); 
					break; 
				case 'r':
					DrawText("r", x, y, size, CHESS_P2); 
					break; 
				case 'K':
					DrawText("K", x, y, size, CHESS_P1); 
					break;
				case 'k':
					DrawText("k", x, y, size, CHESS_P2); 
					break;
				case 'B':
					DrawText("B", x, y, size, CHESS_P1); 
					break; 
				case 'b':
					DrawText("b", x, y, size, CHESS_P2); 
					break; 
				case 'Q':
					DrawText("Q", x, y, size, CHESS_P1); 
					break; 
				case 'q':
					DrawText("q", x, y, size, CHESS_P2); 
					break;
				case 'W':
					DrawText("W", x, y, size, CHESS_P1); 
					break; 
				case 'w':
					DrawText("w", x, y, size, CHESS_P2); 
					break;
			}
			x += size; 
		}
		y += size; 
		x = 0;
	}
}

static inline Vector2 readMouseClick(void)
{
	if(IsMouseButtonPressed(0))
	{
		return GetMousePosition(); 
	}

	return (Vector2){-1, -1}; // No valid selection. 
}

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

static inline void deleteChessMove(Vector2 **source, Vector2 **destination)
{
	if(*source != NULL && *destination != NULL)
	{
		free(*source);
		free(*destination); 
		*source = *destination = NULL; 
	}
}

static inline bool mouseMove(char chessBoard[8][8], move m_data)
{
	move temp_m_data = m_data;
	char temp_chessBoard[8][8]; 
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			temp_chessBoard[i][j] = chessBoard[i][j]; 
		}
	}
	temp_m_data = isPathBlocked(temp_chessBoard, temp_m_data); 
	temp_m_data = executeMove(temp_chessBoard, m_data); 	
	return m_data.playerTurn != temp_m_data.playerTurn ? true : false; 
}

static move drawAndExecuteMove(Vector2 *source, Vector2 *destination, move m_data, char chessBoard[8][8])
{
	int size = 40; 
	int x = 0, y = 0; 

	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{

			if(source != NULL)
			{
				if (source->x > x && source->x < x + size && source->y > y && source->y < y + size)
				{
					m_data.y_sel = i; 
					m_data.x_sel = j; 
					DrawRectangle(x, y, size, size, CHESS_GREEN);
				}
			}

			Vector2 mouse = GetMousePosition();
			if(source != NULL && mouse.x > x && mouse.x < x + size && mouse.y > y && mouse.y < y + size)
			{
				m_data.y_mov = i;
				m_data.x_mov = j; 
				mouseMove(chessBoard, m_data) ? DrawRectangle(x, y, size, size, CHESS_GREEN) : DrawRectangle(x, y, size, size, CHESS_RED);
			}


			if(destination != NULL)
			{
				if (destination->x > x && destination->x < x + size && destination->y > y && destination->y < y + size)
				{
					m_data.y_mov = i;
					m_data.x_mov = j; 
					m_data = isPathBlocked(chessBoard, m_data);
					m_data = castlingController(chessBoard, m_data);
					m_data = executeMove(chessBoard, m_data);
					m_data = enPassant(chessBoard, m_data);
				}
			}


			x += size; 
		}
		y += size; 
		x = 0;
	}
	return m_data; 
}

void gameLoop(char chessBoard[8][8], move m_data)
{
	Vector2 *source = NULL, *destination = NULL;
	initGraphics(); 
	while(!WindowShouldClose())
	{
		BeginDrawing();	
		drawBoard();	
		Vector2 mouseClick = readMouseClick(); 
		getMove(&source, &destination, mouseClick);
		m_data = drawAndExecuteMove(source, destination, m_data, chessBoard);
		deleteChessMove(&source, &destination);
		drawPieces(chessBoard);
		EndDrawing(); 
	}

}

