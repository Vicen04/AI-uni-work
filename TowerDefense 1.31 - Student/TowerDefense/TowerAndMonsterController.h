#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <functional>

#include "GameState.h"
#include "throwProjectile.h"
#include "Tower.h"
#include "Monster.h"
class MonsterController;

using std::weak_ptr;
using std::shared_ptr;

class TowerAndMonsterController {
public:
	TowerAndMonsterController(sf::RenderWindow* window, GameState* gameState,
			MonsterController* MonsterController, vector<Tower*>* allTowers,
			vector<shared_ptr<Monster>>* allMonster);
	~TowerAndMonsterController();
	//throwTower Object
	void addThrowTowerToList(sf::Vector2f position, int index); //using string
	void deleteThrowTowerAtIndex(int index);
	//Throw Objects
	void addThrowObjectToList(int index, int damage, sf::Vector2f towerPosition,
			weak_ptr<Monster> MonsterPtr);
	void drawAllThrowObjectsOnGrid(sf::RenderWindow* theWindow);
	void deleteProjectile(throwProjectile* projectile);
	void deleteThrowObjectAtIndex(int index);
	void updateProjectiles();
	void updateTowerMonsterRange();

	//used for testing
	void addMonsterObjectToList(sf::Vector2f _position, sf::Vector2f _size,
			sf::Texture* _texture, int type, double modifier);

	void update();
	void render();
private:
	sf::RenderWindow* window;
	GameState* gameState;
	MonsterController* MonsterController;
	vector<throwProjectile*> allThrowObjects;

	vector<Tower*>* allThrowTowers;
	vector<Tower*> allThrowTowersInRangeOfMonster;

	// This is a reference to the true Monster vector. Duplication
	// should always be as a weak pointer, to allow for projectiles
	// to properly delete themselves once a target Monster is removed.
	vector<shared_ptr<Monster>>* allMonster;
	vector<weak_ptr<Monster>> allMonsterInRangeOfTowers;

	bool collisionDetected(Tower*, weak_ptr<Monster>);

	//vector<unique_ptr<Tower>> allTSlamTowers;
//vector<unique_ptr<Tower>> allSkipTowers;
	//vector<unique_ptr<Tower>> allSlamObjects;
//vector<unique_ptr<Tower>> allSkipObjects;
};

