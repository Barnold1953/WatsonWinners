
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

void NeuralNetwork::trainNet(vector <vector <double> > &data, vector <bool> &truths)
{
	double error;
	for (int i = 0; i < data.size(); i++){
		error = feedForward(data[i], truths[i]);
	}
}