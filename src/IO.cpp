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
	SessionInfo rv = SessionInfo();

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
	return rv = SessionInfo();
}

CSVParser::CSVParser(){
	
}

void PreprocessData(vector<vector<double> > &dataTable)
{
	//loop through columns
	int size = dataTable[0].size();
	vector <bool> useColumn;
	vector <double> mins, maxs;
	useColumn.resize(size, 0);
	mins.resize(size, 99999999);
	maxs.resize(size, -99999999);

	double val = 0;
	for (int j = 0; j < dataTable[0].size(); j++){
		for (int i = 0; i < dataTable.size(); i++){
			if (i == 0){
				val = dataTable[i][j];
			}else{
				if (val != dataTable[i][j]){
					useColumn[j] = 1;
				}
			}
			//get mins and maxs
			if (dataTable[i][j] < mins[j]) mins[j] = dataTable[i][j];
			if (dataTable[i][j] > maxs[j]) maxs[j] = dataTable[i][j];
		}
	}

	vector <vector <double> > newDataTable;
	newDataTable.resize(dataTable.size(), vector <double>());

	for (int i = 0; i < useColumn.size(); i++){
		if (useColumn[i]){
			for (int j = 0; j < dataTable.size(); j++){ //construct new table
				val = dataTable[j][i];
				//normalize
				if (maxs[i] - mins[i] == 0) cin.get();
				val = (val - mins[i]) / (maxs[i] - mins[i]);
			//	cout << val << "\n";
			//	cin.get();
				newDataTable[j].push_back(val);
			}
		}
	}

	dataTable = newDataTable;//copy it over
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
				if (val > 1){
					val = val / 900000.0 + 1.0;
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

	PreprocessData(dataTable);

	ofstream dumpfile("pretest.txt");
	for (int i = 0; i < dataTable.size(); i++){
		dumpfile << i << " ";
		for (int j = 0; j < dataTable[i].size(); j++){
			dumpfile << dataTable[i][j] << " ";
		}
		dumpfile << "\n";
	}
	dumpfile.close();

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
