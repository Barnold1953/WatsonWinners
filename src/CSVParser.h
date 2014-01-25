#ifndef CSVParser_h
#define CSVParser_h

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class CSVParser{
	public:
		CSVParser();
		int loadEvaluationCSV(string fileName, vector <vector<double> > &dataTable); 
		int loadTrainingCSV(string fileName, vector <vector<double> > &dataTable, vector<int> &truthTable); 
		
};

#endif
