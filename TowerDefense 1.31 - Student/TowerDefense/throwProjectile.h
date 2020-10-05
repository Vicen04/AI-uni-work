#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include "Tower.h"
#include "Monster.h"
#include <vector>
#include <memory>
#include <string>

using std::string;
using std::weak_ptr;

class throwProjectile {
public:
	throwProjectile(int ind, int damage, sf::Vector2f towerPosition,
			weak_ptr<Monster> MonsterToFireAt);
	~throwProjectile();
	void drawProjectile(sf::RenderWindow* _window);
	//using Monster position, heatseeks towards the Monster until collision occurs
	void moveObjectTowardsMonster(sf::Vector2f MonsterPos);
	float DistanceFromMonster(sf::Vector2f Monster); // returns distance between two points
	bool projectileMonsterCollision(sf::Vector2f Monster); // collision detection between two objects, using the distance function
	void dealDamage(); //deals the damage associated with the projectile
	sf::Vector2f normalize(sf::Vector2f v);

	//Accessors:
	string getThrowProjImage();
	sf::Vector2f getThrowProjectilePosition();
	float getSpeed();
	weak_ptr<Monster> getMonsterTarget();
	int getDamage();
	sf::Vector2f getProjectileCenter();

private:
	int index;
	int damage;
	sf::Texture projectileTexture;
	sf::RectangleShape projectileShape;
	sf::Vector2f projectilePosition;
	sf::Vector2f projectileCenter;
	float speed;
	string throwProjImage = "assets/jacket.png";
	weak_ptr<Monster> MonsterTarget;
};
