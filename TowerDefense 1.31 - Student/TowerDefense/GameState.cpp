/*
 * GameState.cpp
 *
 *  Created on: Nov 21, 2018
 *      Author: Chris
 *
 *  This class is responsible for holding all
 *  shared game state properties, allowing for
 *  communication and shared values between different
 *  controllers.
 */

#include "GameState.h"
#include "Timer.h"
#include "Tower.h"

const int FPS = 30;

GameState::GameState(Timer* timer) :
		timer(timer) {
	towerProps[TowerType::slammer] = { {"tam", 30}, {"damage", 15}, {"range", 200}, {"cooldown", (int)(1.5*FPS)}};
	towerProps[TowerType::swinger] = { {"tam", 50}, {"damage", 50}, {"range", 125}, {"cooldown", (int)(3*FPS) }};
	towerProps[TowerType::thrower] = { {"tam", 40}, {"damage", 10}, {"range", 300}, {"cooldown", (int)(1.25*FPS) }};


	MonsterProps[MonsterType::slow] = { {"tam", 1}, {"health", 60}, {"damage", 3}, {"speed", 3}};
	MonsterProps[MonsterType::regular] = { {"tam", 2}, {"health", 30}, {"damage", 2}, {"speed", 4}};
	MonsterProps[MonsterType::fast] = { {"tam", 3}, {"health", 15}, {"damage", 1}, {"speed", 5}};
	helperState = false;
}

// Game State Accessors
int GameState::getHealth() {
	return this->health;
}
int GameState::getTams() {
	return this->tams;
}
int GameState::getCurrentWave() {
	return this->currentWave;
}
int GameState::getMonsterRemaining() {
	return this->MonsterRemaining;
}
int GameState::getMonsterEliminated() {
	return this->MonsterEliminated;
}
bool GameState::getHelperState() {
	return this->helperState;
}

// Start Next Wave
void GameState::startWave() {
	this->currentWave++;
	dirtyBit = true;
}

// Update Player Lives
void GameState::updateHealthBy(int update) {
	this->health += update;
	dirtyBit = true;
}

// Update Player Currency
void GameState::updateTamBy(int update) {
	this->tams += update;
	dirtyBit = true;
}

TowerType GameState::getPurchaseTower() {
	return this->purchaseTower;
}

void GameState::setPurchaseTower(TowerType towerType) {
	this->purchaseTower = towerType;
}

Tower* GameState::getBoardTower() {
	return this->boardTower;
}
void GameState::setBoardTower(Tower* tower) {
	this->boardTower = tower;
}

void GameState::setCurrentWave(int wave) {
	this->currentWave = wave;
	dirtyBit = true;
}

std::map<string, int> GameState::getTowerProps(TowerType type) {
	return this->towerProps[type];
}

std::map<string, int> GameState::getMonsterProps(MonsterType type) {
	return this->MonsterProps[type];
}

void GameState::startGame() {
}

void GameState::endGame() {
}

void GameState::toggleHelpScreen() {
	this->helperState = !helperState;
}

GameState::~GameState() {
}

