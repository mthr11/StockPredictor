#include "MultiLayerPerceptron.h"
#include "Math.h"
#include <numeric>
#include <iostream>

int main(void) {
	MultiLayerPerceptron* nnet = new MultiLayerPerceptron(4, 5, 4);

	vector<float> x = { 0.1,0.05,0.6,0.0,0.05,0.1,0.0,0.1,0.0,0.0 };
	vector<float> z = { 0.1,0.05,0.1,0.0,0.05,0.1,0.0,0.6,0.0,0.0 };
	vector<int> t = { 0,0,1,0,0,0,0,0,0,0 };
	vector<vector<float>> w = { {1,2,3,4,5},{2,3,4,5,6}, { 3,4,5,6,7 }, { 4,5,6,7,8 } };
	vector<float> y = nnet->predict(x);
	//vector<float> z = Math::dot(x, w);
	//for (auto a : y) {
	//	cout << a << " ";
	//}
	cout << Math::CrossEntropyEroor(x, 2) << endl;

	while (1);
	return 0;
}