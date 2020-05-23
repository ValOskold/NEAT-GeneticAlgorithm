#include "GanNet.h"

GanNet::GanNet() {


	//we know we will have two input neurons due to the xor test
	Neuron* xn = new Neuron;
	xn->layer = 0;
	xn->type = Input;
	Neuron* yn = new Neuron;
	yn->layer = 0;
	yn->type = Input;
	inputNeurons.push_back(xn);
	inputNeurons.push_back(yn);
	
	//int hiddenMax = 5;
	////add some random number of hidden Neurons and synapse weights;
	//srand(time(NULL)); //init srand for randomfunctions
	//int tmp = rand() % hiddenMax + 1;

	//for (int i = 0; i != tmp; i++) {
	//	Neuron tmpn;
	//	//tmpn.innov = 0;
	//	tmpn.type = Hidden;
	//	hiddenNeurons.push_back(tmpn);
	//}
	//actually we will not start with any hidden neurons

	//we know we have one output neuron due to the xor test
	Neuron* zn = new Neuron;
	zn->layer = -1;
	zn->type = Output;
	outputNeurons.push_back(zn);

	neurons.push_back(xn);
	neurons.push_back(yn);
	neurons.push_back(zn);
	//we also need to setup synpases, but there are two ways we can do this; with fully connected networks and matricies or we can do it the long way with non fully connected synapses
	//We will stay true to the initial paper as close as we can, so we will not use matrix math, unless i figure out that we actually can...
	
	////add some random number of hidden Neurons and synapse weights;
	//srand(time(NULL)); //init srand for randomfunctions
	//int max = 1;

	Synapse* xz = new Synapse;
	Synapse* yz = new Synapse;
	xz->cFrom = xn;
	xz->cTo = zn;
	xz->innov = 0;
	xz->io = true;
	xz->weight = ((double)rand() / (RAND_MAX));

	yz->cFrom = yn;
	yz->cTo = zn;
	yz->innov = 0;
	yz->io = true;
	yz->weight = ((double)rand() / (RAND_MAX));

	synapses.push_back(xz);
	synapses.push_back(yz);

}

GanNet::~GanNet()
{
	//note because many networks will point to the same neurons deleting this way may fuck with later made structures.
	//Say i crossover two networks i then destroy, i may delete most or all of the information on the newly made network.
	for (int i = 0; i != neurons.size(); i++) {
		delete neurons.at(i);
	}
	for (int i = 0; i != synapses.size(); i++) {
		delete synapses.at(i);
	}
}
	
void GanNet::addConnection()
{
	//to add a new connection we need to have some neuron unconnected to neuron of a different type feeding up, input -> hidden -> output
	//but we can randomly change the weight of the existing synapses

	//find a connection to add or modify by...
	//randomly select a number for the total number neurons minus the outputs that gives us our first neruon, this should only select through inputs and hiddens
	//randomly select a number for the total number of neurons minus the inputs, this should give us our second neuron, it should only select through hiddens and outputs
	//what if we select the same hidden neuron twice? we need a special case for this

	//we also need to figure out how to do hidden layers, because i dont want the hidden layers to hit anything lower than it.
	//given the two neurons we selected are not the same and the first neuron is not positioned lower than the second output (layers) we need to check if a connection exists between them.
	//If no connection exists

	//if a connection exists
	srand(time(NULL));
	//Randomly select a value that cannot be an output
	int tmp = rand() % neurons.size() + 1;

	while (neurons.at(tmp-1)->type == Output) {
		tmp = rand() % neurons.size() + 1;
	}
	Neuron* firstNeuron = neurons.at(tmp-1);

	//Randomly select a value that cannot be an input
	int tmp2 = rand() % neurons.size() + 1;

	while (neurons.at(tmp2-1)->type == Input) {
		tmp2 = rand() % neurons.size() + 1;
	}
	Neuron* secondNeuron = neurons.at(tmp2-1);

	//NOTE I MAY HAVE A BUG, I NEED TO MAKE SURE I DON'T SELECT RANDOM NEURONS THAT ARE ATTACHED TO THE SAME LAYER


	//alright now we need to figure out a way to check to see if these connections exist or not...
	//itterate through all the synapses and see if any contain the neurons we captured.
	int x = 0;
	for (int i = 0; i != synapses.size(); i++) {
		if ((synapses.at(i)->cFrom == firstNeuron) && (synapses.at(i)->cTo == secondNeuron)) {
			synapses.at(i)->weight = ((double)rand() / (RAND_MAX));
			return;
		}
	}
	//if we make it here we must create a new synapse linking firstNeuron and SecondNeuron
	Synapse *s = new Synapse;
	s->cFrom = firstNeuron;
	s->cTo = secondNeuron;
	s->io = true;
	s->weight = ((double)rand() / (RAND_MAX));
	//we still need to add the innov number, for now i dont have a way to handle that
	synapses.push_back(s);
	return;
}

void GanNet::addNode()
{
	//alright so lets figure this out
	//this one might be more tricky than the synapse one because we need to think about node depth

	//oh on second thought this will be easy
	//step 1 select any synapse randomly
	int tmp = rand() % synapses.size() + 1;
	//disable that synapse
	synapses.at(tmp)->io = false;
	//create a new neuron that is a higher layer than the cTo connected to the synapse
	Neuron* n = new Neuron;
	n->type = Hidden;
	n->layer = synapses.at(tmp)->cFrom->layer++;
	neurons.push_back(n);
	hiddenNeurons.push_back(n);
	//create two new synapses that then attah to this new node and the old nodes
	Synapse* s1 = new Synapse;
	s1->cFrom = synapses.at(tmp)->cFrom;
	s1->cTo = n;
	s1->io = true;
	s1->weight = ((double)rand() / (RAND_MAX));
	synapses.push_back(s1);
	
	Synapse* s2 = new Synapse;
	s2->cFrom = n;
	s2->cTo = synapses.at(tmp)->cTo;
	s2->io = true;
	s2->weight = ((double)rand() / (RAND_MAX));
	synapses.push_back(s2);
}

void GanNet::aC()
{
	//get two random neurons
	Neuron* n1;
	Neuron* n2;
	srand(time(NULL));
	int rnd1 = rand() % neurons.size() + 1;
	int rnd2 = rand() % neurons.size() + 1;
	//check if they are a valid combo
	//3 checks, if we can turn into 1 check then super but i think 3 checks is easy
	//Check 1, neuron number 1 cannot be an output
	while (neurons.at(rnd1-1)->type == Output) {
		//always returns something not an output for node 1
		int rnd1 = rand() % neurons.size() + 1;
	}
	//Check 2, nueron number 2 cannot be an input
	while (neurons.at(rnd2-1)->type == Input) {
		//always returns something not an imput for node 2
		int rnd2 = rand() % neurons.size() + 1;
	}
	//Check 3, if neuron number 1 and 2 are both hidden, nuron number 1 needs to have a smaller layer value than nueron number 2
	if ((neurons.at(rnd1-1)->type == Hidden) && (neurons.at(rnd2-1)->type == Hidden)) {
		//if both nodes are of type hidden
		if (neurons.at(rnd1-1)->layer >= neurons.at(rnd2-1)->layer) {
			//if the second node is before the first node
			n1 = neurons.at(rnd2-1);
			n2 = neurons.at(rnd1-1);
		}
		else {
			//everything is fine
			n1 = neurons.at(rnd1-1);
			n2 = neurons.at(rnd2-1);
		}
	}
	else {
		//everything is fine
		n1 = neurons.at(rnd1-1);
		n2 = neurons.at(rnd2-1);
	}

	//now that we have two nerons, we need to see if a synapse exists between them
	for (int i = 0; i < synapses.size(); i++) {
		if (synapses.at(i)->cFrom == n1 && synapses.at(i)->cTo == n2) {
			//then it exists
			//if so random value the synapse strength
			synapses.at(i)->weight = ((double)rand() / (RAND_MAX));
			return;
		}
	}
	//if not create a new synapse
	Synapse* s = new Synapse;
	s->cFrom = n1;
	s->cTo = n2;
	s->io = true;
	s->weight = ((double)rand() / (RAND_MAX));
	synapses.push_back(s);
	//we still need to add the innov number, for now i dont have a way to handle that
	
}

GanNet GanNet::crossOver(GanNet otherparent)
{
	//take this-> and otherparent's synapses and maybe neuron structure

	//for each generation contained in the synapse data we will select one of the two parents to inharet from to create a new network

	//After we generate the synaptic structure we need to figureout the neuron structure of the new network

	//Were going to want to take a parent, call it on another parent and return a new crossedover network
	return GanNet();
}