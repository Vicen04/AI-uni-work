#include "throwProjectile.h"
#include <math.h>

//creates a throwProjectile object
throwProjectile::throwProjectile(int ind, int _damage,
		sf::Vector2f towerPosition, weak_ptr<Monster> MonsterToFireAt) :
		index(ind), damage(_damage), projectilePosition(towerPosition), MonsterTarget(
				MonsterToFireAt) {
	speed = 8.0f;
	if (auto Monster = MonsterToFireAt.lock()) {
		if (Monster->getPixelSpeed() > speed - 3.5f) {
			speed = Monster->getPixelSpeed() + 3.5f;
		}
	}
	projectileShape = sf::RectangleShape(sf::Vector2f(30.0f, 30.0f));
	projectileShape.setSize(sf::Vector2f(30.0f, 30.0f)); //size of projectile

	projectileShape.setPosition(towerPosition);
	if (!projectileTexture.loadFromFile(throwProjImage)) {
		std::cerr << "Error finding image\n";
	} else {
		projectileTexture.loadFromFile(throwProjImage);
		projectileShape.setTexture(&projectileTexture);
	}
	projectileCenter = sf::Vector2f(towerPosition.x + 30.0f / 2,
			towerPosition.y + 30.0f / 2);
}

throwProjectile::~throwProjectile() {
	

}

void throwProjectile::drawProjectile(sf::RenderWindow* _window) {
	_window->draw(projectileShape);
}

//gets a vector of length 1, to be multipled by a speed
sf::Vector2f throwProjectile::normalize(sf::Vector2f v) {
	float len = float(sqrt((v.x * v.x) + (v.y * v.y)));
	if (len != 0) {
		v.x = v.x / len;
		v.y = v.y / len;
	}
	return v;
}

//draws the projectile to the window
void throwProjectile::moveObjectTowardsMonster(sf::Vector2f MonsterPos) {
	sf::Vector2f movePos = normalize(MonsterPos - projectilePosition);
	projectileShape.move(movePos * getSpeed()); //using the x,y positions, moves at a percentage towards the Monster object
	projectilePosition = projectilePosition + movePos * getSpeed();
	projectileCenter = projectileCenter + movePos * getSpeed();
}

// returns distance between two points
float throwProjectile::DistanceFromMonster(sf::Vector2f Monster) {
	//use abs to prevent a negative distance
	float x = abs(
			(Monster.x - projectilePosition.x)
					* (Monster.x - projectilePosition.x));
	float y = abs(
			(Monster.y - projectilePosition.y)
					* (Monster.y - projectilePosition.y));
	return  (powf(x + y, 0.5)); //square root ( x^2+y^2 )
}
bool throwProjectile::projectileMonsterCollision(sf::Vector2f Monster) // simple collision detection between two circles
		{
	return DistanceFromMonster(Monster) < 20;
	//return DistanceFromMonster(Monster) < projectileShape.getSize().x || DistanceFromMonster(Monster) < projectileShape.getSize().y;// if dist < tower range we have a collision
} //will always do for < 10

//Accessors:
weak_ptr<Monster> throwProjectile::getMonsterTarget() {
	return MonsterTarget;
}
string throwProjectile::getThrowProjImage() {
	return throwProjImage;
}
sf::Vector2f throwProjectile::getThrowProjectilePosition() {
	return projectilePosition;
}
float throwProjectile::getSpeed() {
	return speed;
}
int throwProjectile::getDamage() {
	return damage;
}
sf::Vector2f throwProjectile::getProjectileCenter() {
	return projectileCenter;
}
