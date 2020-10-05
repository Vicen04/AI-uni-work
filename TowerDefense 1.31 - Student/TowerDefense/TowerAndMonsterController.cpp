#include "TowerAndMonsterController.h"
#include <math.h>
#include <memory>
#include "MonsterController.h"

using std::weak_ptr;
using std::shared_ptr;

TowerAndMonsterController::TowerAndMonsterController(sf::RenderWindow* window, 
	GameState* gameState,
	::MonsterController* monsterController,
	vector<Tower*>* allTowers,
	vector<shared_ptr<Monster>>* allMonster) :
		window(window), gameState(gameState), MonsterController(
			monsterController), allThrowTowers(allTowers), allMonster(allMonster) {
}

TowerAndMonsterController::~TowerAndMonsterController() {
	for (int i = 0, size = allThrowObjects.size(); i < size; i++) {
		throwProjectile* projectile = allThrowObjects[i];
		delete projectile;
		allThrowObjects[i] = nullptr;
	}	
		allMonsterInRangeOfTowers.clear();	
}

/*___________________________________________OBJECT BEING THROWN_________________________________________________*/

void TowerAndMonsterController::addThrowObjectToList(int index, int damage,
		sf::Vector2f towerPosition, weak_ptr<Monster> MonsterPtr) {
	allThrowObjects.push_back(
			new throwProjectile(index, damage, towerPosition, MonsterPtr));
}

void TowerAndMonsterController::deleteProjectile(throwProjectile* projectile) {
	// This is an acceptable computational cost due to rarity of action.
	// O(N) for each deletion
	for (int i = 0, size = allThrowObjects.size(); i < size; i++) {
		if ((allThrowObjects)[i] == projectile) {
			delete projectile;
			projectile = nullptr;
			allThrowObjects.erase(allThrowObjects.begin() + i);
			break;
		}
	}
}

void TowerAndMonsterController::deleteThrowObjectAtIndex(int index) {
	delete allThrowObjects[index];
	allThrowObjects[index] = nullptr;
	allThrowObjects.erase(allThrowObjects.begin() + index);
	//delete image? I dont think you'll need to explicitly do this
	//delete &allThrowObjects[index];
}
void TowerAndMonsterController::drawAllThrowObjectsOnGrid(
		sf::RenderWindow* _window) {	//draws the tower onto the grid
	for (auto& projectile : allThrowObjects) {
		projectile->drawProjectile(_window);
	}
}
/*__________________________________________OBJECT BEING THROWN_________________________________________________*/

/*___________________________________________Attack Logic_________________________________________________*/

//if missile to Monster collision occurs, delete throwObject, else, move towards Monster
void TowerAndMonsterController::updateProjectiles() { //this parameter will be replaced by allMonster[j]->getPosition() in each if statement
	unsigned int i = 0;
	while(i<allThrowObjects.size()){

		if (auto Monster = allThrowObjects[i]->getMonsterTarget().lock()) {
			if (allThrowObjects[i]->projectileMonsterCollision(
					Monster->getCenterPosition())) {
				//deal damage to Monster
				MonsterController->dealDamage(Monster,
						allThrowObjects[i]->getDamage());
				//delete throw object
				deleteProjectile(allThrowObjects[i]);
				i--;
			} else { //if the projectile assigned to a Monster hasnt hit yet, move towards Monster
				allThrowObjects[i]->moveObjectTowardsMonster(
						Monster->getCenterPosition());
			}
		} else {
			deleteProjectile(allThrowObjects[i]);
			i--;
		}
		i++;
	}
}

//Missile AND Monster COLLISION
// Uses Pythagorean to detect a point collision with a circle
bool TowerAndMonsterController::collisionDetected(Tower* tower,
		weak_ptr<Monster> _Monster) {
	if (auto Monster = _Monster.lock()) {
		float distX = Monster->getPosition().x - tower->getPosition().x;
		float distY = Monster->getPosition().y - tower->getPosition().y;
		float distance = sqrt((distX * distX) + (distY * distY));

		if (distance <= tower->getRange())
			return true;
	}
	return false;
}

//ascocitate each tower with a Monster within range, each with a vector of pointers (eg. tower index 0 will fire at Monster index 0)
void TowerAndMonsterController::updateTowerMonsterRange() {
	//clear contents of allThrowTowersInRangeOfMonster
	if (allThrowTowersInRangeOfMonster.size() > 0)
		allThrowTowersInRangeOfMonster.erase(allThrowTowersInRangeOfMonster.begin(),
				allThrowTowersInRangeOfMonster.end());
	allMonsterInRangeOfTowers.erase(allMonsterInRangeOfTowers.begin(),
			allMonsterInRangeOfTowers.end());

	for (int i = 0, allTowerSize = allThrowTowers->size(); i < allTowerSize; i++) { //for each tower object
		for (int j = 0, allMonsterSize = allMonster->size(); j < allMonsterSize;
				j++) { //for each Monster object
			if (collisionDetected((*allThrowTowers)[i], (*allMonster)[j])) {
				// cout << "Tower Monster Collision" << endl;		
				weak_ptr<Monster> Monster = (*allMonster)[j];
				allMonsterInRangeOfTowers.push_back(Monster);
				allThrowTowersInRangeOfMonster.push_back((*allThrowTowers)[i]);
			}
			else
			{
				//cout << "no collision" << endl;
			}
		}
	}
}

//below creates two unique pointer vectors assigning each throw tower with a Monster target assigned to it
//cooldown, a generation of projectiles
//if Monster is in range of a tower, generate a projectile as long as the tower is not on cooldown
//assign that projectile to a Monster object

void TowerAndMonsterController::update() { //essentially do every attack related function in a small time tick within process
	//for each tower, decrease its cooldown by one tick, when at 0 it can fire
	for (int i = 0, towerSize = allThrowTowers->size(); i < towerSize; i++) {
		if ((*allThrowTowers)[i]->getCooldown() >= 1) {
			(*allThrowTowers)[i]->decreaseCooldown();
		}
	}
	//ascocitate each tower with a Monster within range, each with a vector of pointers (eg. tower index 0 will fire at Monster index 0)
	updateTowerMonsterRange();
	//if the tower is not on cooldown, create a throwProjectile object that is assigned to a Monster object
	for (int i = 0, availableMonster = allThrowTowersInRangeOfMonster.size();
			i < availableMonster; i++) { //towers and Monster will have same index i
		if (allThrowTowersInRangeOfMonster[i]->getCooldown() <= 0) { //if the tower is not on cooldown fire a new projectile, else do nothing
			//cout << "Firing Projectile" << endl;
			allThrowTowersInRangeOfMonster[i]->resetCooldown();
			auto Monster = allMonsterInRangeOfTowers[i].lock();
			(allThrowTowersInRangeOfMonster)[i]->MonsterDirection(Monster->getPosition());
			(allThrowTowersInRangeOfMonster)[i]->Attack(); //sets directioon
			addThrowObjectToList(allThrowObjects.size() - 1,
					(int)allThrowTowersInRangeOfMonster[i]->getDamage(),
					allThrowTowersInRangeOfMonster[i]->getPosition(),
					allMonsterInRangeOfTowers[i]); //append a throw object to the end of the vector
			//allThrowObjects[allThrowObjects.size() - 1]->setMonsterTarget(allMonsterInRangeOfTowers[i]); //assign a Monster target
		}
	}
	//move each projectile towards its assigned Monster target, delete and deal damage if hit
	updateProjectiles();
}

void TowerAndMonsterController::render() {
	drawAllThrowObjectsOnGrid(window);
}

