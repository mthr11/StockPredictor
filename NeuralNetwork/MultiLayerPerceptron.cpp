#include "MultiLayerPerceptron.h"
#include "Math.h"
#include <random>
#include <iostream>

MultiLayerPerceptron::MultiLayerPerceptron(int input_size, int hidden_size, int output_size) {

	/* •Ï”‰Šú‰» */
	W = vector<vector<vector<float>>>(2);
	b = vector<vector<float>>(2);

	W[0] = vector<vector<float>>(input_size);
	W[1] = vector<vector<float>>(hidden_size);
	b[0] = vector<float>(hidden_size, 0);
	b[1] = vector<float>(output_size, 0);

	/* ‹[——”(ƒƒ‹ƒZƒ“ƒkEƒcƒCƒXƒ^)‚Ì¶¬ */
	random_device rnd;
	mt19937 mt(rnd());
	uniform_real_distribution<> rRnd(-1, 1);

	/* d‚İ‚ğ—”‚Å‰Šú‰» */
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

vector<float> MultiLayerPerceptron::predict(vector<float>& input_data)
{
	/* “ü—Í‘w‚©‚ç1‘w–Ú‚Ö‚ÌŒvZ */
	vector<float> a1 = Math::dot(input_data, W[0]) + b[0];
	vector<float> z1;
	for (auto& a : a1)
		z1.push_back(Math::sigmoid(a));

	/* 1‘w–Ú‚©‚ço—Í‘w‚Ö‚ÌŒvZ */
	vector<float> a2 = Math::dot(z1, W[1]) + b[1];
	vector<float> output_data;
	for (int i = 0; i < (int)a2.size(); i++) {
		output_data.push_back(Math::softmax(a2, i));
	}

	return output_data;
}

float MultiLayerPerceptron::loss(vector<float>& input_data, vector<int>& train_data)
{
	vector<float> output_data = predict(input_data);

	return Math::CrossEntropyEroor(output_data, train_data);
}

float MultiLayerPerceptron::accuracy(vector<vector<float>>& input_data, vector<vector<int>>& train_data)
{
	vector<vector<float>> output_data;
	for (auto x : input_data)
		output_data.push_back(predict(x));

	int cnt = 0;
	for (int i = 0; i < (int)input_data.size(); i++) {
		auto max_o = max_element(output_data[i].begin(), output_data[i].end());
		auto max_t = max_element(train_data[i].begin(), train_data[i].end());

		if (max_o - output_data[i].begin() == max_t - train_data[i].begin())
			cnt++;
	}

	return (float)cnt / (float)input_data.size();
}
