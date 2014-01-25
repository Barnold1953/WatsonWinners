#include "CSVParser.h"

using namespace std;

int main(){
	CSVParser temp;
	vector <vector<double>> dataTable;
	vector <int> truthTable;

	/*
	temp.loadTrainingCSV("TGMC_SampleData.csv", dataTable, truthTable);
	
	for (int i = 0; i < dataTable.size(); i++){
		for (int j = 0; j < dataTable[i].size(); j++){
			cout << dataTable[i][j] << ", ";
		}
		cout << "Truth value: " << truthTable[i] << endl;
		cout << endl;
	}
	*/

	temp.loadEvaluationCSV("TGMC_SampleData.csv", dataTable);
	for (int i = 0; i < dataTable.size(); i++){
		for (int j = 0; j < dataTable[i].size(); j++){
			cout << dataTable[i][j] << ", ";
		}
		cout << endl;
	}
}