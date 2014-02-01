
//i goes right, j goes down
//         i->
//  j  ->*->*->*->*
//  |  ->*->*->*->* 
//  v  ->*->*->*->*
//     ->*->*->*->*
//

double NeuralNetwork::feedForward(vector <double> &data, bool truth)
{
	
	//init sigma for all neurons
	for (int i = 0; i < network.size(); i++){
		for (int j = 0; j < network[i].size(); j++){
			network[i][j].sigma = 0.0;	
		}
	}
	
	//initialize first layer with input
	for (int i = 0; i < network[0].size(); i++){ 
		for (int j = 0; j < data.size(); j++){
			network[0][i].sigma += network[0][i].weights[j] * data[j];
		}
	}
	
	for (int i = 0; i < network.size(); i++){ //loop through columns
		for (int j = 0; j < network[i].size(); j++){ //loop through neurons in column
			
		}
	}
}



void NeuralNetwork::initializeNetwork(string filename) {
	ifstream inFile;
	inFile.open(filename.c_str());

	string variableName;
	string variableValue;
	string line;
	bool foundEq = false;
	while(getline(inFile,line)) { //while the file isn't all read
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
		if(variableName.compare("neuronsPerLayer") == 0) { //assign values
			neuronsPerLayer = atoi(variableValue.c_str());
		}
		if(variableName.compare("layers") == 0) {
			layers = atoi(variableValue.c_str());
		}
		if(variableName.compare("randomSeed") == 0) {
			randomSeed = atoi(variableValue.c_str());
		}
		if(variableName.compare("initialRandomBias") == 0) {
			double temp = atof(variableValue.c_str());
			if(temp == -1) initialRandomBias = time(NULL); //if it's -1, do a random seed
			else initialRandomBias = temp;
		}
		foundEq=false;
	}
	inFile.close();
}

void NeuralNetwork::trainNet(vector <vector <double> > &data, vector <bool> &truths)
{
	double error;
	for (int i = 0; i < data.size(); i++){
		error = feedForward(data[i], truths[i]);
	}
}
