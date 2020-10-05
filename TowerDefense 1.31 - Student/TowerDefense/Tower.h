#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <math.h>
#include <string>
#include <vector>

#include "Clickable.h"

using namespace std;
typedef sf::IntRect* srcArrayPtr;

enum class TowerType {
	empty, slammer, swinger, thrower
};

class Tower: public Clickable {
public:
	Tower() = delete;
	Tower(const sf::Vector2f position, sf::Texture* texture, TowerType type,
		float damage, int range, int cooldown);
	~Tower();

	bool wasClicked;
	int upgradeCost;
	// accessors for attributes on heap
	srcArrayPtr* getIntRects() const;

	// accessors for attributes on stack
	char getDirection() const;
	char getMode() const;
	sf::Sprite getTowerSprite() const;
	sf::Vector2f getOriginalTowerPosition();
	sf::Vector2f getPosition() const;
	sf::Vector2f getCenterPosition() const;
	float getDamage() const;
	float getRange() const;
	TowerType getTowerType() const;
	int getCooldown();
	int getUpgradeCost();

	// Attribute modifiers
	void setMode(char);
	void setDirection(char);
	void decreaseCooldown();
	void resetCooldown();

	void update() override;
	void render(sf::RenderWindow& _window) override;
	void onClick() override;
	virtual void upgrade();

	// Handling attacks
	void MonsterDirection(sf::Vector2f MonsterPos); // find the direction of the Monster relative to the tower
	float MonsterDistance(sf::Vector2f Monster); // returns distance between two points
	bool MonsterCollides(sf::Vector2f Monster); // collision detection between two objects, using the distance function
	void Attack();
	void StopAttack();
	void flipSprite();
	void flipBack();

	// test function
	void setPosition(sf::Vector2f);

private:
	// Sprite properties
	sf::Vector2f coordinate; // the tower position on map
	sf::Sprite towerSprite;
	sf::Vector2f towerPos;
	sf::Texture* texture;
	sf::IntRect currentSprite;
	srcArrayPtr* srcSprite; // on heap
	int numPers = 3;
	int numFrame = 3;
	TowerType type;
	int spriteCooldownSwitch = 15;
	// the attributes about attack and tower properties
	char mode; // a - attack; s - stop attack
	char direction; // udlr -> up, down, left, right
	int currentCooldown;
	bool spriteFrame=true;
	// modifed by the upgrade function
	float towerDamage;
	float towerRange;
	int baseCooldown;
};