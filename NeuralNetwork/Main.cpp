#include "MultiLayerPerceptron.h"
#include "Math.h"
#include <numeric>
#include <iostream>

int main(void) {
	MultiLayerPerceptron* nnet = new MultiLayerPerceptron(4, 5, 4);

	vector<float> x = { 1.f,2.f,3.f,4.f };
	vector<vector<float>> w = { {1,2,3,4,5},{2,3,4,5,6}, { 3,4,5,6,7 }, { 4,5,6,7,8 } };
	vector<float> y = nnet->predict(x);
	//vector<float> z = Math::dot(x, w);
	for (auto a : y) {
		cout << a << " ";
	}

	while (1);
	return 0;
}