/*
 * GameMenuController.cpp
 *
 *  Created on: Nov 9, 2018
 *      Author: Chris
 */
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "GameMenuController.h"
#include "Clickable.h"
#include "TowerButton.h"
#include "MenuButton.h"

using std::cout;
using std::endl;
using std::to_string;

// The position is relative to the top left of the menu region,
// so zero is used as the start menu height
sf::Vector2f GameMenuController::menuPosition(float xCubits, float yCubits) {
	return sf::Vector2f(menuPos.x + (xCubits * cubit),
			menuPos.y + (yCubits * cubit));
}

sf::Vector2f GameMenuController::getMenuPos() {
	return this->menuPos;
}

GameMenuController::GameMenuController(sf::RenderWindow* windowPointer,
		GameState* gameState) :
		windowPointer(windowPointer), gameState(gameState) {
	cubit = gameState->cubit;
	float menuRatio = 0.2f, towerButtonCubits = 1.9f * cubit, menuButtonCubits =
			1.8f * cubit;

	gameSize = sf::Vector2f(windowPointer->getSize());
	menuPos = sf::Vector2f(gameSize.x - (gameSize.x * menuRatio), 0);
	// -----------------------
	// Menu Borders
	// -----------------------
	sf::RectangleShape* _drawable = new sf::RectangleShape(
			sf::Vector2f(gameSize.x - menuPos.x, gameSize.y));
	_drawable->setFillColor(sf::Color(0, 37, 105)); // tri-blue
	_drawable->setPosition(menuPos.x, 0);
	_drawable->setOutlineThickness(-14);
	_drawable->setOutlineColor(sf::Color(210, 16, 52)); //tri-red
	drawVec.push_back(_drawable);

	// -----------------------
	// Tower Buttons
	// -----------------------
	sf::Texture* _texture = new sf::Texture;
	if (!_texture->loadFromFile("assets/TowerIcon3.png")) {
		std::cerr << "The texture does not exist" << std::endl;
	}
	texturesVector.push_back(_texture);
	//std::cerr << menuPos.x << ", " << menuPos.y << std::endl;
	std::map<string, int> props1 = gameState->getTowerProps(TowerType::thrower);
	std::map<string, int> props2 = gameState->getTowerProps(TowerType::slammer);
	std::map<string, int> props3 = gameState->getTowerProps(TowerType::swinger);
	TowerButton* _clickable;
	_clickable = new TowerButton(menuPosition(1.4f, 2.2f), towerButtonCubits,
			_texture, to_string(props1["tam"]),
			"Damage: " + to_string(props1["damage"]) + "\nRate: "
					+ to_string(props1["cooldown"]), TowerType::thrower,
			gameState);
	_clickable->setTextureRect(sf::IntRect(512 * 0, 0, 512, 512));
	towerButtonVec.push_back(_clickable);

	_clickable = new TowerButton(menuPosition(1.4f, 6.2f), towerButtonCubits,
			_texture, to_string(props2["tam"]),
			"Damage: " + to_string(props2["damage"]) + "\nRate: "
					+ to_string(props2["cooldown"]), TowerType::slammer,
			gameState);
	_clickable->setTextureRect(sf::IntRect(512 * 1, 0, 512, 512));
	towerButtonVec.push_back(_clickable);

	_clickable = new TowerButton(menuPosition(1.4f, 10.2f), towerButtonCubits,
			_texture, to_string(props3["tam"]),
			"Damage: " + to_string(props3["damage"]) + "\nRate: "
					+ to_string(props3["cooldown"]), TowerType::swinger,
			gameState);
	_clickable->setTextureRect(sf::IntRect(512 * 2, 0, 512, 512));
	towerButtonVec.push_back(_clickable);

	// -------------------------
	// Menu Buttons
	// -------------------------
	sf::Texture* _menuTexture = new sf::Texture;
	if (!_menuTexture->loadFromFile("assets/menuButtons.png")) {
		std::cerr << "The texture does not exist (2)" << std::endl;
	}
	texturesVector.push_back(_menuTexture);
}

GameMenuController::~GameMenuController() {
	for (sf::Drawable* d : drawVec) {
		delete d;
		d = nullptr;
	}
	for (Clickable* c : towerButtonVec) {
		sf::Vector2f temp = c->getPosition();
		temp.x = (temp.x - menuPos.x) / cubit;
		temp.y = (temp.y - menuPos.y) / cubit;
		delete c;
		c = nullptr;
	}
	for (Clickable* c : clickVec) {
		sf::Vector2f temp = c->getPosition();
		temp.x = (temp.x - menuPos.x) / cubit;
		temp.y = (temp.y - menuPos.y) / cubit;
		delete c;
		c = nullptr;
	}
	for (sf::Texture* t : texturesVector) {
		delete t;
		t = nullptr;
	}

	// Delete menu assets
	//music->stop();
}

void GameMenuController::setDebug(bool mode) {
	if (mode) {
		for (Clickable* c : towerButtonVec) {
			c->setDebug(true);
		}
		for (Clickable* c : clickVec) {
			c->setDebug(true);
		}
	}
}

// Handle windowPointer events
void GameMenuController::process(sf::Event event, sf::Vector2i mousePos) {
	// Clear the selectedTower if the menu is selected
	if ((mousePos.x >= menuPos.x)
			&& gameState->getPurchaseTower() != TowerType::empty
			&& (event.type == sf::Event::MouseButtonPressed)
			&& (event.mouseButton.button == sf::Mouse::Left)) {
		gameState->setPurchaseTower(TowerType::empty);
	}
	for (Clickable* c : towerButtonVec) {
		c->process(event, mousePos);
	}
	for (Clickable* c : clickVec) {
		c->process(event, mousePos);
	}
}

void GameMenuController::update() {
	for (TowerButton* f : towerButtonVec) {
		if (f->getTowerType() == gameState->getPurchaseTower()) {
			f->setTransparency(255);
		} else {
			f->setTransparency(180);
		}
	}
}

void GameMenuController::render() {
	for (sf::Drawable* d : drawVec) {
		windowPointer->draw(*d);
	}
	for (Clickable* c : towerButtonVec) {
		c->render(*windowPointer);
	}
	for (Clickable* c : clickVec) {
		c->render(*windowPointer);
	}
	for (TowerButton* c : towerButtonVec) {
		c->renderText(*windowPointer);
	}
}

