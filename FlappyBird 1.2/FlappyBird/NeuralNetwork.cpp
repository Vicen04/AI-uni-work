#include "NeuralNetwork.h"


NeuralNetwork::NeuralNetwork(int inputs, int hiddenlayers, int outputs)
{
	layers.push_back(new Layer());

	for (int i = 0; i < inputs; i++)
		layers.at(0)->neurons.push_back(new Neuron());


	layers.push_back(new Layer());
	for (int i = 0; i < hiddenlayers; i++)
		layers.at(1)->neurons.push_back(new Neuron());

	AdjustLayer1();

	layers.push_back(new Layer());
	for (int i = 0; i < outputs; i++)
		layers.at(2)->neurons.push_back(new Neuron());

	AdjustLayer2();
}

NeuralNetwork::~NeuralNetwork()
{
	for (unsigned int i = 0; i < layers.size(); i++)
	{
		delete layers.at(i);
	}
	layers.clear();
	layers.shrink_to_fit();
}

void NeuralNetwork::AdjustLayer1()
{
	for (unsigned int i = 0; i < layers.at(1)->neurons.size()/2; i++)
		for (unsigned int j = 0; j < layers.at(0)->neurons.size()/2; j++)
			layers.at(1)->neurons.at(i)->link.push_back(layers.at(0)->neurons.at(j));

	for (unsigned int i = layers.at(1)->neurons.size() / 2; i < layers.at(1)->neurons.size(); i++)
		for (unsigned int j = layers.at(0)->neurons.size() / 2; j < layers.at(0)->neurons.size(); j++)
			layers.at(1)->neurons.at(i)->link.push_back(layers.at(0)->neurons.at(j));
}

void NeuralNetwork::AdjustLayer2()
{
	for (unsigned int i = 0; i < layers.at(2)->neurons.size(); i++)
		for (unsigned int j = 0; j < layers.at(1)->neurons.size(); j++)
			layers.at(2)->neurons.at(i)->link.push_back(layers.at(1)->neurons.at(j));
}

void NeuralNetwork::SetInput(int value1, int value2, int value3, int value4)
{
	layers.at(0)->neurons.at(0)->value = value1;
	layers.at(0)->neurons.at(1)->value = value2;
	layers.at(0)->neurons.at(2)->value = value3;
	layers.at(0)->neurons.at(3)->value = value4;
}

void NeuralNetwork::Calculate(std::vector<float> weights)
{
	unsigned int loop = 0;

	for (unsigned int i = 1; i < layers.size(); i++)
		for (unsigned int j = 0; j < layers.at(i)->neurons.size(); j++)
		{
			float result = 0.0f;

			for (unsigned int k = 0; k < layers.at(i)->neurons.at(j)->link.size(); k++)
			{
				result = result + layers.at(i)->neurons.at(j)->link.at(k)->value * weights.at(loop);
				loop++;
			}

			layers.at(i)->neurons.at(j)->value = result;
		}
}

int NeuralNetwork::GetLinkNumber()
{
	int count = 0;
	for (unsigned int i = 1; i < layers.size(); i++)
		for (unsigned int j = 0; j < layers.at(i)->neurons.size(); j++)
		{
				count = layers.at(i)->neurons.at(j)->link.size() + count;
		}
	return count;
}