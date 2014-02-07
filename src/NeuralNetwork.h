#include "Neuron.h"
#include <iostream>
#include <vector>
using namespace std;

class NeuralNetwork{
public:
	void initializeNetwork(string fileName);
	double feedForward(vector <double> &data, bool truth);
	void trainNet(vector <vector <double> > &data, vector <bool> &truths);
	void backProp(vector <bool> &truths);
private:
	int neuronsPerLayer;
	int layers;
	int randomSeed;
	double trainingVal;
	double initialRandomBias;
	vector < vector <Neuron> > network; //treat as a 2D array
};
