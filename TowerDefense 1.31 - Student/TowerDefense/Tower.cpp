#include "Tower.h"
#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
typedef sf::IntRect* srcArrayPtr;

Tower::Tower(const sf::Vector2f position, sf::Texture* _texture, TowerType _type,
		float damage, int range, int cooldown) :
		Clickable(position, sf::Vector2f(120, 120)), coordinate(position), towerPos(position), texture(
				_texture), type(_type), towerDamage(damage), towerRange((float)range), baseCooldown(
				cooldown) {
	// these can be changed to adapt to whichever tower spritesheet we are using
	// and to their sizes, as well as the coordinate
	// construct current sprite on the given position
	srcSprite = new srcArrayPtr[numPers];
	int x = 0;
	int y = 0;
	for (int i = 0; i < numPers; i++) {
		srcSprite[i] = new sf::IntRect[numFrame];
		for (int j = 0; j < numFrame; j++) {
			srcSprite[i][j] = sf::IntRect(x, y, 128, 128);
			x += 128;
		}
		y += 128;
		x = 0;
	}
	
	currentSprite = srcSprite[0][0];
	towerSprite = sf::Sprite(*texture, currentSprite);
	towerSprite.setPosition(towerPos.x, towerPos.y);

	// set initial attributes
	mode = 'a';
	direction = 'r';
	currentCooldown = 0;
	wasClicked = false;
	upgradeCost = 25;
}

Tower::~Tower() {
	for (int i = 0; i < numPers; i++) {
		delete[] srcSprite[i];
		srcSprite[i] = nullptr;
	}
	delete[] srcSprite;
	srcSprite = nullptr;
}

TowerType Tower::getTowerType() const {
	return this->type;
}

// test heap functions
void Tower::setPosition(sf::Vector2f newPosition) {
	towerSprite.setPosition(newPosition);
}

void Tower::setMode(char AorS) {
	mode = AorS;
}

char Tower::getDirection() const {
	return direction;
}

char Tower::getMode() const {
	return mode;
}

void Tower::setDirection(char UDLR) {
	direction = UDLR;
}

void Tower::decreaseCooldown() {
	this->currentCooldown--;
}
void Tower::resetCooldown() {
	this->currentCooldown = this->baseCooldown;
}

void Tower::update() {
}
void Tower::render(sf::RenderWindow& _window) {
	_window.draw(this->towerSprite);
}

void Tower::onClick() {
	this->wasClicked = true;
}

sf::Vector2f Tower::getPosition() const {
	return towerPos;
}

sf::Vector2f Tower::getCenterPosition() const {
	sf::Vector2f center = this->towerSprite.getPosition();
	center.x += 120 / 2;
	center.y += 120 / 2;

	cout << center.x << ", " << center.y << endl;
	return center;
}

float Tower::getDamage() const {
	return this->towerDamage;
}
float Tower::getRange() const {
	return this->towerRange;
}

int Tower::getCooldown() {
	return this->currentCooldown;
}

int Tower::getUpgradeCost() {
	return this->upgradeCost;
}

void Tower::MonsterDirection(sf::Vector2f MonsterPos) {
	float dx = coordinate.x - MonsterPos.x;
	float dy = coordinate.y - MonsterPos.y;
	if ((dy > 0 && (dx >= 0 && dx < dy)) || (dy > 0 && (dx < 0 && -dx <= dy))) {
		if (direction != 'u') {
			setDirection('u');
			//cout << "direction up" << endl;
		}
	}
	else if ((dy > 0 && (dx <= 0 && dx < dy)) || (dy > 0 && (dx < 0 && -dx <= dy))) {
		if (direction != 'r') {
			setDirection('r');
			//cout << "direction right" << endl;
		}
	}
	else if ((dy < 0 && (dx >= 0 && dx <= -dy)) || (dy < 0 && (dx < 0 && dx > dy))) {
		if (direction != 'd') {
			setDirection('d');
			//cout << "direction down" << endl;
		}
	}
	else if ((dx > 0 && (dy >= 0 && dx >= dy)) || (dx > 0 && (dy < 0 && dx > -dy))) {
		if (direction != 'l') {
			setDirection('l');
			//cout << "direction left" << endl;
		}
	}
	else {
		setDirection('d');
	}
}

sf::Sprite Tower::getTowerSprite() const {
	return towerSprite;
}

sf::Vector2f Tower::getOriginalTowerPosition()
{
	return coordinate;
}

void resetSpriteRange() {

}

void Tower::flipSprite() {
	towerSprite.setScale(-1.0, 1.0);
	towerSprite.setPosition(towerPos.x +128, towerPos.y);
}
void Tower::flipBack() {
	towerSprite.setScale(1.0, 1.0);
	towerSprite.setPosition(towerPos.x, towerPos.y);
}

float Tower::MonsterDistance(sf::Vector2f Monster) // returns distance between two points
		{
	float x = (coordinate.x - Monster.x) * (coordinate.x - Monster.x);
	float y = (coordinate.y - Monster.y) * (coordinate.y - Monster.y);
	float xPlusY = x + y;
	return (float) (powf(xPlusY, 0.5)); //returns hypotenuse , ie. distance between points
}

bool Tower::MonsterCollides(sf::Vector2f Monster) // simple collision detection between two circles
{
	float dist = MonsterDistance(Monster); // gets distance between circles
	return dist < towerRange; // if dist < tower range we have a collision
}

void Tower::Attack() {
	// (myClock.getElapsedClick() > attackSpeeck) or whatever,
	// waiting for the "click" done by the controller class
	// use another layer of conditional to determine which direction is facing
	// i.e. switch (direction) {case 'l': //sth; case 'r': //sth; ...}
	// where "sth" is using the logic bellow to loop through its sprites
	//spriteCooldownSwitch starts at 15, each tick represents one cycle
	switch (direction) {
	case 'u':
		if (spriteFrame){
			towerSprite.setTextureRect(srcSprite[1][0]);
			spriteFrame = false;
		}
		else {
			towerSprite.setTextureRect(srcSprite[1][1]);
			spriteFrame = true;
		}
		break;
	case 'd':
		if (spriteFrame) {
			towerSprite.setTextureRect(srcSprite[0][0]);
			spriteFrame = false;
		}
		else{
			towerSprite.setTextureRect(srcSprite[0][1]);
			spriteFrame = true;
		}
		break;
	case 'l':
		if (spriteFrame) {
			towerSprite.setTextureRect(srcSprite[2][0]);
			spriteFrame = false;
			flipSprite();
		}
		else {
			towerSprite.setTextureRect(srcSprite[2][1]);
			spriteFrame = true;
			flipSprite();
		}
		break;
	case 'r':
		if (spriteFrame) {
			towerSprite.setTextureRect(srcSprite[2][0]);
			spriteFrame = false;
			flipBack();
		}
		else{
			towerSprite.setTextureRect(srcSprite[2][1]);
			spriteFrame = true;
			flipBack();
		}
		break;
	} // end switch
}


void Tower::StopAttack() {
	currentSprite = srcSprite[0][0];
	towerSprite.setTextureRect(currentSprite);
}

void Tower::upgrade() {
	this->towerDamage *= 1.2f;
	this->towerRange *= 1.2f;
	this->currentCooldown = (int)(this->currentCooldown * 1.2f);
	this->upgradeCost = (int)(this->upgradeCost * 1.8f);
}

srcArrayPtr* Tower::getIntRects() const {
	srcArrayPtr* newArr = new srcArrayPtr[numPers];
	for (int i = 0; i < numPers; i++) {
		newArr[i] = new sf::IntRect[numFrame];
		for (int j = 0; j < numFrame; j++) {
			newArr[i][j] = srcSprite[i][j];
		}
	}
	return newArr;
}
