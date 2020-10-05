#pragma once
#include "Chess\Gameplay.h"
#include <vector>

using namespace std;

class Piece;
class Board;
class GameStatus;
class Gameplay;
class Move;

struct PieceInPostion
{
	Piece* piece;
	int col;
	int row;
};

typedef vector<PieceInPostion> vecPieces;

struct MinMax;

class ChessPlayer
{
public:
	static void		setupPlayers(ChessPlayer** playerWhite, ChessPlayer** playerBlack, Board* pBoard, GameStatus* pGameStatus, Gameplay* pGamePlay);

	ChessPlayer(Board* pBoard, GameStatus* pGameStatus, Gameplay* pGamePlay, PieceColor colour);

	void			setAI() { m_bAI = true; }
	void            SetAllRivalPieces(vecPieces rival, int count);
	bool			isAI() { return m_bAI; }
	unsigned int	getAllLivePieces(vecPieces& vpieces);
	vector<Move>	getValidMovesForPiece(PieceInPostion pip);
	bool			chooseAIMove(Move* moveToMake);
	float           MiniMax2(MinMax node, int depth, float max, float min, bool Player);
	Move            MiniMax(MinMax node, int depth, float max, float min, bool Player);
	float           GetScore(vecPieces vPieces, vecPieces enemyPieces);
	int             GetScorebyPieces(vecPieces piece);
	int             GetScorebyPosition(vecPieces piece);
	int             GetScorebyMovesandAttack(vecPieces piece);
	int             GetScoreEndGame(vecPieces piece, vecPieces enemy);

	void			setEndGame() { m_playbook2 = false; m_playbook3 = true; }
	Move            RandomMove();

protected:
	PieceColor		getColour() { return m_colour; }

	

private:
	PieceColor	m_colour;
	Board*		m_pBoard;
	GameStatus* m_pGameStatus;
	Gameplay*	m_pGamePlay;
	Move        m_oldMove;
	bool		m_bAI, m_playbook1 = true, m_playbook2 = false, m_playbook3 = false;
	vecPieces   m_rival;
	int Pawn = 1, knight = 5, bishop = 5, rook = 10, queen = 25, king = 0, rivalPieces, initialDepth = 3, changePlaybook = 0;


	int boardPawnBlack[8][8] = { 10, 10, 10, 10, 10, 10, 10, 10,
					                5, 5, 5, 5, 5, 5, 5, 5,
				     	            4, 4, 4, 4, 4, 4, 4, 4,
					                3, 3, 3, 3, 3, 3, 3, 3,
				    	            3, 3, 3, 3, 3, 3, 3, 3,
				    	            1, 1, 1, 1, 1, 1, 1, 1,
				                    1, 1, 1, 1, 1, 1, 1, 1,
				                    1, 1, 1, 1, 1, 1, 1, 1 };

	int boardPawnWhite[8][8] = { 1, 1, 1, 1, 1, 1, 1, 1,
						         1, 1, 1, 1, 1, 1, 1, 1,
						         1, 1, 1, 1, 1, 1, 1, 1,
						         3, 3, 3, 3, 3, 3, 3, 3,
						         3, 3, 3, 3, 3, 3, 3, 3,
						         4, 4, 4, 4, 4, 4, 4, 4,
						         5, 5, 5, 5, 5, 5, 5, 5,
						     10, 10, 10, 10, 10, 10, 10, 10};
};

