#define _CRT_SECURE_NO_WARNINGS
#include "IO.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>

SessionInfo LoadSession(const char *fileName)
{
	ifstream inFile;
	string lineBuffer;
	double val = 0;
	SessionInfo rv;

	inFile.open(fileName);
	if (inFile.fail()){
		perror(fileName);
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
        if (variableName == "randomSeed") { //new neuron layer
            rv.randomSeed = atof(variableValue.c_str());
		} else if (variableName == "epoch"){
			rv.epoch = atoi(variableValue.c_str());
		} else if (variableName == "maxNetworks"){
			rv.maxNetworks = atoi(variableValue.c_str());
		} else if (variableName == "isPermute"){
			rv.isPermute = atoi(variableValue.c_str());
		} else if (variableName == "minLayers"){
			rv.minLayers = atoi(variableValue.c_str());
		} else if (variableName == "maxLayers"){
			rv.maxLayers = atoi(variableValue.c_str());
		} else if (variableName == "minNeurons"){
			rv.minNeurons = atoi(variableValue.c_str());
		} else if (variableName == "maxNeurons"){
			rv.maxNeurons = atoi(variableValue.c_str());
		} else if (variableName == "numInputs"){
			rv.numInputs = atoi(variableValue.c_str());
		}
		foundEq=false;
	}

	inFile.close();
	return rv;
}

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
