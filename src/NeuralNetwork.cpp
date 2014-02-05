#include "NeuralNetwork.h"
#include <stdio.h>
#include <cstdlib>
#include <math.h> //for exp?
#include <ctime>
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

inline double Sigmoid(double x){
	return 1.0/(1.0 + exp(-x));
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
    
	for (int i = 0; i < network[0].size(); i++){ 
		network[0][i].sigma = network[0][i].weights[0] * data[i];
	}

	for (int i = 0; i < network.size()-1; i++){ //loop through columns except for the final output
		for (int j = 0; j < network[i].size(); j++){ //loop through neurons in column
			n1 = &(network[i][j]);
			n1->sigma = Sigmoid(n1->sigma); //sigmoidify our sigma
			for (int k = 0; k < network[i+1].size(); k++){ //set sigmas for next layer
				n2 = &(network[i+1][k]);
				n2->sigma += n2->weights[j] * n1->sigma;
			}
		}
	}
	
	//sigmoidify final output
	network[network.size()-1][0].sigma = Sigmoid(network[network.size()-1][0].sigma);

	//there should only be a single neuron in the last layer. Its the output
	double error;
	output = network[network.size()-1][0].sigma;
	if (truth){
		error = output - 1.0;
	}else{
		error = output;
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
		} else if(variableName == "neuronsPerLayer") { //assign values
			neuronsPerLayer = atoi(variableValue.c_str());
		} else if(variableName == "randomSeed") {
			randomSeed = atoi(variableValue.c_str());
			if (randomSeed == -1) randomSeed = time(NULL);
		} else if(variableName == "initialRandomBias") {
			initialRandomBias = atof(variableValue.c_str());
		}
		foundEq=false;
	}

	//set up weights for inputs
	srand(randomSeed);

	for (int i = 0; i < network[0].size(); i++){
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
    
	inFile.close();
}

void NeuralNetwork::backProp(vector <bool> &truths){
/*
	for (int i = network.size() - 1; i >= 0; i--){
		for (int j = network[i].size() - 1; j >= 0; j--){
			if(i == network.size() - 1){
				for (int k = network[i][j].weights.size(); k >= 0; k--){
					network[i][j].weights[k] += deltaWeight;
				}
			}
			else{
				for (int k = network[i][j].weights.size(); k >= 0; k--){
					network[i][j].weights[k] = 
				}
			}
		}
	}
	*/
}

void NeuralNetwork::trainNet(vector <vector <double> > &data, vector <bool> &truths)
{
    ofstream testdump("testdump.txt");
	double error;
	for (int i = 0; i < data.size(); i++){
		error = feedForward(data[i], truths[i]);
        testdump << i << " " << error << "\n";
	}
    testdump.close();
}
