#include "MultiLayerPerceptron.h"
#include "Math.h"
#include <numeric>
#include <iostream>

int main(void) {
	MultiLayerPerceptron* nnet = new MultiLayerPerceptron(4, 5, 3);

	vector<vector<float>> x = { {1,2,3,4},{5,6,7,8} };
	vector<float> z = { 0.1,0.05,0.1,0.0,0.05,0.1,0.0,0.6,0.0,0.0 };
	vector<int> t = { 1,0 };
	vector<vector<float>> w = { {1,2,3,4,5},{2,3,4,5,6}, {3,4,5,6,7}, {4,5,6,7,8} };
	vector<vector<float>> a = { {0,1},{2,3},{4,5} };
	vector<vector<float>> b = { {0,1,2,3},{4,5,6,7} };
	vector<vector<float>> tmp;

	//tmp = Math::transpose(x);
	//for (auto& p : tmp) {
	//	for (auto& q : p)
	//		cout << q << " ";
	//	cout << endl;
	//}

	nnet->gradient(x, t);
	//cout << nnet->accuracy(x, t) << endl;


	while (1);
	return 0;
}