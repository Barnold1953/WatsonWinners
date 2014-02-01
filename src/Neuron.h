#ifndef Neuron_h
#define Neuron_h

#include <vector>

using namespace std;

class Neuron{
	public:
		Neuron(vector <double> &weights);

		vector <double> weights;
		double sigma;
};

#endif