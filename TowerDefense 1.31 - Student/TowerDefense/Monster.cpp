/*
 * Monster.cpp
 *
 *  Created on: Nov 22, 2018
 *      Author: Chris
 */



#include "Monster.h"
#include <iostream>

Monster::Monster(sf::Vector2f _position, sf::Vector2f _size, sf::Texture* _texture,
		sf::IntRect _textureRect, int _tam, int _health, int _damage,
		float _pixelSpeed, MonsterType _type) :
		texture(_texture), textureRect(_textureRect), tamValue(_tam), health(
				_health), damage(_damage), pixelSpeed(_pixelSpeed), type(_type) {
	// Map shape properties
	shape = sf::RectangleShape(sf::Vector2f(_size.x, _size.y));
	shape.setPosition(_position);
	shape.setSize(_size);
	shape.setTexture(texture);
	shape.setTextureRect(textureRect);

	//shape.setType(type);

	shape.setFillColor(sf::Color(255, 255, 255, 255)); // Half transparency
	targetPathPoint = 0;
}

// MonsterController is responsible for creating and deleting objects
Monster::~Monster() {
}

int Monster::getTamValue() {
	return this->tamValue;
}
int Monster::getHealth() {
	return this->health;
}
int Monster::getDamage() {
	return this->damage;
}

sf::Vector2f Monster::getPosition() {
	return this->shape.getPosition();
}

sf::Vector2f Monster::getCenterPosition() {
	sf::Vector2f center = this->shape.getPosition();
	center.x += this->shape.getSize().x / 2;
	center.y += this->shape.getSize().y / 2;
	return center;
}

MonsterType Monster::getMonsterType() const {
	return this->type;
}

void Monster::setPosition(sf::Vector2f position) {
	this->shape.setPosition(position);
}

sf::IntRect Monster::getTextureRect() {
	return this->textureRect;
}

void Monster::setTextureRect(sf::IntRect textureRect) {
	this->textureRect = textureRect;
    this->shape.setTextureRect(textureRect);
}

int Monster::reduceHealth(int damage) {
	this->health -= damage;
	return this->health;
}

int Monster::getPixelSpeed() {
	return (int)this->pixelSpeed;
}

int Monster::getPathIndex() {
	return this->targetPathPoint;
}

void Monster::increasePathIndex() {
	this->targetPathPoint++;
}

//	This is the movement logic that updates the position
void Monster::update() {

}

void Monster::render(sf::RenderWindow* window) {
	window->draw(shape);
}

