#include "Board.h"

Board::Board()
{
}

Board::Board(const Board& board)
{
	for(int row=0; row<10; row++)
	{
		for(int col=0; col<10; col++)
		{
			this->squares[row][col] = board.squares[row][col];
		}
	}
}

Square* Board::getSquare(int row, int col)
{
	return &squares[row][col];
}

Board* Board::hardCopy()
{
	Board* pBoard = new Board();

	for (int row = 0; row < 10; row++)
	{
		for (int col = 0; col < 10; col++)
		{
			pBoard->squares[row][col] = this->squares[row][col].hardCopy();
		}
	}

	return pBoard;
}
