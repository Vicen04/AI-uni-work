/*
 * TowerController.h
 *
 *  Created on: Nov 24, 2018
 *      Author: Chris
 */

#pragma once
#include "Tower.h"
#include "GameState.h"
#include <string>
#include <SFML/Graphics.hpp>

using std::string;

class TowerController {
public:
	sf::Texture* slammerTexture;
	sf::Texture* swingerTexture;
	sf::Texture* throwerTexture;
	sf::RenderWindow* window;
	GameState* gameState;

	sf::Vector2f towerBaseSize = sf::Vector2f(120, 120);
	// Contains all active tower pointers
	std::vector<Tower*>* towerVec;

public:
	TowerController() = delete;
	TowerController(sf::RenderWindow* window, GameState* gameState);
	// This destroys all the towers that it contains
	virtual ~TowerController();

	Tower* spawnTower(sf::Vector2f position, TowerType type);
	// Game functions

	void process(const sf::Event& event, const sf::Vector2i& mousePos);
	void update();
	// Displays all the contained tower on the board
	void render();

	// Accessors
	std::vector<Tower*>* getTowerVec();

};

