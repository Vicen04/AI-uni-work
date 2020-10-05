#include "GAmanager.h"
#include "Tower.h"

GAmanager::GAmanager()
{
	for (int i = 0; i < 20; i++)
	chromosomes.push_back(new Chromosome());
	loop = chromosomes.size();
}
GAmanager::~GAmanager()
{
	for (int i = 0; i < chromosomes.size(); i++)
		delete chromosomes.at(i);
	chromosomes.clear();
}
void GAmanager::Update(float kills, float time)
{
	currentIteration++;
	if (time > 0.5f)
	{
		if (chromosomes.at(currentChromosome)->GetfirstPlacement() == 0)
			chromosomes.at(currentChromosome)->SetFirstPlacement(currentIteration);
		chromosomes.at(currentChromosome)->SetKillCount(kills, time);
		chromosomes.at(currentChromosome)->SetKillLeft(kills, time);
	}
	if (currentIteration == chromosomes.at(currentChromosome)->GetgridPosX().size())
	{
		chromosomes.at(currentChromosome)->SetgridPosX(rand() % 26);
		chromosomes.at(currentChromosome)->SetgridPosY(rand() % 18);
		int random = rand() % 3;
		if (random == 0)
			chromosomes.at(currentChromosome)->SetTowerType(TowerType::slammer);
		else if (random == 1)
			chromosomes.at(currentChromosome)->SetTowerType(TowerType::swinger);
		else
			chromosomes.at(currentChromosome)->SetTowerType(TowerType::thrower);
		while (!CheckIfPossible())
			chromosomes.at(currentChromosome)->Randomize(currentIteration);
	}
	price = SetPrice();
}

void GAmanager::NextGeneration()
{
	for (int i = 0; i < chromosomes.size(); i++)
	{
		for (int j = i + 1; j < chromosomes.size(); j++)
		{
			if (chromosomes.at(i)->GetScore() < chromosomes.at(j)->GetScore())
				swap(chromosomes.at(i), chromosomes.at(j));
		}
	}
	vector<Chromosome*> temp = chromosomes;
	int rand1 = 0, rand2 = 1;
	loop = chromosomes.size() / 2;
	for (int i = 0; i < loop; i++)
	{
		rand1 = rand() % temp.size();
		rand2 = rand() % temp.size();
		while (rand2 == rand1)
			rand2 = rand() % temp.size();

		chromosomes.insert(chromosomes.begin() ,new Chromosome(temp.at(rand1), temp.at(rand2)));

		temp.erase(temp.begin() + rand1);
		if (rand1 > rand2)
			temp.erase(temp.begin() + rand2);
		else
			temp.erase(temp.begin() + rand2 - 1);
	}
	temp.clear();
	temp.shrink_to_fit();

	for (int i = loop * 2; i < chromosomes.size(); i++)
	{
		delete chromosomes.at(i);
	}
	chromosomes.erase(chromosomes.end() - loop, chromosomes.end());

}

bool GAmanager::CheckIfNextGen()
{
	if (chromosomes.size() <= 1)
	{
		currentChromosome = 0;
		return false;
	}

	if (currentChromosome == loop)
		return true;
	else
		return false;
		
}

void GAmanager::randomizeCurrentChromosomeIteration()
{
	chromosomes.at(currentChromosome)->Randomize(currentIteration);
}

int GAmanager::SetPrice()
{
	if (chromosomes.at(currentChromosome)->GetTowerType().at(currentIteration) == TowerType::slammer)
		return 30;
	else if (chromosomes.at(currentChromosome)->GetTowerType().at(currentIteration) == TowerType::swinger)
		return 50;
	else
		return 40;

}
