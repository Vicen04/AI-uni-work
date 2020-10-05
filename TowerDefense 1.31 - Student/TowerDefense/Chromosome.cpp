#include "Chromosome.h"
#include "Tower.h"

Chromosome::Chromosome()
{
	int random;
	for (int i = 0; i < 15; i++)
	{
		gridPosX.push_back(rand() % 26);
		gridPosY.push_back(rand() % 18);		
		random = rand() % 3;
		if (random == 0)
		    towerType.push_back(TowerType::slammer);
		else if (random == 1)
			towerType.push_back(TowerType::swinger);
		else
			towerType.push_back(TowerType::thrower);
	}
}

Chromosome::Chromosome(Chromosome* Chromosome1, Chromosome* Chromosome2)
{
	int sizeChromosome;

	if (Chromosome1->GetgridPosX().size() < Chromosome2->GetgridPosX().size())
		sizeChromosome = Chromosome1->GetgridPosX().size();
	else
		sizeChromosome = Chromosome2->GetgridPosX().size();

	for (int i = 0, j = i; i < sizeChromosome; i++)
	{
		if (Chromosome1->killCount.size() > j && Chromosome2->killCount.size() > j)
		{
			if (Chromosome1->killCount.at(j) > Chromosome2->killCount.at(j))
			{
				if (i < Chromosome1->GetfirstPlacement())
				{
					do  
					{
						gridPosX.push_back(Chromosome1->GetgridPosX().at(i));
						gridPosY.push_back(Chromosome1->GetgridPosY().at(i));
						towerType.push_back(Chromosome1->GetTowerType().at(i));
						i++;
					} while (i < Chromosome1->GetfirstPlacement());
					i--;
				}
				else
				{
					gridPosX.push_back(Chromosome1->GetgridPosX().at(i));
					gridPosY.push_back(Chromosome1->GetgridPosY().at(i));
					towerType.push_back(Chromosome1->GetTowerType().at(i));
				}

			}
			else if (Chromosome1->killCount.at(j) < Chromosome2->killCount.at(j))
			{
				if (i < Chromosome2->GetfirstPlacement())
				{
					do
					{
						
						gridPosX.push_back(Chromosome2->GetgridPosX().at(i));
						gridPosY.push_back(Chromosome2->GetgridPosY().at(i));
						towerType.push_back(Chromosome2->GetTowerType().at(i));
						i++;
					} while (i < Chromosome2->GetfirstPlacement());
					i--;
				}
				else
				{
					gridPosX.push_back(Chromosome2->GetgridPosX().at(i));
					gridPosY.push_back(Chromosome2->GetgridPosY().at(i));
					towerType.push_back(Chromosome2->GetTowerType().at(i));
				}
			}
			else
			{
				if (Chromosome1->killLeft.at(j) > Chromosome2->killLeft.at(j))
				{
					if (i < Chromosome1->GetfirstPlacement())
					{
						do
						{
							gridPosX.push_back(Chromosome1->GetgridPosX().at(i));
							gridPosY.push_back(Chromosome1->GetgridPosY().at(i));
							towerType.push_back(Chromosome1->GetTowerType().at(i));
							i++;
						} while (i < Chromosome1->GetfirstPlacement());
						i--;
					}
					else
					{
						gridPosX.push_back(Chromosome1->GetgridPosX().at(i));
						gridPosY.push_back(Chromosome1->GetgridPosY().at(i));
						towerType.push_back(Chromosome1->GetTowerType().at(i));
					}

				}
				else if (Chromosome1->killLeft.at(j) < Chromosome2->killLeft.at(j))
				{
					if (i < Chromosome2->GetfirstPlacement())
					{
						do
						{

							gridPosX.push_back(Chromosome2->GetgridPosX().at(i));
							gridPosY.push_back(Chromosome2->GetgridPosY().at(i));
							towerType.push_back(Chromosome2->GetTowerType().at(i));
							i++;
						} while (i < Chromosome2->GetfirstPlacement());
						i--;
					}
					else
					{
						gridPosX.push_back(Chromosome2->GetgridPosX().at(i));
						gridPosY.push_back(Chromosome2->GetgridPosY().at(i));
						towerType.push_back(Chromosome2->GetTowerType().at(i));
					}
				}
				else
				{
					gridPosX.push_back(rand() % 26);
					gridPosY.push_back(rand() % 18);
					int random = rand() % 3;
					if (random == 0)
						towerType.push_back(TowerType::slammer);
					else if (random == 1)
						towerType.push_back(TowerType::swinger);
					else
						towerType.push_back(TowerType::thrower);
				}
			}
			j++;
		}
		else if (Chromosome1->killCount.size() > j || Chromosome2->killCount.size() > j)
		{
			if (Chromosome1->killCount.size() > Chromosome2->killCount.size())
		     	{				
					gridPosX.push_back(Chromosome1->GetgridPosX().at(i));
					gridPosY.push_back(Chromosome1->GetgridPosY().at(i));
					towerType.push_back(Chromosome1->GetTowerType().at(i));
		    	}
			else
			    {
				    gridPosX.push_back(Chromosome2->GetgridPosX().at(i));
				    gridPosY.push_back(Chromosome2->GetgridPosY().at(i));
					towerType.push_back(Chromosome2->GetTowerType().at(i));
				}
			j++;
		}
		else
		{
			if (rand() % 2 == 1)
			{
				gridPosX.push_back(Chromosome1->GetgridPosX().at(i));
				gridPosY.push_back(Chromosome1->GetgridPosY().at(i));
				towerType.push_back(Chromosome1->GetTowerType().at(i));
			}
			else
			{
				gridPosX.push_back(Chromosome2->GetgridPosX().at(i));
				gridPosY.push_back(Chromosome2->GetgridPosY().at(i));
				towerType.push_back(Chromosome2->GetTowerType().at(i));
			}
		}

		while (!checkIfValidMove(gridPosX.at(i), gridPosY.at(i), towerType.at(i)))
			Randomize(i);
			
	}
	
}


Chromosome::~Chromosome()
{
	gridPosX.clear();
	gridPosY.clear();
	killCount.clear();
	towerType.clear();
	killLeft.clear();
}

void Chromosome::Randomize(int iteration)
{
	gridPosX.at(iteration) = rand() % 26;
	gridPosY.at(iteration) = rand() % 18;
	int random = rand() % 3;
	if (random == 0)
		towerType.at(iteration) = (TowerType::slammer);
	else if (random == 1)
		towerType.at(iteration) = (TowerType::swinger);
	else
		towerType.at(iteration) = (TowerType::thrower);
}


bool Chromosome::checkIfValidMove(int X, int Y, TowerType tower)
{
	switch (tower)
	{
	case TowerType::slammer:
		if (slammer[Y][X] == 1)
			return true;
		else
			return false;
		break;
	case TowerType::swinger:
		if (swinger[Y][X] == 1)
			return true;
		else
			return false;
		break;
	case TowerType::thrower:
		if (thrower[Y][X] == 1)
			return true;
		else
			return false;
		break;
	};
}