/*
 * MonsterController.h
 *
 *  Created on: Nov 22, 2018
 *      Author: Chris
 */

#pragma once

#include "Monster.h"
#include "GameState.h"
#include "GameController.h"
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

using std::string;
using std::weak_ptr;
using std::shared_ptr;

class MonsterController {
private:
	sf::Texture* MonsterSprites;
	sf::RenderWindow* window;
	GameState* gameState;

	sf::Vector2f MonsterBaseSize = sf::Vector2f(120, 120);
	// This is a difficulty modifer that affects the stats
	// of generated Monster. This is modified when waves change
	float modifier;
	std::vector<shared_ptr<Monster>>* MonsterVec;
	const std::vector<sf::Vector2f> pathInCubits;
	int counter;
	int wave;

public:
	MonsterController() = delete;
	MonsterController(sf::RenderWindow* window, GameState* gameState,
			const std::vector<sf::Vector2f> pathToFollow);
	// This destroys all the Monster that it contains
	virtual ~MonsterController();

	void turnMonster(shared_ptr<Monster> Monster, sf::IntRect textureRect, int direction );

	shared_ptr<Monster> spawnMonster(sf::Vector2f position, MonsterType type);
	void updateMonster();
	// This removes a Monster from the vector, setting it to null.
	void removeMonster(shared_ptr<Monster> targetMonster);
	// Used by the attackController to deal damage. This function
	// is responsible for updating the state of the Monster, as well
	// as increasing the Tam value when a Monster is defeated
	void dealDamage(shared_ptr<Monster> Monster, int damage);

	void MonsterWave(int number, MonsterType type);
	void waveOne();
	void waveTwo();
	// Game functions
	void update();
	// Displays all the contained Monster on the board
	void render();

	// Accessors
	float getModifier();
	void setModifier(float _modifier);
	std::vector<shared_ptr<Monster>>* getMonsterVec();

};
