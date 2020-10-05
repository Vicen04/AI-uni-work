/*
 * MenuButton.h
 *
 *  Created on: Nov 15, 2018
 *      Author: Connor
 */

#ifndef MENUBUTTON_H_
#define MENUBUTTON_H_
#include <SFML/Audio.hpp>
#include "Clickable.h"
#include "GameState.h"
#include "Timer.h"

class MenuButton: public Clickable {
public:
	MenuButton(sf::Vector2f _position, int _sizex, int _sizey,
			sf::Texture* _texture);
	void setTextureRect(const sf::IntRect& rect);
	void setTransparency(const int transparency);
	void update() override;
	void render(sf::RenderWindow& _window) override;

private:
	void onClick() override;
	void onMouseEnter() override;
	void onMousePressed() override;
	void onMouseLeave() override;
	sf::RectangleShape graphic;
};

class VolumeButton: public MenuButton {
public:
	VolumeButton(sf::Vector2f _position, int _sizex, int _sizey,
			sf::Texture* _texture, sf::Music* music);
private:
	sf::Music* music;
	bool isPlaying;
	void onClick() override;
};

class InfoButton: public MenuButton {
public:
	InfoButton(sf::Vector2f _position, int _sizex, int _sizey,
			sf::Texture* _texture, GameState* gameState);
private:
	GameState* gameState;
	void onClick() override;
};

class PauseButton: public MenuButton {
public:
	PauseButton(sf::Vector2f _position, int _sizex, int _sizey,
			sf::Texture* _texture, Timer* timer);
private:
	Timer* timer;
	void onClick() override;
};

class UpgradeButton: public MenuButton {
public:
	UpgradeButton(sf::Vector2f _positin, int _sizex, int _sizey,
			sf::Texture* _texture, GameState* gameState);

	bool getCanUpgrade();
	void setCanUpgrade(bool status);
private:
	GameState* gameState;
	bool selected;
	bool canUpgrade = true;

	void onClick() override;
	void onMouseEnter() override;
	void onMouseLeave() override;

};
#endif /* MENUBUTTON_H_ */
