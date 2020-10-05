/*
 * TowerButton.cpp
 *
 * Created on: Nov 10, 2018
 * Author: Chris
 */
#include <SFML/Graphics.hpp>
#include "Tower.h"
#include "TowerButton.h"
#include "Clickable.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;

sf::Font font;

const bool _debug = true;

TowerButton::TowerButton(sf::Vector2f _position, float _radius,
		sf::Texture* _texture, std::string inText, std::string inText2,
	TowerType _type, GameState* _gameState) :
		towerType(_type), gameState(_gameState) {
	if (!font.loadFromFile("assets/georgia.ttf")) {
	}
	
	_menuTexture = new sf::Texture;
	if (!_menuTexture->loadFromFile("assets/menuButtons.png")) {
		std::cerr << "The texture does not exist (3)" << std::endl;
	}

	graphic = sf::CircleShape(_radius);
	setPosition(_position);
	setSize(sf::Vector2f(_radius * 2, _radius * 2));
	graphic.setTexture(_texture);
	graphic.setFillColor(sf::Color(255, 255, 255, 180)); // half transparent
	graphic.setOutlineColor(sf::Color(210, 16, 52)); // Tri-red

	displayText = false;
	// Text for Price (large)
	towerButtonText = sf::Text();
	towerButtonText.setString(inText);
	towerButtonText.setCharacterSize(40);
	// Text for Damage & Rate (small)
	towerButtonText2 = sf::Text();
	towerButtonText2.setString(inText2);
	towerButtonText2.setCharacterSize(20);
	// Background for text
	towerTextBack = sf::RectangleShape(sf::Vector2f(320, 128));
	towerTextBack.setTexture(_menuTexture);
	towerTextBack.setTextureRect(sf::IntRect(1056, 165, 320, 128));
	// Position by Tower
	float baseY = 45, baseX = 1573;
	if (_type == TowerType::thrower) {
		baseY += 275;
	} else if (_type == TowerType::slammer) {
		baseY += 515;
	} else if (_type == TowerType::swinger) {
		baseY += 755;
	}
	towerButtonText.setPosition(baseX + 70, baseY + 35);
	towerButtonText2.setPosition(baseX + 150, baseY + 40);
	towerTextBack.setPosition(baseX, baseY);

	update();
}

TowerButton::~TowerButton() {
	delete _menuTexture;
}

TowerType TowerButton::getTowerType() {
	return this->towerType;
}

void TowerButton::setTransparency(const int transparency) {
	graphic.setFillColor(sf::Color(255, 255, 255, transparency));
}

void TowerButton::setTextureRect(const sf::IntRect& rect) {
	graphic.setTextureRect(rect);
}

void TowerButton::update() {
	graphic.setPosition(getPosition());
}
void TowerButton::render(sf::RenderWindow& _window) {

	_window.draw(graphic);

}
void TowerButton::renderText(sf::RenderWindow& _window){
	towerButtonText.setFont(font);
	towerButtonText2.setFont(font);
	if (displayText) {
			_window.draw(towerTextBack);
			_window.draw(towerButtonText);
			_window.draw(towerButtonText2);
		}
}

void TowerButton::onClick() {
	graphic.setOutlineThickness(0);
	gameState->setPurchaseTower(this->towerType);
	gameState->setBoardTower(nullptr);
}

void TowerButton::onMouseEnter() {
	graphic.setFillColor(sf::Color(255, 255, 255, 255)); // Solid Colour
	displayText = true;
}

void TowerButton::onMousePressed() {
	graphic.setOutlineThickness(10);
}

void TowerButton::onMouseLeave() {
	graphic.setOutlineThickness(0);
	graphic.setFillColor(sf::Color(255, 255, 255, 180)); // half transparent
	displayText = false;
}
