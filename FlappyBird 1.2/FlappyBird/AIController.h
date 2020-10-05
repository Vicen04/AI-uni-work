#pragma once


#include "GameState.hpp"
#include "NeuralNetwork.h"
using namespace Sonar;



class AIController
{
public:
	AIController();
	AIController(std::vector<float> weight1, std::vector<float> weight2);
	~AIController();



	void setGameState(GameState* pGameState) { m_pGameState = pGameState; }
	void setValues(int i) {pipe = m_pGameState->GetPipeContainer(); land = m_pGameState->GetLandContainer(); bird = m_pGameState->GetBirds().at(i); }
	void update();
	float  GetScore() { return score; }
	void SetGeneration(float gen) { generation = gen + 1; }
	void SetGeneration() { generation++; }
	int GetGeneration() { return generation; }
	std::vector<float> GetWeights() { return weight; }
	void SetSurvivalTime(float time) { survival = time; if (time > score) score = time; }
	float GetSurvivalTime() { return survival; }
	bool shouldFlap(); // note when this is called, it resets the flap state
	bool neuralNetwork();

public:

private:
	float distanceToFloor();
	float distanceToNearestPipes();
	float distanceToCentreOfPipeGap();

private:
	GameState*	m_pGameState;
	bool		m_bShouldFlap;
	float survival;
	std::vector<float> weight;
	int inputs = 4, output = 1, hidden = 12, generation = 0;
	float fDistanceToFloor = 0.0f , fDistanceToNearestPipe = 0.0f, fDistanceToCentreOfGap = 0.0f, gapToFloor = 0.0f, insidePipe = 1.0f, score = 0.0f;
	Pipe* pipe;
	Land* land;
	Bird* bird;
	NeuralNetwork* network;

};

