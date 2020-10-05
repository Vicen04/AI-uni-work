/*
 * TowerButton.h
 *
 *  Created on: Nov 10, 2018
 *      Author: Taffy
 */

#pragma once
#include "Clickable.h"
#include "GameState.h"
#include <string>

class TowerButton: public Clickable {
public:
	TowerButton(sf::Vector2f _position, float _radius, sf::Texture* _texture,
			std::string inText, std::string in, TowerType _type, GameState* _gameState);
	void setTextureRect(const sf::IntRect& rect);
	virtual ~TowerButton();
	TowerType getTowerType();
	void setTransparency(const int transparency);

	void update() override;
	// Draw the button, relying on textures/shapes defined
	// by the class that implements this interface.
	void render(sf::RenderWindow& _window) override;
	void renderText(sf::RenderWindow& _window) ;

private:
	TowerType towerType;
	GameState* gameState;

	sf::Texture* _menuTexture;
	void onClick() override;
	void onMouseEnter() override;
	void onMousePressed() override;
	void onMouseLeave() override;
	sf::CircleShape graphic;
	bool displayText;
	sf::Text towerButtonText;
	sf::Text towerButtonText2;
	sf::RectangleShape towerTextBack;
};

