#include "NeuralNetwork.h"
#include <stdio.h>
#include <cstdlib>
#include <math.h> //for exp?
#include <ctime>
#include <cmath>
#include <fstream>
#include <string>
#include <climits>

//i goes right, j goes down
//         i->
//  j  ->*->*->*->*
//  |  ->*->*->*->* 
//  v  ->*->*->*->*
//     ->*->*->*->*
//

double e = 2.7182818284;

inline double Sigmoid(double x){
	return 1.0/(1.0 + exp(-x));
}

inline double sigDir(double x){
	return Sigmoid(x)*(1.0 - Sigmoid(x));
}

double NeuralNetwork::feedForward(vector <double> &data, bool truth)
{
	Neuron *n1, *n2;
	double output;
	//init sigma for all neurons
	for (int i = 0; i < network.size(); i++){
		for (int j = 0; j < network[i].size(); j++){
			network[i][j].sigma = 0.0;
		}
	}

	//initialize first layer with input 
	//First layer should be 1:1 for number of inputs.
	if (data.size() != network[0].size()){
		printf("ERROR: There are %d inputs but %d neurons in the first layer\n", data.size(), network[0].size());
		exit(1);
	}

	//TODO: Get rid of the first layer altogether. We can just access the data table instead!
	for (int i = 0; i < network[0].size(); i++){ 
		network[0][i].sigma = data[i];
	}

	for (int i = 0; i < network.size()-1; i++){ //loop through columns except for the final output
		for (int j = 0; j < network[i].size(); j++){ //loop through neurons in column
			n1 = &(network[i][j]);
			if (i != 0) n1->sigma = Sigmoid(n1->sigma+n1->bias); //sigmoidify our sigma if were not on the first layer
			for (int k = 0; k < network[i+1].size(); k++){ //set sigmas for next layer
				n2 = &(network[i+1][k]);
				n2->sigma += n2->weights[j] * n1->sigma;
			}
		}
	}

	//sigmoidify final output
	network[network.size() - 1][0].sigma = Sigmoid(network[network.size() - 1][0].sigma + network[network.size() - 1][0].bias);

	//there should only be a single neuron in the last layer. Its the output
	double error;
	output = network[network.size()-1][0].sigma;
	if (truth){
		error = 1.0 - output;
	}else{
		error = 0.0 - output;
	}

	return error;
}



void NeuralNetwork::initializeNetwork(string filename) {
	ifstream inFile;
	inFile.open(filename.c_str());
	if (inFile.fail()){
		perror(filename.c_str());
		exit(1);
	}

	string variableName;
	string variableValue;
	string line;

	bool foundEq = false;
	while(getline(inFile,line)) { //while the file isn't all read
		if (line[0] == '#' || line[0] == '[') continue; //ignore comments and sections for now
		for(int i=0; i < line.size(); i++) {
			if(line[i] != '=' && foundEq == false) { //get the equal sign
				continue;
			}
			else if(line[i] == '=') { //set everything before to varibale name
				variableName = line.substr(0,i);
				foundEq = true;
			}
			else { //set everything after to variable value
				variableValue = line.substr(i);
				break;
			}
		}
		if (variableName == "l") { //new neuron layer
			network.push_back(vector <Neuron>());
			network[network.size()-1].resize(atoi(variableValue.c_str()));
		} else if(variableName == "randomSeed") {
			randomSeed = atoi(variableValue.c_str());
			if (randomSeed == -1) randomSeed = time(NULL);
		} else if(variableName == "initialRandomBias") {
			initialRandomBias = atof(variableValue.c_str());
		} else if(variableName == "trainingVal"){
			trainingVal = atof(variableValue.c_str());
		}
		foundEq=false;
	}

	//set up weights for inputs
	srand(randomSeed);

	for (int i = 0; i < network[0].size(); i++){
		network[0][i].bias = 0.0f;
		network[0][i].weights.push_back(0.0); //first layer doesnt even need weights
	}

	//set up weights for the rest of the layers
	for (int i = 1; i < network.size(); i++){
		for (int j = 0; j < network[i].size(); j++){
			network[i][j].bias = ((rand() % RAND_MAX) / (float)RAND_MAX) * initialRandomBias * 2.0 - initialRandomBias;
			network[i][j].weights.resize(network[i-1].size());
			for (int k = 0; k < network[i-1].size(); k++){ //loop through previous layer to make weights
				network[i][j].weights[k] = ((rand()%RAND_MAX)/(float)RAND_MAX) * initialRandomBias * 2.0 - initialRandomBias;
			}
		}
	}

	inFile.close();
}

void NeuralNetwork::initializeNetwork(vector <int> &neuronsPerLayer, int RandomSeed, double TrainingVal, double IRB)
{
	layers = neuronsPerLayer.size();
	randomSeed = RandomSeed;
	trainingVal = TrainingVal;
	initialRandomBias = IRB;

	network.resize(layers, vector <Neuron>());
	for (int i = 0; i < layers; i++){
		network[i].resize(neuronsPerLayer[i]);
	}

	//set up weights for inputs
	srand(randomSeed);

	for (int i = 1; i < network[0].size(); i++){ //first layer doesnt need weights
		network[0][i].weights.push_back(((rand()%INT_MAX)/(float)INT_MAX) * initialRandomBias);
	}

	//set up weights for the rest of the layers
	for (int i = 1; i < network.size(); i++){
		for (int j = 0; j < network[i].size(); j++){
			network[i][j].weights.resize(network[i-1].size());
			for (int k = 0; k < network[i-1].size(); k++){ //loop through previous layer to make weights
				network[i][j].weights[k] = ((rand()%INT_MAX)/(float)INT_MAX) * initialRandomBias;
			}
		}
	}
}

void NeuralNetwork::backProp(double error){
	Neuron *n = &(network[network.size() - 1][0]);

	//last neuron
	for (int i = 0; i < n->weights.size(); i++){
		double sigD = sigDir(n->sigma);
		n->delta = 2.0 * sigD * error;
		n->weights[i] += trainingVal * n->delta * network[network.size() - 2][i].sigma;
	} 

	//TODO:? delta can be stored in sigma to save 8 bytes on our neuron struct
	for (int i = network.size() - 2; i > 0; i--){
		for (int j = 0; j < network[i].size(); j++){
			n = &(network[i][j]);
			double sigD = sigDir(n->sigma);
			double sumError = 0;
			for(int k = 0; k < network[i+1].size(); k++){
				sumError += network[i+1][k].delta * network[i+1][k].weights[j];
			}
			n->delta = sigD * sumError;
			for (int l = 0; l < n->weights.size(); l++){
				n->weights[l] += trainingVal * n->delta * network[i - 1][l].sigma;
				//printf("Node[%d][%d] weight adjusted by %lf\n", i, j, N * network[i][j].delta * network[i - 1][l].sigma);
			}
		}
	}
}

void NeuralNetwork::trainNet(vector <vector <double> > &data, vector <bool> &truths)
{
	//*** metrics ***
	double avgError;
	int numCorrect;
	int falseCorrect; //number of FALSE values that turned up correctly
	int trueCorrect;

	//***   end   ***
	ofstream testdump("testdump.txt");
	double error;
	cout << "Random Seed: " << randomSeed << endl;
	vector< vector<double> > newData = data;
	vector< vector<double> > tempData;
	vector<bool> newTruths = truths;
	vector<bool> tempTruths;
	for (int q = 0; q < 35; q++){

		//shuffle the inputs
		while (newData.size()){
			int r = rand() % newData.size();
			tempTruths.push_back(newTruths[r]);
			tempData.push_back(newData[r]);
			newTruths[r] = newTruths.back();
			newData[r] = newData.back();
			newTruths.pop_back();
			newData.pop_back();
		}
		newData.swap(tempData);
		newTruths.swap(tempTruths);
		tempData.clear();
		tempTruths.clear();

		avgError = 0.0;
		numCorrect = 0;
		falseCorrect = 0;
		trueCorrect = 0;
		for (int i = 0; i < newData.size(); i++){
			error = feedForward(newData[i], newTruths[i]);
			
			if (error < 0){
				backProp(-(error*error));
			}
			else{
				backProp(error*error);
			}

			avgError += abs(error);
			if (abs(error) < 0.5){
				numCorrect++;
				if (newTruths[i]){
					trueCorrect++;
				} else{
					falseCorrect++;
				}
			}
			//testdump << i << " " << error << "\n";
			//cout << i << " " << error << "\n";
		}
		avgError /= newData.size();

		printf("Epoch %2d: avgError: %1.4lf Correct: %4d / %d fCorrect: %d tCorrect: %d\n", q, avgError, numCorrect, newData.size(), falseCorrect, trueCorrect);
	}
	testdump.close();
}
