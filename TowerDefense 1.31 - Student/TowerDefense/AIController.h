#pragma once

#include "Tower.h"

class GameController;
class GameBoard;
class Timer;
class GameState;
class GAmanager;

class AIController
{
public:
	AIController();
	~AIController();

	void setupBoard();
	int  recordScore();

	void setGameController(GameController* gameController) { m_gameController = gameController; }
	void setGameBoard(GameBoard* gameBoard) { m_gameBoard = gameBoard; }
	void setTimer(Timer* timer) { m_Timer = timer; }
	void setGameState(GameState* gameState) { m_gameState = gameState; }
	void setChromosomes();
	void update();
	void addTower(TowerType type, int gridx, int gridy);
	void gameOver();

private:
	GameController* m_gameController;
	GameBoard*		m_gameBoard;
	Timer*			m_Timer;
	GameState*		m_gameState;
	GAmanager*      m_chromosomes;
	int temp = 0, generation = 1;
	float time = 0.5f;
};

