#include "CSVParser.h"

CSVParser::CSVParser(){
	
}

int CSVParser::loadTrainingCSV(string filename, vector<vector<double> > &dataTable, vector<int> &truthTable){
	string temp, strcat, line;
	int i = 0;
	double tempn;
	istringstream ss;
	ifstream input;
	truthTable.resize(0);
	input.open(filename.c_str());
	int a;

//	ofstream outfile("temp.txt"); //to check validity
	
	while (!(input.eof())){
		dataTable.push_back(vector <double>());
		getline(input, line);
		int start = 0, end = 0;

		while (line.find(",", start) != string::npos && line[start] != 'T' && line[start] != 'F'){
			end = line.find(",", start);
			temp = line.substr(start, end - start);
			start = end + 1;

			ss.clear();
			ss.str(temp);
			ss >> tempn;

	//		outfile << tempn << " ";

			dataTable[i].push_back(tempn);
		}
		temp = line.substr(start, line.length() - start);
		if (temp == "true"){
	//		outfile << "true\n";
			truthTable.push_back(1);
		}else if (temp == "false"){
	//		outfile << "false\n";
			truthTable.push_back(0);
		}
		i++;
	}
//	outfile.close();
	input.close();
	return 1;
}

int CSVParser::loadEvaluationCSV(string filename, vector<vector<double> > &dataTable){
	string temp, strcat, line;
	int i = 0;
	double tempn;
	vector <double> tempv;
	istringstream ss;

	ifstream input;
	input.open(filename.c_str());

	while (!(input.eof())){
		tempv.resize(0);
		dataTable.push_back(tempv);
		int start = 0, end = 0;
		getline(input, line);
		while (line.find(",", start) != string::npos){
			end = line.find(",", start);
			temp = line.substr(start, end - start);
			start = end + 1;

			ss.clear();
			ss.str(temp);
			ss >> tempn;

			dataTable[i].push_back(tempn);
		}
		cout << i << endl;
		i++;
	}
	input.close();
	return 1;
}
