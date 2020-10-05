#include "AIController.h"
#include "Timer.h"
#include "GameState.h"
#include <iostream>

#include "GameController.h"
#include "GAmanager.h"

using namespace std;

AIController::AIController()
{
	m_gameController = nullptr;
	m_gameBoard = nullptr;
	m_Timer = nullptr;
	m_gameState = nullptr;
	m_chromosomes = nullptr;

}

AIController::~AIController()
{
	delete m_gameController;
	delete m_gameBoard;
	delete m_Timer;
	delete m_gameState;
	delete m_chromosomes;
}

void AIController::gameOver()
{
	m_chromosomes->Update(m_gameState->getMonsterEliminated() - temp, m_Timer->elapsedSeconds() - time);
	cout << m_chromosomes->GetChromosomeIteration() << "," << m_gameState->getCurrentWave() << "," << m_gameState->getMonsterEliminated() << "," << m_gameState->getScore() << endl;
	m_chromosomes->SetCurrentChromosome();
	m_chromosomes->RestartIteration();
	temp = 0;
	time = 0.5f;
	if (m_chromosomes->CheckIfNextGen())
	{
		cout << endl;
		generation++;
		cout << "generation " << generation << endl;
		m_chromosomes->NextGeneration();
		m_chromosomes->RestartChromosomeCount();
	}
}

void AIController::update()
{
	if (m_Timer == nullptr)
		return;

	// HINT
	// a second has elapsed - your GA manager (GA Code) may decide to do something at this time...
	//GAManager::Instance()->Update(m_Timer->elapsedSeconds());
	if (m_gameState->getTams() >= m_chromosomes->GetPrice())
	{
		if (m_gameBoard->addTower(m_chromosomes->GetTower(), m_chromosomes->GetX(), m_chromosomes->GetY()))
			m_chromosomes->Update(m_gameState->getMonsterEliminated() - temp, m_Timer->elapsedSeconds() - time);
		else
		{
			m_chromosomes->randomizeCurrentChromosomeIteration();
			while (!m_chromosomes->CheckIfPossible())
				m_chromosomes->randomizeCurrentChromosomeIteration();
		}
		temp = m_gameState->getMonsterEliminated();
		time = m_Timer->elapsedSeconds();
	}
	
	// this might be useful? Monsters killed

	m_chromosomes->SaveScore(recordScore());
}

void AIController::addTower(TowerType type, int gridx, int gridy)
{
	// grid position can be from 0,0 to 25,17
	/*
	enum class TowerType {
	empty, slammer, swinger, thrower };
	*/

	m_gameBoard->addTower(type, gridx, gridy);
}

void AIController::setupBoard()
{
	m_Timer->start();
}

void AIController::setChromosomes()
{
	m_chromosomes = new GAmanager();
	m_chromosomes->SetPrice();
	cout << "iteration" << "," << "wave" << "," << "kills" << "," << "score" << endl;
}

int AIController::recordScore()
{
	int currentWave = m_gameState->getCurrentWave();
	int killCount = m_gameState->getMonsterEliminated();
	currentWave *= 10; // living longer is good
	int score = currentWave + killCount;

	m_gameState->setScore(score);

	return score;
}