#include "Neuron.h"
#include <iostream>
#include <vector>
using namespace std;

class NeuralNetwork{
public:
	//create network from file
	void initializeNetwork(string fileName);
	//create network from inputs (faster)
	void initializeNetwork(vector <int> &neuronsPerLayer, int RandomSeed, double TrainingVal, double IRB);
	double feedForward(vector <double> &data, bool truth);
	void trainNet(vector <vector <double> > &data, vector <bool> &truths);
	void backProp(double error);
	vector < vector <Neuron> > network; 
private:
	int layers;
	int randomSeed;
	double trainingVal;
	double initialRandomBias;
};
