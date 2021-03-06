#include <iostream>
#include <cstdlib>
#include <vector>
#include "IO.h"
#include "NeuralNetwork.h"
#include "Threadpool.h"

using namespace std;

void SingleExecution(char **argv);
void MultithreadedTrainer(char **argv);

int main (int argc, char **argv)
{
	if (argc != 3 && argc != 4){
		cerr << "ERROR! Usage:\n   Single Execution: watson networkFile dataFile isTrainer(0/1)\n   Multithreaded Trainer: watson dataFile sessionFile\n";
		exit(1);
	}

	if (argc == 4){
		SingleExecution(argv);
	}else if (argc == 3){
		MultithreadedTrainer(argv);
	}

	return 0;
}



void SingleExecution(char **argv)
{
	CSVParser csvParser;
	int isTrain = atoi(argv[3]);
	char *networkFileName = argv[1];
    char *dataFileName = argv[2];
	//these vectors store the data from the .csv files
	vector < vector <double> > dataTable;
	vector <bool> truths;

    NeuralNetwork neuralNetwork;
	if (isTrain){
		if (csvParser.loadTrainingCSV(dataFileName, dataTable, truths)){
			return; //error
		}else{
			neuralNetwork.network.push_back(vector <Neuron>());
			neuralNetwork.network[neuralNetwork.network.size() - 1].resize(dataTable[0].size());
			neuralNetwork.initializeNetwork(networkFileName);
            neuralNetwork.trainNet(dataTable, truths);
		}
	}else{
		if (csvParser.loadEvaluationCSV(dataFileName, dataTable)){
			return; //error
		}else{
			//do stuff with the data!
		}
		
	}
}

void MultithreadedTrainer(char **argv)
{
	CSVParser csvParser;
    char *dataFileName = argv[1];

	SessionInfo sessionInfo = LoadSession(argv[2]);

	Threadpool::Begin(4); //TODO: Determine how many hardware threads the machine has

	Threadpool::End();
	
}
