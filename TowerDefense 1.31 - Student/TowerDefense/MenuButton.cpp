/*
 * MenuButton.cpp
 *
 *  Created on: Nov 15, 2018
 *      Author: Connor
 */
#include <SFML/Graphics.hpp>
#include "MenuButton.h"
#include "Clickable.h"
#include <iostream>

MenuButton::MenuButton(sf::Vector2f _position, int _sizex, int _sizey,
		sf::Texture* _texture) :
		Clickable(_position, sf::Vector2f((float)_sizex, (float)_sizey)) {
	graphic = sf::RectangleShape(sf::Vector2f((float)_sizex, (float)_sizey));
	setPosition(_position);
	graphic.setTexture(_texture);
	graphic.setFillColor(sf::Color(255, 255, 255, 180)); // Half transparency
	update();
}

void MenuButton::setTextureRect(const sf::IntRect& rect) {
	graphic.setTextureRect(rect);
}

void MenuButton::setTransparency(const int transparency) {
	graphic.setFillColor(sf::Color(255, 255, 255, transparency));
}

void MenuButton::update() {
	graphic.setPosition(getPosition());
}
void MenuButton::render(sf::RenderWindow& _window) {
	_window.draw(graphic);
}

void MenuButton::onClick() {
}

void MenuButton::onMouseEnter() {
	graphic.setFillColor(sf::Color(255, 255, 255, 255)); // No transparency
}

void MenuButton::onMousePressed() {
}

void MenuButton::onMouseLeave() {
	graphic.setOutlineThickness(0);
	graphic.setFillColor(sf::Color(255, 255, 255, 180)); // Half transparency
}

// Volume Button Implementation
VolumeButton::VolumeButton(sf::Vector2f _position, int _sizex, int _sizey,
		sf::Texture* _texture, sf::Music* music) :
		MenuButton(_position, _sizex, _sizey, _texture), music(music) {
	//music->play();
	isPlaying = true;
}

void VolumeButton::onClick() {
	if (isPlaying) {
		music->setVolume(0);
	} else {
		music->setVolume(100);
	}
	isPlaying ^= 1;
}

// Info Button Implementation
InfoButton::InfoButton(sf::Vector2f _position, int _sizex, int _sizey,
		sf::Texture* _texture, GameState* gameState) :
		MenuButton(_position, _sizex, _sizey, _texture), gameState(gameState) {
}

void InfoButton::onClick() {
	gameState->toggleHelpScreen();
}

// Pause Button Implementation
PauseButton::PauseButton(sf::Vector2f _position, int _sizex, int _sizey,
		sf::Texture* _texture, Timer* timer) :
		MenuButton(_position, _sizex, _sizey, _texture), timer(timer) {
}

void PauseButton::onClick() {
	if (timer->isRunning()) {
		setTextureRect(sf::IntRect(896, 384, 128, 128));
		timer->stop();
	} else {
		setTextureRect(sf::IntRect(897, 256, 128, 128));
		timer->start();
	}
}

// Upgrade Button Implementation
UpgradeButton::UpgradeButton(sf::Vector2f _position, int _sizex, int _sizey,
		sf::Texture* _texture, GameState* _gameState) :
		MenuButton(_position, _sizex, _sizey, _texture), gameState(_gameState) {
	selected = false;
}

bool UpgradeButton::getCanUpgrade() {
	return this->canUpgrade;
}

void UpgradeButton::setCanUpgrade(bool status) {
	this->canUpgrade = status;
}

void UpgradeButton::onClick() {
	if (gameState->getBoardTower() != nullptr && canUpgrade) {
		gameState->updateTamBy(-(gameState->getBoardTower()->getUpgradeCost()));
		gameState->getBoardTower()->upgrade();
	}
}

void UpgradeButton::onMouseEnter() {
}

void UpgradeButton::onMouseLeave() {
}
