#include "NeuralNetwork.h"
#include <stdio.h>
#include <math.h> //for exp?
#include <ctime>
#include <fstream>
#include <string>

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
	bool computedTruth;
	double output;
	//init sigma for all neurons
	for (int i = 0; i < network.size(); i++){
		for (int j = 0; j < network[i].size(); j++){
			network[i][j].sigma = 0.0;
		}
	}
	
	//initialize first layer with input
	for (int i = 0; i < network[0].size(); i++){ 
		for (int j = 0; j < data.size(); j++){
			network[0][i].sigma += Sigmoid(network[0][i].weights[j] * data[j]);
		}
	}
	
	for (int i = 0; i < network.size()-1; i++){ //loop through columns except for the final output
		for (int j = 0; j < network[i].size(); j++){ //loop through neurons in column
			n1 = &(network[i][j]);
			for (int k = 0; k < network[i+1].size(); k++){ //set sigmas for next layer
				n2 = &(network[i+1][k]);
				n2->sigma += Sigmoid(n2->weights[j] * n1->sigma);
			}
		}
	}
	
	//there should only be a single neuron in the last layer. Its the output
	output = network[network.size()-1][0].sigma;
	if (output < 0.5){
		computedTruth = 0;
	}else{
		computedTruth = 1;
	}
	
	if (computedTruth != truth){
		return 1.0-(computedTruth-0.5);
	}
	return 0.0; //no error yay
}



void NeuralNetwork::initializeNetwork(string filename) {
	ifstream inFile;
	inFile.open(filename.c_str());

	string variableName;
	string variableValue;
	string line;
	bool foundEq = false;
	while(getline(inFile,line)) { //while the file isn't all read
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
		if(variableName.compare("neuronsPerLayer") == 0) { //assign values
			neuronsPerLayer = atoi(variableValue.c_str());
		} else if(variableName.compare("layers") == 0) {
			layers = atoi(variableValue.c_str());
		} else if(variableName.compare("randomSeed") == 0) {
			randomSeed = atoi(variableValue.c_str());
		} else if(variableName.compare("initialRandomBias") == 0) {
			double temp = atof(variableValue.c_str());
			if(temp == -1) initialRandomBias = time(NULL); //if it's -1, do a random seed
			else initialRandomBias = temp;
		}
		foundEq=false;
	}
	inFile.close();
}



void NeuralNetwork::trainNet(vector <vector <double> > &data, vector <bool> &truths)
{
	double error;
	for (int i = 0; i < data.size(); i++){
		error = feedForward(data[i], truths[i]);
	}
}
