#pragma once
#include <vector>

struct Neuron
{
	~Neuron()
	{
		link.clear();
		link.shrink_to_fit();
	}

	std::vector<Neuron*> link;
	float value;
};

struct Layer
{
	~Layer()
	{
		for (int i = 0; i < neurons.size(); i++)
		{
			delete neurons.at(i);
		}
		neurons.clear();
		neurons.shrink_to_fit();
	}

	std::vector<Neuron*> neurons;
};

class NeuralNetwork
{
public:
	NeuralNetwork(int inputs, int hiddenlayers, int outputs);
    ~NeuralNetwork();
	void AdjustLayer1();
	void AdjustLayer2();
	int  GetLinkNumber();
	void SetInput(int value1, int value2, int value3, int value4);
	void Calculate(std::vector<float> weights);

	Layer* Output() { return layers.at(layers.size() - 1); }

private:
	std::vector<Layer*> layers;
};

