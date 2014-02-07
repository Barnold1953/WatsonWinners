#ifndef CSVParser_h
#define CSVParser_h

#include <iostream>
#include <vector>

struct SessionInfo
{
	double randomSeed;
	int epoch;
	int maxNetworks;
	bool isPermute;
	int minLayers;
	int maxLayers;
	int minNeurons;
	int maxNeurons;
	int numInputs;
};

using namespace std;

//sets up a training session struct from file
SessionInfo LoadSession(const char *fileName);

class CSVParser{
	public:
		CSVParser();
		int loadEvaluationCSV(string fileName, vector <vector<double> > &dataTable); 
		int loadTrainingCSV(string fileName, vector <vector<double> > &dataTable, vector<bool> &truthTable); 
		
};

#endif
