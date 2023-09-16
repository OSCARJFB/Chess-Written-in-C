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
static inline void drawPiece(int x, int y, char piece)
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
 * At the moment it will also handle movement and game logic. This should be split later on.
 */
void drawBoard(char chessBoard[8][8], move m_data)
{
	int x = 0, y = 0; 
	int size = 40; 
	bool squareColor = true, is_Selected = false;
	Vector2 s_coordinates = {-1, -1}, m_coordinates = {-1, -1};
	Color black = {0, 0, 0, 255}, white = {255, 255, 255, 255}, green = {0, 255, 0, 255};

	while(!WindowShouldClose())
	{
		BeginDrawing();	

		if(IsMouseButtonPressed(0) && !is_Selected)
		{
			s_coordinates = GetMousePosition(); 
		}	

		if(IsMouseButtonPressed(0) && is_Selected)
		{
			m_coordinates = GetMousePosition();	
		}

		for(int i = 0; i < 8; ++i)
		{
			for(int j = 0; j < 8; ++j)
			{
				squareColor == true ? DrawRectangle(x, y, size, size, white) : DrawRectangle(x, y, size, size, black);

				if (s_coordinates.x > x && s_coordinates.x < x + size &&
						s_coordinates.y > y && s_coordinates.y < y + size)
				{	
					m_data.y_sel = i; 
					m_data.x_sel = j; 
					DrawRectangle(x, y, size, size, green);
					is_Selected = true; 
				}

				drawPiece(x + 5, y + 5, chessBoard[i][j]); 

				if (is_Selected && m_coordinates.x > x && m_coordinates.x < x + size &&
						m_coordinates.y > y && m_coordinates.y < y + size)
				{
					m_data.y_mov = i;
					m_data.x_mov = j; 
					m_data = isPathBlocked(chessBoard, m_data);
					m_data = castlingController(chessBoard, m_data);
					m_data = executeMove(chessBoard, m_data);
					m_data = enPassant(chessBoard, m_data);
					s_coordinates.x = s_coordinates.y = m_coordinates.x = m_coordinates.y = -1; 
					is_Selected = false; 
				}

				x += size; 
				squareColor = squareColor == true ? false : true;

			}
			squareColor = squareColor == true ? false : true;
			y += size; 
			x = 0; 
		}

		if (checkmate(chessBoard, m_data))
		{

			DrawText("Checkmate", x, (y + 1) * size, size, white);
			DrawText("Would you like to play again? (y/n)", x, (y + 2) * size, size, white);
			// do something here
		}

		x = y = 0;
		EndDrawing(); 
	}

	CloseWindow(); 
}
