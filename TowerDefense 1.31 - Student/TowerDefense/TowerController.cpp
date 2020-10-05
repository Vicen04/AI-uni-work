/*
 * TowerController.cpp
 *
 *  Created on: Nov 24, 2018
 *      Author: Chris
 */

#include "Tower.h"
#include "TowerController.h"


TowerController::TowerController(sf::RenderWindow* _window, GameState* _gameState) :
		window(_window), gameState(_gameState) {
	slammerTexture = new sf::Texture;
	swingerTexture = new sf::Texture;
	throwerTexture = new sf::Texture;
	slammerTexture->loadFromFile("assets/SlammingTowerSprite.png");
	swingerTexture->loadFromFile("assets/SwingTowerSprite.png");
	throwerTexture->loadFromFile("assets/ThrowingTowerSprite.png");
	towerVec = new std::vector<Tower*>;
}

TowerController::~TowerController() {
	for (Tower* tower : *towerVec) {
		delete tower;
		tower = nullptr;
	}
	delete slammerTexture;
	delete swingerTexture;
	delete throwerTexture;
}

Tower* TowerController::spawnTower(sf::Vector2f position, TowerType type) {
	Tower* tower;
	sf::Texture* texture = slammerTexture;
	if (type == TowerType::empty) {
		return nullptr;
	} else if (type == TowerType::slammer) {
		texture = slammerTexture;
	} else if (type == TowerType::swinger) {
		texture = swingerTexture;
	} else if (type == TowerType::thrower) {
		texture = throwerTexture;
	}
	std::map<string, int> props = gameState->getTowerProps(type);
	tower = new Tower(position, texture, type, (float)props["damage"], props["range"],
			props["cooldown"]);
	towerVec->push_back(tower);
	return tower;
}

void TowerController::process(const sf::Event& event,
		const sf::Vector2i& mousePos) {
	for (Tower* tower : *towerVec) {
		tower->process(event, mousePos);
		if (tower->wasClicked) {
			gameState->setBoardTower(tower);
			gameState->setPurchaseTower(TowerType::empty);
			tower->wasClicked = false;
		}
	}
}

void TowerController::update() {
}

void TowerController::render() {
	sf::CircleShape range;
	sf::Vector2f pos;
	range.setFillColor(sf::Color::Transparent);
	range.setOutlineColor(sf::Color::Red);
	range.setOutlineThickness(-3);
	for (Tower* tower : *towerVec) {
		window->draw(tower->getTowerSprite());

		if (gameState->getBoardTower() == tower) {
			pos = tower->getOriginalTowerPosition();
			range.setRadius(tower->getRange());
			pos.x = pos.x - tower->getRange() + 60;
			pos.y = pos.y - tower->getRange() + 60;
			range.setPosition(pos);
			window->draw(range);
		}
	}
}

std::vector<Tower*>* TowerController::getTowerVec() {
	return this->towerVec;
}

