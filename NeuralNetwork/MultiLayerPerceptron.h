#pragma once
#include <vector>
#include <unordered_map>

using namespace std;

class MultiLayerPerceptron
{
public:
	/* コンストラクタとデストラクタ */
	MultiLayerPerceptron(int input, int hidden, int output);
	~MultiLayerPerceptron();

	/* 推論を行う */
	vector<vector<float>> predict(const vector<vector<float>>& input_data);
	/* 損失関数 */
	float loss(const vector<vector<float>>& input_data, const vector<int>& train_data);
	/* 精度を求める */
	float accuracy(const vector<vector<float>>& input_data, const vector<int>& train_data);
	/* 誤差逆伝播法 */
	void gradient(const vector<vector<float>>& input_data, const vector<int>& train_data);
	/* 数値微分(誤差逆伝播法の勾配確認用) */
	void numerical_gradient(const vector<vector<float>>& input_data, const vector<int>& train_data, vector<float>& x);
	/* 勾配降下法 */
	void gradient_descent();
private:
	/* 各層の重みとバイアス */
	vector<vector<vector<float>>> W;
	vector<vector<float>> b;

	/* 勾配 */
	vector<vector<vector<float>>> dW;
	vector<vector<float>> db;

	float learning_rate;	// 学習率

	/* 各層の数 */
	int input_size;
	int hidden_size;
	int output_size;
};
