#pragma once
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

class GanNet
{
	enum nType {Input, Hidden, Output, Bias};

	struct Neuron {
		nType type;
		int layer;
	};

	struct Synapse {
		Neuron* cFrom;
		Neuron* cTo;
		float weight;
		bool io;
		int innov;
	};

	struct Mutator {
		float connection;
		float node;
	};

	
	int input1;
	int input2;
	float output;

	std::vector<Neuron*> inputNeurons;
	std::vector<Neuron*> hiddenNeurons;
	std::vector<Neuron*> outputNeurons;

	std::vector<Neuron*> neurons;
	std::vector<Synapse*> synapses;
	

public :
	GanNet();
	~GanNet();
	void addConnection();
	void addNode();
	void aC();
	GanNet crossOver(GanNet otherparent);
};

