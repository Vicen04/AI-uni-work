/*
 * MonsterController.cpp
 *
 *  Created on: Nov 22, 2018
 *      Author: Chris
 */

#include "MonsterController.h"
#include "Monster.h"
#include <iostream>
#include <algorithm> // remove and remove_if
#include <string>
#include <memory>

using namespace std;
using std::string;
using std::shared_ptr;

MonsterController::MonsterController(sf::RenderWindow* _window,
		GameState* _gameState, const std::vector<sf::Vector2f> _path) :
		window(_window), gameState(_gameState), pathInCubits(_path) {
	MonsterVec = new std::vector<shared_ptr<Monster>>;
	modifier = 1;
	counter = 90;
	wave = 1;


	MonsterSprites = new sf::Texture;
	if (!MonsterSprites->loadFromFile("assets/MonsterSprite.png")) {
		std::cerr << "The Monster texture could not be loaded" << std::endl;
	}
}

MonsterController::~MonsterController() {
	/*for (shared_ptr<Monster> monster : *MonsterVec) {
		delete monster;
		monster = nullptr;
	}*/
	delete MonsterSprites;
	MonsterSprites = nullptr;
	MonsterVec->clear();
	delete MonsterVec;
	MonsterVec = nullptr;
}

// This is used to produce Monster, and will be placed
// within the MonsterVec vector
shared_ptr<Monster> MonsterController::spawnMonster(sf::Vector2f position,
		MonsterType type) {
	shared_ptr<Monster> monster = nullptr;
	std::map<string, int> props;

	switch (type) {
	case MonsterType::slow:
		props = gameState->getMonsterProps(type);
		monster = std::make_shared<Monster>(position, MonsterBaseSize, MonsterSprites,
				sf::IntRect(0, 0, 130, 130), (int)(props["tam"] * modifier),
			(int)(props["health"] * modifier), (int)(props["damage"] * modifier),
				(float)props["speed"] * modifier, MonsterType::slow);
		break;
	case MonsterType::regular:
		props = gameState->getMonsterProps(type);
		monster = std::make_shared<Monster>(position, MonsterBaseSize, MonsterSprites,
				sf::IntRect(0, 130, 130, 130), (int)(props["tam"] * modifier),
			(int)(props["health"] * modifier), (int)(props["damage"] * modifier),
			(float)props["speed"] * modifier, MonsterType::regular);
		break;
	case MonsterType::fast:
		props = gameState->getMonsterProps(type);
		monster = std::make_shared<Monster>(position, MonsterBaseSize, MonsterSprites,
				sf::IntRect(0, 260, 130, 130), (int)(props["tam"] * modifier),
			(int)(props["health"] * modifier), (int)(props["damage"] * modifier),
			(float)props["speed"] * modifier, MonsterType::fast);
		break;
	}
	MonsterVec->push_back(monster);
	//std::cout << "Monster added" << std::endl;
	return monster;
}

void MonsterController::MonsterWave(int number, MonsterType type) {
	spawnMonster(sf::Vector2f(875, 0), type);
}

void MonsterController::waveOne() {

	//cout << "wave one" << endl;
	if (counter == 50 || counter == 150 || counter == 300) {
		MonsterWave(3, MonsterType::slow);
	}
	if (counter == 100 || counter == 250 || counter == 350
		|| counter == 450 || counter == 500) {
		MonsterWave(3, MonsterType::regular);
	}
	if (counter == 200 || counter == 400 || counter == 550) {
		MonsterWave(3, MonsterType::fast);
	}
	if (counter == 1000) {
		counter = 0;
		this->modifier *= 1.1f;
		wave = 2;
		gameState->setCurrentWave(2);
	}
}

void MonsterController::waveTwo() {
	//cout << "wave two" << endl;
	if (counter == 50 || counter == 150 || counter == 300
		|| counter == 175) {
		MonsterWave(3, MonsterType::slow);
	}
	if (counter == 100 || counter == 250 || counter == 350
		|| counter == 475) {
		MonsterWave(3, MonsterType::regular);
	}
	if (counter == 200 || counter == 400 || counter == 550
		|| counter == 450 || counter == 500 || counter == 75) {
		MonsterWave(3, MonsterType::fast);
	}
	if (counter == 900) {
		counter = 0;
		modifier *= 1.2f;
		gameState->setCurrentWave(gameState->getCurrentWave() + 1);
	}
}


// This goes through the array and releases all Monster objects
// whose health are < 0, erasing them from the vector
void MonsterController::updateMonster() {

}

// Deprecated. This causes unknown memory accesses
void MonsterController::removeMonster(shared_ptr<Monster> targetMonster) {
// This is an acceptable computational cost due to rarity of action.
// O(N) for each deletion
	for (int i = 0, size = MonsterVec->size(); i < size; i++) {
		if ((*MonsterVec)[i] == targetMonster) {
			targetMonster = nullptr;
			MonsterVec->erase(MonsterVec->begin() + i);
			break;
		}
	}
}

void MonsterController::dealDamage(shared_ptr<Monster> Monster, int damage) {
	if (Monster != nullptr && Monster->reduceHealth(damage) <= 0) {
		gameState->updateTamBy(Monster->getTamValue());
		gameState->monsterEliminated();
		removeMonster(Monster);
		updateMonster();
	}
}

void MonsterController::update() {
// Go through each Monster object, and find the best point.
// Follow that path! This is a simple implementation, which
// relies on the fact that all paths follow linear changes,
// aka no need for diagonal travel.

	if (wave == 1) {
		waveOne();
	} else if (wave == 2) {
		waveTwo();
	}

	sf::Vector2f currentPos, targetPos, distancePos;
	int cubit = gameState->cubit;
	int maxPathIndex = pathInCubits.size();
	float pixelSpeed;
	for (shared_ptr<Monster> Monster : *MonsterVec) {
		// Monster has reached the finish line
		if (Monster->getPathIndex() == maxPathIndex) {
			gameState->updateHealthBy(-(Monster->getDamage()));
			removeMonster(Monster);
			break;
		}
		currentPos = Monster->getPosition();
		targetPos = pathInCubits[Monster->getPathIndex()];
		// Convert the path vector to one in cubits
		targetPos = sf::Vector2f(
				(pathInCubits[Monster->getPathIndex()].x * cubit) - cubit / 2,
				(pathInCubits[Monster->getPathIndex()].y * cubit) - cubit / 2);
		distancePos = targetPos - currentPos;

		if (distancePos.x == 0 && distancePos.y == 0) {
			Monster->increasePathIndex();
		} else {
			pixelSpeed = (float)Monster->getPixelSpeed();
			// Deal with excess movements
			if (distancePos.x != 0 && (abs(distancePos.x) < pixelSpeed)) {
				pixelSpeed = abs(distancePos.x);
			} else if (distancePos.y != 0 && abs(distancePos.y) < pixelSpeed) {
				pixelSpeed = abs(distancePos.y);
			}

			int y;
			MonsterType type = Monster->getMonsterType();
			if (type == MonsterType::slow) {
				y = 0;
			} else if (type == MonsterType::regular) {
				y = 130;
			} else if (type == MonsterType::fast) {
				y = 260;
			} else {
				y = 75;
			}

			//In each of these indexes, change the Monster sprite to reflect direction
			//string MonsterType = ToString(Monster->getMonsterType());

			if (distancePos.x > 0) {
				currentPos.x += pixelSpeed;
				Monster->setTextureRect(sf::IntRect(260, y, 130, 130));
			} else if (distancePos.x < 0) {
				currentPos.x -= pixelSpeed;
				Monster->setTextureRect(sf::IntRect(260, y, -130, 130));
			} else if (distancePos.y > 0) {
				currentPos.y += pixelSpeed;
				Monster->setTextureRect(sf::IntRect(0, y, 130, 130));
			} else if (distancePos.y < 0) {
				currentPos.y -= pixelSpeed;
				Monster->setTextureRect(sf::IntRect(0, y, 130, 130));
			}
			Monster->setPosition(currentPos);
		}
	}
	updateMonster();
	counter++;
}
/*
void MonsterController::turnMonster(shared_ptr<Monster> Monster, sf::IntRect _textureRect, int direction){
	Monster->getTextureRect();

	int x = 0;
	int y = 0;
	char MonsterType = ToString(Monster->getMonstertype());

	switch(MonsterType) {
		case "slow":  y=0;
		case "regular": y=130;
		case "fast": y=260;
	}

	//1 means down, 2 means right
	switch(direction) {
	case 1: x = 0;
	case 2: x = 1;
	}
	Monster->setTextureRect(sf::IntRect());

	if(MonsterType == "regular") {
		Monster->setTextureRect(sf::IntRect(222, 120, 120, 120));
		Monster->setTextureRect(_textureRect);

	}
	//Monster->setTextureRect(sf::IntRect(222, 120, 120, 120));
	//Monster->setTextureRect(textureRect);
	Monster->setPosition(sf::Vector2f(100, 400));
}
*/

void MonsterController::render() {
	for (shared_ptr<Monster> Monster : *MonsterVec) {
		Monster->render(window);
	}
}

float MonsterController::getModifier() {
	return this->modifier;
}

void MonsterController::setModifier(float _modifier) {
	this->modifier = _modifier;
}

std::vector<shared_ptr<Monster>>* MonsterController::getMonsterVec() {
	return this->MonsterVec;
}
