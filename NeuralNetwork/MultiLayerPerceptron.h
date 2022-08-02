#pragma once
#include <vector>

using namespace std;

class MultiLayerPerceptron
{
public:
	/* コンストラクタとデストラクタ */
	MultiLayerPerceptron(int input_size, int hidden_size, int output_size);
	~MultiLayerPerceptron();

	vector<float> predict(vector<float>& data);	// 推論を行う
	float loss(vector<float>& input_data, vector<float>& train_data);	// 損失関数の値を計算する
	float accuracy(vector<float>& input_data, vector<float>& train_data);	// 予測精度を求める
private:
	/* 各層の重みとバイアス */
	vector<vector<vector<float>>> W;
	vector<vector<float>> b;
};
