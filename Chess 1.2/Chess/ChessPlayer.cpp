#include "ChessPlayer.h"
#include <time.h>
#include <iostream>

using namespace std;



struct MinMax
{
	vecPieces player;
	vecPieces rival;
	Move m_move;

	MinMax(vecPieces pla, vecPieces ri)
	{

		player = pla;
		rival = ri;
	}
};

void ChessPlayer::setupPlayers(ChessPlayer** playerWhite, ChessPlayer** playerBlack, Board* pBoard, GameStatus* pGameStatus, Gameplay* pGamePlay)
{
	*playerBlack = new ChessPlayer(pBoard, pGameStatus, pGamePlay, PieceColor::BLACK);
	(*playerBlack)->setAI();
	*playerWhite = new ChessPlayer(pBoard, pGameStatus, pGamePlay, PieceColor::WHITE);
	(*playerWhite)->setAI();

	srand(time(NULL));
}

ChessPlayer::ChessPlayer(Board* pBoard, GameStatus* pGameStatus, Gameplay* pGamePlay, PieceColor colour)
{
	m_colour = colour;
	m_pBoard = pBoard;
	m_pGameStatus = pGameStatus;
	m_pGamePlay = pGamePlay;
	m_bAI = false;
}

unsigned int ChessPlayer::getAllLivePieces(vecPieces& vpieces)
{
	vpieces.clear();

	PieceInPostion pip;

	unsigned int count = 0;
	for (int i = m_pBoard->MIN_ROW_INDEX; i <= m_pBoard->MAX_ROW_INDEX; i++)
	{
		for (int j = m_pBoard->MIN_COL_INDEX; j <= m_pBoard->MAX_COL_INDEX; j++)
		{
			Piece* pPiece = m_pBoard->getSquare(i, j)->getOccupyingPiece();
			if (pPiece == nullptr)
				continue;
			if (pPiece->getColor() != m_colour)
				continue;

			count++;
			pip.piece = pPiece;
			pip.row = i;
			pip.col = j;
			vpieces.push_back(pip);
		}
	}

	return count;
}

vector<Move> ChessPlayer::getValidMovesForPiece(PieceInPostion pip)
{
	return m_pGamePlay->getValidMoves(m_pGameStatus, m_pBoard, pip.piece, pip.row, pip.col);
}

bool ChessPlayer::chooseAIMove(Move* moveToMake)
{


	// BAD AI !! - for the first piece which can move, choose the first available move

	if (m_playbook1)
	{

		if (changePlaybook == 5)
		{
			m_playbook1 = false;
			m_playbook2 = true;
			changePlaybook = 2;
		}
		else
			changePlaybook++;

		
		*moveToMake = m_oldMove = RandomMove();
			return true;
		
		
	}

	else if (m_playbook2 || m_playbook3)
	{
		vecPieces vPieces;
		getAllLivePieces(vPieces);
		MinMax node = MinMax(vPieces, m_rival);

		if (vPieces.size() > 0)
		{		
			*moveToMake  = MiniMax(node, initialDepth, INT32_MIN, INT32_MAX, true);
			if (changePlaybook > 1)
			{
				m_oldMove = *moveToMake;
				changePlaybook = 0;
			}
			changePlaybook++;
			return true;
		}
	}
	return false;
}

void ChessPlayer::SetAllRivalPieces(vecPieces rival, int count)
{
	 m_rival = rival; 
	 rivalPieces = count; 
}

Move ChessPlayer::MiniMax(MinMax node, int depth, float Max, float Min, bool player)
{
		// Return value of maximum-valued child node        
		float result = INT32_MIN;
		float temp = result;
		Move selectedMove;
		for (int i = 0; i < node.player.size(); i++)
		{
			vector<Move> move = getValidMovesForPiece(node.player[i]);
			for (int j = 0; j < move.size(); j++)
			{
				node.m_move = move.at(j);
				result = MiniMax2(node, depth - 1, Max, Min, false);
				Max = std::fmaxf(result, Max);

					if (result >= temp)
					{
						temp = result;
						selectedMove = move.at(j);
						if ((selectedMove.getMovedPiece()->getType() == m_oldMove.getMovedPiece()->getType() && node.player.size() > 1))
						{
							if (selectedMove.getDestinationPosition().first == m_oldMove.getDestinationPosition().first && selectedMove.getDestinationPosition().second == m_oldMove.getDestinationPosition().second && selectedMove.getMovedPiece()->getType() == m_oldMove.getMovedPiece()->getType())
							{
								selectedMove = RandomMove();

							}
						}
					
				}
			}
			move.clear();
		}

	return selectedMove;
}

float ChessPlayer::MiniMax2(MinMax node, int depth, float Max, float Min, bool player)
{
	float result, captured = 0.0f;
	if (initialDepth > depth)
	{
		if (node.m_move.getMovedPiece()->getColor() == this->getColour())
		{
			for (int i = 0; i < node.player.size(); i++)
			{
				if (node.player.at(i).row == node.m_move.getOriginPosition().first && node.player.at(i).col == node.m_move.getOriginPosition().second)
				{
					node.player.at(i).row = node.m_move.getDestinationPosition().first;
					node.player.at(i).col = node.m_move.getDestinationPosition().second;
					break;
				}
			}
		}
		else
		{
			for (int i = 0; i < node.rival.size(); i++)
			{
				if (node.rival.at(i).row == node.m_move.getOriginPosition().first && node.rival.at(i).col == node.m_move.getOriginPosition().second)
				{
					node.rival.at(i).row = node.m_move.getDestinationPosition().first;
					node.rival.at(i).col = node.m_move.getDestinationPosition().second;
					break;
				}
			}
		}	

		if (node.m_move.getCapturedPiece() != nullptr)
		{
			PieceInPostion pieceCaptured;
			pieceCaptured.row = node.m_move.getDestinationPosition().first;
			pieceCaptured.col = node.m_move.getDestinationPosition().second;
			pieceCaptured.piece = node.m_move.getCapturedPiece();

			if (node.m_move.getMovedPiece()->getColor() == this->getColour())
			{
				for (int i = 0; i < node.rival.size(); i++)
				{
					if (node.rival.at(i).row == pieceCaptured.row && node.rival.at(i).col == pieceCaptured.col && node.rival.at(i).piece == pieceCaptured.piece)
					{
						node.rival.erase(node.rival.begin() + i);
						captured = 0.3f;
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < node.player.size(); i++)
				{
					if (node.player.at(i).row == pieceCaptured.row && node.player.at(i).col == pieceCaptured.col && node.player.at(i).piece == pieceCaptured.piece)
					{
						node.player.erase(node.player.begin() + i);
						captured = -0.3f;
						break;
					}
				}
			}			
		}
	}
	if (depth == 0)
	{	

			if (m_playbook3)
				return  GetScoreEndGame(node.player, node.rival) + captured;
			else
				return GetScore(node.player, node.rival) + captured;		
	}

		if (!player)
		{
			// Return value of minimum-valued child node        	
			result = INT32_MAX;
		
			for (int i = 0; i < node.rival.size(); i++)
			{
				vector<Move> move = getValidMovesForPiece(node.rival[i]);
				for (int j = 0; j < move.size(); j++)
				{
					node.m_move = move.at(j);
					result = std::fminf(result, MiniMax2(node, depth - 1, Max, Min, true));
					Min = std::fminf(result, Min);
					if (Min <= Max)
						return result + captured;
				}
				move.clear();
			}
		}
		else 
		{
			// Return value of maximum-valued child node        
			result = INT32_MIN;
			float temp = result;
			for (int i = 0; i < node.player.size(); i++)
			{
				vector<Move> move = getValidMovesForPiece(node.player[i]);
				for (int j = 0; j < move.size(); j++)
				{
					node.m_move = move.at(j);
					result = MiniMax2(node, depth - 1, Max, Min, false);
					Max = std::fmaxf(result, Max);		
					if (Min <= Max)		
						return result + captured;					
				}
				move.clear();
			}

		}

		return result + captured;
}

float ChessPlayer::GetScore(vecPieces vPieces, vecPieces enemyPieces)
{
	float score1, score2, totalScore;

	score1 = GetScorebyPieces(vPieces) + GetScorebyPosition(vPieces) + GetScorebyMovesandAttack(vPieces);

	score2 = GetScorebyPosition(enemyPieces) + GetScorebyPieces(enemyPieces) + GetScorebyMovesandAttack(enemyPieces);

	totalScore = score1 / score2;

	return totalScore;
}

int  ChessPlayer::GetScorebyPieces(vecPieces pieces)
{
	int score = 0;
	for (int i = 0; i < pieces.size(); i++)
	{
		switch (pieces.at(i).piece->getType())
		{
		case PieceType::PAWN:
			score = score + Pawn;
			break;
		case PieceType::ROOK:
			score = score + rook;
			break;
		case PieceType::KNIGHT:
			score = score + knight;
			break;
		case PieceType::QUEEN:
			score = score + queen;
			break;
		case PieceType::BISHOP:
			score = score + bishop;
			break;
		}
	}
	return score;
}

int  ChessPlayer::GetScorebyPosition(vecPieces pieces)
{
	int score = 0;
	for (int i = 0; i < pieces.size(); i++)
	{
		switch (pieces.at(i).piece->getType())
		{
		case PieceType::PAWN:
			if (pieces.at(i).piece->getColor() == PieceColor::BLACK)
				score = score + boardPawnBlack[pieces.at(i).row - 1][pieces.at(i).col - 1];
			else
				score = score + boardPawnWhite[pieces.at(i).row - 1][pieces.at(i).col - 1];

			break;
		}
	}
	return score;
}
int  ChessPlayer::GetScorebyMovesandAttack(vecPieces pieces)
{
	int score = 0;
	vector<Move> move;
	for (int i = 0; i < pieces.size(); i++)
	{
		move = getValidMovesForPiece(pieces.at(i));
			score = score + move.size();
			for (int j = 0; j < move.size(); j++)
			{
				if (move.at(j).getCapturedPiece() != nullptr)
					switch (move.at(j).getCapturedPiece()->getType())
					{
					case PieceType::PAWN:
						score++;
						break;
					case PieceType::ROOK:
						score = score + 3;
						break;
					case PieceType::KNIGHT:
						score = score + 2;
						break;
					case PieceType::QUEEN:
						score = score + 5;
						break;
					case PieceType::BISHOP:
						score = score + 2;
						break;
					case PieceType::KING:
						score = score + 5;
						break;
					}
		}
	}
	return score;
}

int  ChessPlayer::GetScoreEndGame(vecPieces pieces, vecPieces enemy)
{
	int score = 0, posX, posY;
	for (int i = 0; i < pieces.size(); i++)
	{
		switch (pieces.at(i).piece->getType())
		{
		case PieceType::PAWN:
			if (pieces.at(i).piece->getColor() == PieceColor::BLACK)
				score = score + boardPawnBlack[pieces.at(i).row - 1][pieces.at(i).col - 1];
			else
				score = score + boardPawnWhite[pieces.at(i).row - 1][pieces.at(i).col - 1];
			break;
		default:
			for (int j = 0; j < enemy.size(); j++)
			{
				if (enemy.at(j).piece->getType() == PieceType::KING)
				{
					posX = enemy.at(j).row;
					posY = enemy.at(j).col;
				}
			}
			posX = posX - pieces.at(i).row;
			if (posX < 0)
				posX = -posX;
			posY = posY - pieces.at(i).col;
			if (posY < 0)
				posY = -posY;
			if (posX == 1 || posY == 1)
			{
				score--;
				break;
			}
			switch (pieces.at(i).piece->getType())
			{
			case PieceType::ROOK:
				if (posX == pieces.at(i).row || posY == pieces.at(i).col)
				{
					score = score + (14 - posX - posY);
				}
				break;

			case PieceType::KNIGHT:
				if ((posX == pieces.at(i).row + 1 && posY == pieces.at(i).col + 2) || (posX == pieces.at(i).row + 1 && posY == pieces.at(i).col + 2) || (posX == pieces.at(i).row - 1 && posY == pieces.at(i).col - 2) || (posX == pieces.at(i).row - 1 && posY == pieces.at(i).col - 2))
				{
					score = score + 10;
				}
				break;
			case PieceType::BISHOP:
				if (posX - posY == pieces.at(i).row - pieces.at(i).col || posX - posY + 2 == pieces.at(i).row - pieces.at(i).col || posX - posY + 4 == pieces.at(i).row - pieces.at(i).col || posX - posY + 6 == pieces.at(i).row - pieces.at(i).col || posX - posY - 2 == pieces.at(i).row - pieces.at(i).col || posX - posY - 4 == pieces.at(i).row - pieces.at(i).col || posX - posY - 6 == pieces.at(i).row - pieces.at(i).col)
				{
					score = score + (14 - posX - posY);
				}
				break;
			case PieceType::QUEEN:
				score = score + (14 - posX - posY);
			case PieceType::KING:
				if (enemy.size() > 1)
				    score = score + posX + posY;
				else
					score = score + (14 - posX - posY);
				break;
			}
		}
		vector<Move> move = getValidMovesForPiece(pieces.at(i));
		for (int j = 0; j < move.size(); j++)
		{
			if (move.at(j).getCapturedPiece() != nullptr && move.at(j).getMovedPiece()->getType() != PieceType::KING)
				switch (move.at(j).getCapturedPiece()->getType())
				{
				case PieceType::PAWN:
					score = score + 3;
					break;
				default:
					score = score + 5;
					break;
				}
		}
	}
	return score;
}

Move  ChessPlayer::RandomMove()
{
	vecPieces vPieces;
	getAllLivePieces(vPieces);

	bool moveAvailable = false;
	int randomPiece;
	while (!moveAvailable)
	{
		randomPiece = rand() % vPieces.size();
		vector<Move> moves = getValidMovesForPiece(vPieces[randomPiece]);
		if (moves.size() > 0)
			moveAvailable = true;
	}

	vector<Move> moves = getValidMovesForPiece(vPieces[randomPiece]);
	if (moves.size() > 0)
	{
		int field = moves.size();
		int randomMove = rand() % field;
		return moves[randomMove];
	}
}