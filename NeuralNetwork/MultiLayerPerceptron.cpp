#include "MultiLayerPerceptron.h"
#include "Math.h"
#include <random>
#include <iostream>

MultiLayerPerceptron::MultiLayerPerceptron(int input, int hidden, int output)
{
	input_size = input;
	hidden_size = hidden;
	output_size = output;

	/* ƒƒ“ƒo•Ï”‰Šú‰» */
	W = vector<vector<vector<float>>>(2);
	b = vector<vector<float>>(2);
	dW = vector<vector<vector<float>>>(2);
	db = vector<vector<float>>(2);

	W[0] = vector<vector<float>>(input_size);
	W[1] = vector<vector<float>>(hidden_size);
	b[0] = vector<float>(hidden_size, -10);
	b[1] = vector<float>(output_size, -10);

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

	W[0] = { {0.1f,0.2f,0.3f,0.4f,0.5f},{0.2f,0.3f,0.4f,0.5f,0.6f}, {0.3f,0.4f,0.5f,0.6f,0.7f}, {0.4f,0.5f,0.6f,0.7f,0.8f} };
	W[1] = { {0.1f,0.2f,0.3f},{0.2f,0.3f,0.4f},{0.3f,0.4f,0.5f},{0.4f,0.5f,0.6f},{0.5f,0.6f,0.7f} };
}

MultiLayerPerceptron::~MultiLayerPerceptron()
{
}

vector<vector<float>> MultiLayerPerceptron::predict(vector<vector<float>>& input_data) const
{
	/* “ü—Í‘w‚©‚ç1‘w–Ú‚Ö‚ÌŒvZ */
	vector<vector<float>> a1 = Math::dot(input_data, W[0]);
	for (auto& p : a1)
		p = p + b[0];
	vector<vector<float>> z1 = a1;
	for (auto& p : z1)
		for (auto& q : p)
			q = Math::sigmoid(q);

	/* 1‘w–Ú‚©‚ço—Í‘w‚Ö‚ÌŒvZ */
	vector<vector<float>> a2 = Math::dot(z1, W[1]);
	for (auto& p : a2)
		p = p + b[1];
	vector<vector<float>> output_data = a2;
	for (int i = 0; i < (int)a2.size(); i++) {
		for (int j = 0; j < (int)a2[0].size(); j++) {
			output_data[i][j] = Math::softmax(a2[i], j);
		}
	}

	cout << "output:\n";
	for (auto& p : output_data){
		for (auto& q : p)
			cout << q << " ";
		cout << endl;
	}

	return output_data;
}

vector<float> MultiLayerPerceptron::loss(vector<vector<float>>& input_data, vector<int>& train_data) const
{
	vector<vector<float>> output_data = predict(input_data);
	vector<float> result;

	for (int i = 0; i < (int)input_data.size(); i++) {
		float e = Math::CrossEntropyLoss(output_data[i], train_data[i]);
		result.push_back(e);
	}

	return result;
}

//float MultiLayerPerceptron::loss(vector<float>& input_data, int& train_data) const
//{
//	vector<float> output_data = predict(input_data);
//
//	return Math::CrossEntropyEroor(output_data, train_data);
//}

float MultiLayerPerceptron::accuracy(vector<vector<float>>& input_data, vector<int>& train_data) const
{
	vector<vector<float>> output_data = predict(input_data);

	int cnt = 0;
	for (int i = 0; i < (int)input_data.size(); i++) {
		auto max_o = max_element(output_data[i].begin(), output_data[i].end());

		if (max_o - output_data[i].begin() == train_data[i])
			cnt++;
	}

	return (float)cnt / (float)input_data.size();
}

//float MultiLayerPerceptron::accuracy(vector<vector<float>>& input_data, vector<int>& train_data) const
//{
//	vector<vector<float>> output_data;
//	for (auto x : input_data)
//		output_data.push_back(predict(x));
//
//	int cnt = 0;
//	for (int i = 0; i < (int)input_data.size(); i++) {
//		auto max_o = max_element(output_data[i].begin(), output_data[i].end());
//
//		if (max_o - output_data[i].begin() == train_data[i])
//			cnt++;
//	}
//
//	return (float)cnt / (float)input_data.size();
//}

void MultiLayerPerceptron::gradient(const vector<vector<float>>& input_data, const vector<int>& train_data)
{
	/*========== forward ==========*/
	/* “ü—Í‘w‚©‚ç1‘w–Ú‚Ö‚ÌŒvZ */
	vector<vector<float>> a1 = Math::dot(input_data, W[0]);
	for (auto& p : a1)
		p = p + b[0];
	vector<vector<float>> z1 = a1;
	for (auto& p : z1)
		for (auto& q : p)
			q = Math::sigmoid(q);

	/* 1‘w–Ú‚©‚ço—Í‘w‚Ö‚ÌŒvZ */
	vector<vector<float>> a2 = Math::dot(z1, W[1]);
	for (auto& p : a2)
		p = p + b[1];
	vector<vector<float>> output_data = a2;
	for (int i = 0; i < (int)a2.size(); i++) {
		for (int j = 0; j < (int)a2[0].size(); j++) {
			output_data[i][j] = Math::softmax(a2[i], j);
		}
	}

	cout << "output:\n";
	for (auto& p : output_data) {
		for (auto& q : p)
			cout << q << " ";
		cout << endl;
	}

	/*========== backward ==========*/
	/* Softmax with Loss Layer */
	vector<vector<float>> dl = output_data;
	for (int i = 0; i < (int)input_data.size(); i++) {
		dl[i][train_data[i]]--;
	}
	cout << "\ndl:\n";
	for (auto& p : dl) {
		for (auto& q : p)
			cout << q << " ";
		cout << endl;
	}

	/* Affine Layer */
	dW[1] = Math::dot(Math::transpose(z1), dl);
	cout << "\ndW1:\n";
	for (auto& p : dW[1]) {
		for (auto& q : p)
			cout << q << " ";
		cout << endl;
	}
}

//vector<float> MultiLayerPerceptron::gradient(vector<vector<float>>& input_data, vector<int>& train_data) const
//{
//
//}
