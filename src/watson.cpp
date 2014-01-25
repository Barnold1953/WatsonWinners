#include <iostream>
#include <cstdlib>
#include <vector>
#include "CSVParser.h"

using namespace std;

int main (int argc, char **argv)
{
	CSVParser csvParser;
	if (argc <= 2){
		cerr << "ERROR! Usage: watson filename isTrainer(0/1)\n";
		exit(1);
	}

	int isTrain = atoi(argv[2]);
	char *fileName = argv[1];
	//these vectors store the data from the .csv files
	vector < vector <double> > dataTable;
	vector <int> truths;

	if (isTrain){
		if (csvParser.loadTrainingCSV(fileName, dataTable, truths)){
			return 1; //error
		}else{
			//do training with the data!
		}
	}else{
		if (csvParser.loadEvaluationCSV(fileName, dataTable)){
			return 1; //error
		}else{
			//do stuff with the data!
		}
		
	}

	return 0;
}

