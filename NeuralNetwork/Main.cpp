#include "MultiLayerPerceptron.h"
#include "Math.h"
#include <numeric>
#include <iostream>

int main(void) {
	MultiLayerPerceptron* nnet = new MultiLayerPerceptron(4, 5, 3);

	vector<vector<float>> x = { {1,2,3,4},{5,6,8,7} };
	vector<float> z = { 0.1,0.05,0.1,0.0,0.05,0.1,0.0,0.6,0.0,0.0 };
	vector<int> t = { 1,0 };
	vector<vector<float>> w = { {1,2,3,4,5},{2,3,4,5,6}, {3,4,5,6,7}, {4,5,6,7,8} };
	vector<vector<float>> a = { {0,1},{2,3},{4,5} };
	vector<vector<float>> b = { {0,1,2,3},{4,5,6,7} };
	vector<vector<float>> tmp;

	for (int i = 0; i < 200; i++) {
		nnet->gradient(x, t);
		nnet->gradient_descent();
		cout << "Loss: " << nnet->loss(x, t);
		cout << "\nAccuracy: " << nnet->accuracy(x, t) << endl;
	}

	while (1);
	return 0;
}