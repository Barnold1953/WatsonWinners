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
double N = .003;

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
		network[0][i].sigma = data[i];
	}

	for (int i = 0; i < network.size()-1; i++){ //loop through columns except for the final output
		for (int j = 0; j < network[i].size(); j++){ //loop through neurons in column
			n1 = &(network[i][j]);
			if (i != 0) n1->sigma = Sigmoid(n1->sigma); //sigmoidify our sigma if were not on the first layer
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

double sigDir(double x){
	return Sigmoid(x)*(1.0 - Sigmoid(x));
}

void NeuralNetwork::backProp(double error){
    Neuron *n = &network[network.size() - 1][0];
    
	//last neuron
    for (int i = 0; i < n->weights.size(); i++){
        double sigD = sigDir(n->sigma);
        n->delta = 2 * sigD * error;
        n->weights[i] += N * 2 * sigD * error * network[network.size() - 2][i].sigma;
    } 


    for (int i = network.size() - 2; i > 0; i--){
		for (int j = 0; j < network[i].size(); j++){
			double sigD = sigDir(network[i][j].sigma);
            double sum = 0;
            for(int k = 0; k < network[i+1].size(); k++){
                sum += network[i+1][k].delta * network[i+1][k].weights[j];
            }
			network[i][j].delta = sigD * sum;
			for (int l = 0; l < network[i][j].weights.size(); l++){
				network[i][j].weights[l] += N * network[i][j].delta * network[i - 1][l].sigma;
				//printf("Node[%d][%d] weight adjusted by %lf\n", i, j, N * network[i][j].delta * network[i - 1][l].sigma);
			}
		}
	}
}

void NeuralNetwork::trainNet(vector <vector <double> > &data, vector <bool> &truths)
{
    ofstream testdump("testdump.txt");
	double error;
	for (int i = 0; i < data.size(); i++){
		error = feedForward(data[i], truths[i]);
        backProp(error);
        testdump << i << " " << error << "\n";
		cout << i << " " << error << "\n";
	}
    testdump.close();
}
