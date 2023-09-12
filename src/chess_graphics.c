#include "chess_graphics.h"

void initGraphics(void)
{	
	const int width = GetScreenWidth(); 
	const int height = GetScreenHeight(); 
	InitWindow(width, height, "Chess Game"); 
	SetTargetFPS(30); 
}

/**
 * Here we print a piece at a location, 
 * This is dependant on the char (piece type) int (coordinates) paramters. 
 * TODO Add sprites / models for each piece type instead using primitive symbols. 
 */

static inline void retPiece(int x, int y, char piece)
{
	Vector2 v1 = (Vector2){x, y}; 
	switch(piece)
	{
		case 'P':
		     	DrawTriangle(v1, v1, v1,  BLACK);  	
			break; 
		case 'p':
			break; 
		case 'R':
			break; 
		case 'r':
			break; 
		case 'K':
			break;
		case 'B':
			break; 
		case 'b':
			break; 
		case 'Q':
			break; 
		case 'q':
			break;
		case 'W':
			break; 
		case 'w':
			break;
	}
}

/**
 * This function will draw the board including the pieces at their location.
 */
void drawBoard(char chessBoard[8][8])
{
	int x = 0, y = 0; 
	int size = 10; 
	bool squareColor = true; 

	while(!WindowShouldClose())
	{

		BeginDrawing();	
		
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				squareColor == true ? DrawRectangle(x, y, size, size, RAYWHITE) : 
						      DrawRectangle(x, y, size, size, BLACK);
				retPiece(x / 2, y / 2, chessBoard[i][j]); 
				x += size; 
				squareColor = squareColor == true ? false : true;  
			}
			y += size; 
			x = 0; 
		}
	
		x = y = 0;
		EndDrawing(); 
	}

	CloseWindow(); 
}

/*
int main(void)
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
		
	initGraphics(); 
	drawBoard(chessBoard); 
}
*/
