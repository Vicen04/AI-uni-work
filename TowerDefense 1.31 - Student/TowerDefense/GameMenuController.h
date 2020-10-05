/*
 * GameMenuController.h
 *
 *  Created on: Nov 9, 2018
 *      Author: Taffy
 */

#ifndef GAMEMENUCONTROLLER_H_
#define GAMEMENUCONTROLLER_H_
#include "Clickable.h"
#include "GameState.h"
#include "MenuButton.h"
#include "TowerButton.h"
#include <SFML/Audio.hpp>
#include <vector>

class GameMenuController {
public:
	GameMenuController(sf::RenderWindow* windowPointer, GameState* gameState);
	virtual ~GameMenuController();

	void setDebug(bool mode);
	void initialize();
	void run();
	void process(sf::Event event, sf::Vector2i mousePos);
	void update();
	void render();
	sf::Vector2f getMenuPos();

private:
	std::vector<sf::Drawable*> drawVec;
	std::vector<TowerButton*> towerButtonVec;
	std::vector<Clickable*> clickVec;
	std::vector<sf::Texture*> texturesVector;
	sf::RenderWindow* windowPointer;
	sf::Music* music;
	GameState* gameState;
	sf::Vector2f gameSize, menuPos;
	int cubit;
	sf::Vector2f menuPosition(float xCubits, float yCubits);

	UpgradeButton* upgradeButton;
};

#endif /* GAMEMENUCONTROLLER_H_ */
