#pragma once
#include "Chromosome.h"

class GAmanager
{
public:
	GAmanager();
	~GAmanager();
	void Update(float kills, float time);
	void NextGeneration();
	Chromosome* GetCurrentChromosome() { return chromosomes.at(currentChromosome); }
	bool CheckIfNextGen() ;
	bool CheckIfPossible() {return chromosomes.at(currentChromosome)->checkIfValidMove(chromosomes.at(currentChromosome)->GetgridPosX().at(currentIteration), chromosomes.at(currentChromosome)->GetgridPosY().at(currentIteration), chromosomes.at(currentChromosome)->GetTowerType().at(currentIteration));}
	int GetX() { return chromosomes.at(currentChromosome)->GetgridPosX().at(currentIteration); }
	int GetY() { return chromosomes.at(currentChromosome)->GetgridPosY().at(currentIteration); }
	TowerType GetTower() { return chromosomes.at(currentChromosome)->GetTowerType().at(currentIteration); }
	void SetCurrentChromosome() { currentChromosome++; }
	void RestartChromosomeCount() { currentChromosome = 0; }
	void RestartIteration() { currentIteration = 0; }
	void SaveScore(int score) { chromosomes.at(currentChromosome)->SetScore(score); }
	void randomizeCurrentChromosomeIteration();
	int GetChromosomeIteration() { return currentChromosome; }
	int SetPrice();
	int GetPrice() { return price; }

private:
	vector<Chromosome*> chromosomes;
	int currentChromosome = 0;
	int currentIteration = 0;
	int price, loop;
};

