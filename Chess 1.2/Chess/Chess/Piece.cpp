#include "Piece.h"
#include <iostream>

Piece::Piece(PieceType pType, PieceColor pColor)
{
	type = pType;
	color = pColor;
}

Piece::Piece(const Piece &piece)
{
	this->type = piece.type;
	this->color = piece.color;
}

Piece::~Piece()
{
	std::cout << "Piece Deleted" << std::endl;
}

PieceType Piece::getType()
{
	return type;
}

PieceColor Piece::getColor()
{
	return color;
}