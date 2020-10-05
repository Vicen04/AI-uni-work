/*
 * GameState.h
 *
 *  Created on: Nov 21, 2018
 *      Author: Chris
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_
#include "Timer.h"
#include "Tower.h"

enum class MonsterType {
	slow, regular, fast
};

class GameState {
private:
	// This contains info about tower pricing, damage, etc
	std::map<TowerType, std::map<string, int>> towerProps;
	std::map<MonsterType, std::map<string, int>> MonsterProps;

	// Should the help menu be active
	bool helperState = true;

	int health = 50;
	int tams = 100;
	int currentWave = 1;
	int MonsterRemaining = -1;
	int MonsterEliminated = 0;
	int score = 0;

	TowerType purchaseTower = TowerType::empty;
	// This is used by the upgrade button
	Tower* boardTower = nullptr;

public:
	GameState(Timer* timer);
	virtual ~GameState();
	// This is the pixel length of a cubit
	const int cubit = 60;
	bool dirtyBit = true;
	Timer* timer;
	sf::Vector2i mousePos;

	int getScore() { return score; }
	void setScore(const int scoreIn) { score = scoreIn; }
	//Accessors
	int getHealth();
	int getTams();
	int getCurrentWave();
	void setCurrentWave(int wave);
	int getMonsterRemaining();
	int getMonsterEliminated();
	bool getHelperState();
	// This is used by the GameMenuController to determine opacity,
	// as well as determining which tower the towerController should buy
	TowerType getPurchaseTower();
	void setPurchaseTower(TowerType towerType);
	Tower* getBoardTower();
	void setBoardTower(Tower* tower);

	std::map<string, int> getTowerProps(TowerType type);
	std::map<string, int> getMonsterProps(MonsterType type);

	void startGame();				// Starts game
	void endGame();					// Ends game
	void startWave();				// Starts wave
	void updateHealthBy(int update);	// Update player lives
	void updateTamBy(int update);		// Update player currency
	void toggleHelpScreen();

	void monsterEliminated() { MonsterEliminated++; }

};

#endif /* GAMESTATE_H_ */
