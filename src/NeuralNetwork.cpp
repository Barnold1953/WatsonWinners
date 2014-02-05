#include "NeuralNetwork.h"
#include <stdio.h>
#include <math.h> //for exp?
#include <ctime>
#include <cmath>
#include <fstream>
#include <string>

//i goes right, j goes down
//         i->
//  j  ->*->*->*->*
//  |  ->*->*->*->* 
//  v  ->*->*->*->*
//     ->*->*->*->*
//
double e = 2.7182818284;
double n = .003;

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

double NeuralNetwork::sigDir(double x){
    return ((1/(1+exp(-x)))((-1*exp(-x))/(1+exp(-x))));
}

void NeuralNetwork::backPropH(double error, Neuron *n, int layer){
    double deltaW, sdSum = 0;
    double sigD = sigDir(n->sigma);
    
    for(int i = 0; i < network[layer].size(); i++){
    }
  /*
    for (int i = network.size() - 1; i >= 0; i--){
        for (int j = network[i].size() - 1; j >= 0; j--){
            if(i == network.size() - 1){
                double sigD = sigDir(network[i][j].sigma);
                for (int k = network[i][j].weights.size(); k >= 0; k--){
                    network[i][j].weights[k] += 2 * deltaWeight * ();
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

void NeuralNetwork::backProp(double error){
    Neuron *n = network[network.size() - 1][0];
    for (int i = 0; i < n->weights.size(); i++){
        double sigD = sigDir(n->sigma);
        n->weights[i] += n * 2 * sigDir * error * n->weights[i].sigma;
        backPropH(2 * sigD * error, network[network.size() - 2][i], network.size() - 2);
    }
}

void NeuralNetwork::trainNet(vector <vector <double> > &data, vector <bool> &truths)
{
    double error;
    for (int i = 0; i < data.size(); i++){
        error = feedForward(data[i], truths[i]);
    }
}
