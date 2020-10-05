#include "Game.h"
#include "..\ChessPlayer.h"

Game::Game()
{
	gameplay = new Gameplay();
	board = new Board();
	status = new GameStatus();
	setInitialPieces(PieceColor::WHITE);
	setInitialPieces(PieceColor::BLACK);
	turn = 1;

	ChessPlayer::setupPlayers(&m_PlayerWhite, &m_PlayerBlack ,board, status, gameplay);

	int x = 1;
}

Game::~Game()
{
	for (unsigned int i = 0; i < m_vecPieces.size(); i++)
	{
		delete m_vecPieces[i];
		m_vecPieces[i] = nullptr;
	}
}

void Game::setInitialPieces(PieceColor color)
{
	int firstRow = (color==PieceColor::WHITE)? board->MIN_ROW_INDEX : board->MAX_ROW_INDEX;
	int secondRow = (color==PieceColor::WHITE)? board->MIN_ROW_INDEX+1 : board->MAX_ROW_INDEX-1;
	int firstCol = board->MIN_COL_INDEX;
	/* PAWN */
	for(int col = firstCol; col<=board->MAX_COL_INDEX; col++)
	{
		Piece* pPiece = new Piece(PieceType::PAWN, color);
		m_vecPieces.push_back(pPiece);
		board->getSquare(secondRow, col)->occupySquare(pPiece);
	}
	/* ROOK */
	Piece* pPiece = new Piece(PieceType::ROOK, color);
	m_vecPieces.push_back(pPiece);
	board->getSquare(firstRow, firstCol)->occupySquare(pPiece);

	pPiece = new Piece(PieceType::ROOK, color);
	m_vecPieces.push_back(pPiece);
	board->getSquare(firstRow, firstCol+7)->occupySquare(pPiece);

	/* KNIGHT */
	pPiece = new Piece(PieceType::KNIGHT, color);
	m_vecPieces.push_back(pPiece);
	board->getSquare(firstRow, firstCol+1)->occupySquare(pPiece);
	pPiece = new Piece(PieceType::KNIGHT, color);
	m_vecPieces.push_back(pPiece);
	board->getSquare(firstRow, firstCol+6)->occupySquare(pPiece);

	/* BISHOP */
	pPiece = new Piece(PieceType::BISHOP, color);
	m_vecPieces.push_back(pPiece);
	board->getSquare(firstRow, firstCol+2)->occupySquare(pPiece);
	pPiece = new Piece(PieceType::BISHOP, color);
	m_vecPieces.push_back(pPiece);
	board->getSquare(firstRow, firstCol+5)->occupySquare(pPiece);
	
	/* QUEEN */
	pPiece = new Piece(PieceType::QUEEN, color);
	m_vecPieces.push_back(pPiece);
	board->getSquare(firstRow, firstCol+3)->occupySquare(pPiece);
	
	/* KING */
	pPiece = new Piece(PieceType::KING, color);
	m_vecPieces.push_back(pPiece);
	board->getSquare(firstRow, firstCol+4)->occupySquare(pPiece);
}

std::stack<Move> Game::getAllLog()
{
	return log;
}

Board* Game::getBoard()
{
	return board;
}

Board* Game::getCopyOfBoard()
{
	Board* pBoard = new Board(*board);

	return pBoard;
}

Board* Game::replaceBoard(Board* newboard)
{
	Board* oldboard = board;
	board = newboard;
	return oldboard;
}

GameStatus* Game::getGameStatus()
{
	return status;
}

std::vector<Move> Game::getValidMoves(int fromRow, int fromCol)
{
	return gameplay->getValidMoves(status, board, board->getSquare(fromRow, fromCol)->getOccupyingPiece(), fromRow, fromCol);
}

bool Game::move(int fromRow, int fromCol, int toRow, int toCol)
{
	std::vector<Move> valid = getValidMoves(fromRow, fromCol);
	int sz = valid.size();
	for (int index = 0; index < sz; index++)
	{
		if (valid[index].getDestinationPosition().first == toRow && valid[index].getDestinationPosition().second == toCol)
		{
			if (gameplay->move(status, board, valid[index]))
			{
				log.push(valid[index]);
				

				// your code here
				vecPieces vpiecesBlack;
				vecPieces vpiecesWhite;

				unsigned int numPiecesBlack = m_PlayerBlack->getAllLivePieces(vpiecesBlack);
				unsigned int numPiecesWhite = m_PlayerWhite->getAllLivePieces(vpiecesWhite);
				m_PlayerBlack->SetAllRivalPieces(vpiecesWhite, numPiecesWhite);
				m_PlayerWhite->SetAllRivalPieces(vpiecesBlack, numPiecesBlack);

				if (numPiecesBlack < 6 || numPiecesWhite < 6)
				{
					m_PlayerBlack->setEndGame();
					m_PlayerWhite->setEndGame();
				}

				return true;
			}
		}
	}

	return false;
}

PieceColor Game::getPieceColor(int fromRow, int fromCol)
{
	return board->getSquare(fromRow, fromCol)->getOccupyingPiece()->getColor();
}

Piece* Game::getPiece(int fromRow, int fromCol)
{
    return board->getSquare(fromRow, fromCol)->getOccupyingPiece();
}

bool Game::isSquareOccupied(int fromRow, int fromCol)
{
    return board->getSquare(fromRow, fromCol)->occupiedState();
}

bool Game::inCheckState()
{
	return gameplay->isCheckState(status, board, getTurnColor());
}

bool Game::inCheckMateState()
{
	return gameplay->isCheckMateState(status, board, getTurnColor());
}

int Game::getTurn()
{
	return turn;
}

void Game::nextTurn()
{
	turn++;
	status->setPieceEnPassantable(getTurnColor(), NULL);
}

PieceColor Game::getTurnColor()
{
	return turn & 1? PieceColor::WHITE : PieceColor::BLACK;
}

bool Game::promote(int row, int col, PieceType type)
{
	return gameplay->pawnPromotion(board, row, col, type);
}
