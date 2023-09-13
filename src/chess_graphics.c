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
	int size = 20; 
	Color p1 = {150, 55, 0, 255}, p2 = {255, 200, 150, 255}; 
	switch(piece)
	{
		case 'P':
			DrawRectangle(x, y, size, size, p1); 
			break; 
		case 'p':
			DrawRectangle(x, y, size, size, p2); 
			break; 
		case 'R':
			DrawRectangle(x, y, size, size, p1); 
			break; 
		case 'r':
			DrawRectangle(x, y, size, size, p2); 
			break; 
		case 'K':
			DrawRectangle(x, y, size, size, p1); 
			break;
		case 'k':
			DrawRectangle(x, y, size, size, p2); 
			break;
		case 'B':
			DrawRectangle(x, y, size, size, p1); 
			break; 
		case 'b':
			DrawRectangle(x, y, size, size, p2); 
			break; 
		case 'Q':
			DrawRectangle(x, y, size, size, p1); 
			break; 
		case 'q':
			DrawRectangle(x, y, size, size, p2); 
			break;
		case 'W':
			DrawRectangle(x, y, size, size, p1); 
			break; 
		case 'w':
			DrawRectangle(x, y, size, size, p2); 
			break;
	}
}

/**
 * This function will draw the board including the pieces at their location.
 */
void drawBoard(char chessBoard[8][8])
{
	int x = 0, y = 0; 
	int size = 40; 
	bool squareColor = true; 

	while(!WindowShouldClose())
	{

		BeginDrawing();	
		
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				squareColor == true ? DrawRectangle(x, y, size, size, RAYWHITE) : DrawRectangle(x, y, size, size, BLACK);
				retPiece(x + 5, y + 5, chessBoard[i][j]); 
				x += size; 
				squareColor = squareColor == true ? false : true;  
			}
			squareColor = squareColor == true ? false : true;
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
