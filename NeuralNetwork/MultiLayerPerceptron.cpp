#include "MultiLayerPerceptron.h"
#include "Math.h"
#include <random>
#include <iostream>

MultiLayerPerceptron::MultiLayerPerceptron(int input_size, int hidden_size, int output_size) {

	/* 変数初期化 */
	W = vector<vector<vector<float>>>(2);
	b = vector<vector<float>>(2);

	W[0] = vector<vector<float>>(input_size);
	W[1] = vector<vector<float>>(hidden_size);
	b[0] = vector<float>(hidden_size, 0);
	b[1] = vector<float>(output_size, 0);

	/* 擬似乱数(メルセンヌ・ツイスタ)の生成 */
	random_device rnd;
	mt19937 mt(rnd());
	uniform_real_distribution<> rRnd(-1, 1);

	/* 重みを乱数で初期化 */
	for(int i = 0;i<input_size;i++){
		for (int j = 0; j < hidden_size; j++) {
			float r = (float)rRnd(mt);
			W[0][i].push_back(r);
			//cout << r << " ";
		}
		//cout << endl;
	}
	//cout << endl;
	for (int i = 0; i < hidden_size; i++) {
		for (int j = 0; j < output_size; j++) {
			float r = (float)rRnd(mt);
			W[1][i].push_back(r);
			//cout << r << " ";
		}
		//cout << endl;
	}
}

MultiLayerPerceptron::~MultiLayerPerceptron()
{
}

vector<float> MultiLayerPerceptron::predict(vector<float>& data)
{
	vector<float> a1 = Math::dot(data, W[0]);	// + b[0];
	vector<float> z1;
	for (auto& a : a1) {
		z1.push_back(Math::sigmoid(a));
	}

	vector<float> a2 = Math::dot(z1, W[1]);		// + b[1];
	vector<float> y;
	for (int i = 0; i < (int)a2.size(); i++) {
		y.push_back(Math::softmax(a2, i));
	}

	return y;
}

float MultiLayerPerceptron::loss(vector<float>& input_data, vector<float>& train_data)
{
	return 0.0f;
}

float MultiLayerPerceptron::accuracy(vector<float>& input_data, vector<float>& train_data)
{
	return 0.0f;
}
