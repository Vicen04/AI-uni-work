#include "AIController.h"

#include <iostream>

using namespace std;
#define ERROR_DISTANCE 9999


AIController::AIController()
{
	m_pGameState = nullptr;
	m_bShouldFlap = false;
	network = new NeuralNetwork(inputs, hidden, output);
	int count = network->GetLinkNumber();
	for (int i = 0; i < count; i++)
		weight.push_back(float(rand() % 1801) / 1000.0f - 0.9f);
}

AIController::AIController(std::vector<float> weight1, std::vector<float> weight2)
{
	int chanceOfMutation;
	for (int i = 0; i < weight1.size(); i++)
	{
		chanceOfMutation = rand() % 20;
		if (1 == chanceOfMutation)
			weight.push_back(float(rand() % 1801) / 1000.0f - 0.9f);
		else if (rand() % 2 <= 1)
			weight.push_back(weight1.at(i));
		else
			weight.push_back(weight2.at(i));

	}
	m_pGameState = nullptr;
	m_bShouldFlap = false;
	network = new NeuralNetwork(inputs, hidden, output);
}

AIController::~AIController()
{
	delete network;
	pipe = nullptr;
	land = nullptr;
	bird = nullptr;
	weight.clear();
	weight.shrink_to_fit();
	m_pGameState = nullptr;
}

void AIController::update()
{
	if (m_pGameState == nullptr || bird == nullptr)
		return;

	// do some AI stuff, decide whether to flap
	fDistanceToFloor = distanceToFloor();

	fDistanceToNearestPipe = distanceToNearestPipes();

	if (fDistanceToNearestPipe != ERROR_DISTANCE)
	{
		fDistanceToCentreOfGap = distanceToCentreOfPipeGap();
	
		m_bShouldFlap = neuralNetwork();
	}
	else
		m_bShouldFlap = true;
	// this means the birdie always flaps. Should only be called when the bird should need to flap. 
	return;
}

float AIController::distanceToFloor()
{
	// the land is always the same height so get the first sprite
	std::vector<sf::Sprite> landSprites = land->GetSprites();
	if (landSprites.size() > 0)
	{
		return landSprites.at(0).getPosition().y - bird->GetSprite().getPosition().y;
	}

	return ERROR_DISTANCE; // this is an error but also means 
}

float AIController::distanceToNearestPipes()
{
	float nearest1 = 999999;
	sf::Sprite* nearestSprite1 = nullptr;

	// get nearest pipes
	std::vector<sf::Sprite> pipeSprites = pipe->GetSprites();
	for (unsigned int i = 0; i < pipeSprites.size(); i++) {
		sf::Sprite s = pipeSprites.at(i);
		float fDistance = s.getPosition().x + (float)s.getTextureRect().width * 0.5f  - bird->GetSprite().getPosition().x;
		if (fDistance > 0 && fDistance < nearest1) {
			nearestSprite1 = &(pipeSprites.at(i));
			nearest1 = fDistance;
		}
	}

	if (nearestSprite1 == nullptr)
		return ERROR_DISTANCE;

	insidePipe = (float)nearestSprite1->getTextureRect().width * 0.9f;
	return nearestSprite1->getPosition().x - bird->GetSprite().getPosition().x;
}

float AIController::distanceToCentreOfPipeGap()
{
	float nearest1 = 999999;
	float nearest2 = 999999;
	sf::Sprite* nearestSprite1 = nullptr;
	sf::Sprite* nearestSprite2 = nullptr;

	// get nearest pipes
	std::vector<sf::Sprite> pipeSprites = pipe->GetSprites();
	for (unsigned int i = 0; i < pipeSprites.size(); i++) {
		sf::Sprite s = pipeSprites.at(i);
		float fDistance = s.getPosition().x - bird->GetSprite().getPosition().x;
		if (fDistance > 0 && fDistance < nearest1) {
			nearestSprite1 = &(pipeSprites.at(i));
			nearest1 = fDistance;
		}
		else if (fDistance > 0 && fDistance < nearest2) {
			nearestSprite2 = &(pipeSprites.at(i));
			nearest2 = fDistance;
		}
	}

	if (nearestSprite1 == nullptr || nearestSprite2 == nullptr)
		return ERROR_DISTANCE;


	sf::Sprite* topSprite = nullptr;
	sf::Sprite* bottomSprite = nullptr;

	if (nearestSprite1->getPosition().y < nearestSprite2->getPosition().y) {
		topSprite = nearestSprite1;
		bottomSprite = nearestSprite2;
	}
	else {
		topSprite = nearestSprite2;
		bottomSprite = nearestSprite1;
	}

	float distance = ((bottomSprite->getGlobalBounds().top) - (topSprite->getGlobalBounds().height + topSprite->getGlobalBounds().top)) / 2.0f;
	distance += (topSprite->getGlobalBounds().top + topSprite->getGlobalBounds().height);

	return distance - bird->GetSprite().getPosition().y;
}

// note when this is called, it resets the flap state
bool AIController::shouldFlap() 
{
	bool output = m_bShouldFlap;
	m_bShouldFlap = false;

	return output;
}

bool AIController::neuralNetwork()
{ 
	network->SetInput(fDistanceToNearestPipe, insidePipe, fDistanceToCentreOfGap, fDistanceToFloor);

	network->Calculate(weight);

	if (network->Output()->neurons.at(0)->value < 0)
		return false;
	else
		return true;
}