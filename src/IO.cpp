#define _CRT_SECURE_NO_WARNINGS
#include "IO.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>

CSVParser::CSVParser(){
	
}

int CSVParser::loadTrainingCSV(string filename, vector<vector<double> > &dataTable, vector<bool> &truthTable){
	ifstream inFile;
	string lineBuffer;
	double val = 0;
	dataTable.reserve(1000); //reserve 1000 slots
	truthTable.reserve(1000);

	inFile.open(filename.c_str());
	if (inFile.fail()){
		perror(filename.c_str());
		return 1;
	}

	while (getline(inFile, lineBuffer)){
		dataTable.push_back(vector <double>());
		dataTable.back().reserve(320); //reserve all 320 slots so we dont reallocate
		
		int prev = 0;
	
		for (int i = 0; i < lineBuffer.size(); i++){

			if ((int)lineBuffer[i] < 0){ //discard the UTF-8 Bom at the beginning of the file ( I think its bytes are always neg )
				prev = i+1;
				continue;
			}

			if (lineBuffer[i] == ','){
				lineBuffer[i] = '\0'; // so sscanf works
				if (sscanf(&(lineBuffer[prev]), "%lf", &val) < 1){
					cerr << "sscanf failed\n";
				}
				dataTable.back().push_back(val);

				prev = i+1;
			}
		}

		if (strcmp(&(lineBuffer[prev]), "true") == 0){
			truthTable.push_back(1);
		}else if (strcmp(&(lineBuffer[prev]), "false") == 0){
			truthTable.push_back(0);
		}
	}

	inFile.close();
	return 0;
}

int CSVParser::loadEvaluationCSV(string filename, vector<vector<double> > &dataTable){
		ifstream inFile;
	string lineBuffer;
	double val = 0;
	dataTable.reserve(1000); //reserve 1000 slots
	
	inFile.open(filename.c_str());
	if (inFile.fail()){
		perror(filename.c_str());
		return 1;
	}

	while (getline(inFile, lineBuffer)){
		dataTable.push_back(vector <double>());
		dataTable.back().reserve(320); //reserve all 320 slots so we dont reallocate
		
		int prev = 0;
	
		for (int i = 0; i < lineBuffer.size(); i++){

			if ((int)lineBuffer[i] < 0){ //discard the UTF-8 Bom at the beginning of the file ( I think its bytes are always neg )
				prev = i+1;
				continue;
			}

			if (lineBuffer[i] == ','){
				lineBuffer[i] = '\0'; // so sscanf works
				if (sscanf(&(lineBuffer[prev]), "%lf", &val) < 1){
					cerr << "sscanf failed\n";
				}
				dataTable.back().push_back(val);

				prev = i+1;
			}
		}
	}

	inFile.close();
	return 0;
}
