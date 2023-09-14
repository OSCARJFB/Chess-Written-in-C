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

static inline Vector2 selectPiece(bool isPressed, Vector2 coordinates)
{
	if(!isPressed)
	{
		Vector2 outOfBoard = {-1, -1};
		return outOfBoard; 
	}
	return coordinates;
}

/**
 * Here we print a piece at a location, 
 * This is dependant on the char (piece type) int (coordinates) paramters. 
 * TODO Add sprites / models for each piece type instead using primitive symbols. 
 */
static inline void retPiece(int x, int y, char piece)
{
	int size = 25; ; 
	Color p1 = {150, 55, 0, 255}, p2 = {255, 200, 150, 255}; 
	switch(piece)
	{
		case 'P':
			DrawText("P", x, y, size, p1); 
			break; 
		case 'p':
			DrawText("P", x, y, size, p2); 
			break; 
		case 'R':
			DrawText("R", x, y, size, p1); 
			break; 
		case 'r':
			DrawText("r", x, y, size, p2); 
			break; 
		case 'K':
			DrawText("K", x, y, size, p1); 
			break;
		case 'k':
			DrawText("k", x, y, size, p2); 
			break;
		case 'B':
			DrawText("B", x, y, size, p1); 
			break; 
		case 'b':
			DrawText("b", x, y, size, p2); 
			break; 
		case 'Q':
			DrawText("Q", x, y, size, p1); 
			break; 
		case 'q':
			DrawText("q", x, y, size, p2); 
			break;
		case 'W':
			DrawText("W", x, y, size, p1); 
			break; 
		case 'w':
			DrawText("w", x, y, size, p2); 
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
	Color black = {0, 0, 0, 255}, white = {255, 255, 255, 255}, green = {0, 255, 0, 255};

	while(!WindowShouldClose())
	{

		BeginDrawing();	
		
		Vector2 coordinates = selectPiece(IsMouseButtonPressed(0), GetMousePosition()), currSquare = {0, 0};
		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				squareColor == true ? DrawRectangle(x, y, size, size, white) : DrawRectangle(x, y, size, size, black);
				
				currSquare.x = x, currSquare.y = y; 
				if ((coordinates.x > currSquare.x && coordinates.x < currSquare.x) &&
				   (coordinates.y > currSquare.y && coordinates.y < currSquare.y))
				{
					 DrawRectangle(x, y, size, size, green);
				}
				
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
