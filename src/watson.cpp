#include <iostream>
#include <cstdlib>
#include <vector>
#include "IO.h"
#include "NeuralNetwork.h"

using namespace std;

int main (int argc, char **argv)
{
	CSVParser csvParser;
	if (argc <= 3){
		cerr << "ERROR! Usage: watson networkFile dataFile isTrainer(0/1)\n";
		exit(1);
	}

	int isTrain = atoi(argv[3]);
	char *networkFileName = argv[1];
    char *dataFileName = argv[2];
	//these vectors store the data from the .csv files
	vector < vector <double> > dataTable;
	vector <bool> truths;

    NeuralNetwork neuralNetwork;
    neuralNetwork.initializeNetwork(networkFileName);
	if (isTrain){
		if (csvParser.loadTrainingCSV(dataFileName, dataTable, truths)){
			return 1; //error
		}else{
            neuralNetwork.trainNet(dataTable, truths);
		}
	}else{
		if (csvParser.loadEvaluationCSV(dataFileName, dataTable)){
			return 1; //error
		}else{
			//do stuff with the data!
		}
		
	}

	return 0;
}

