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

	vector<vector<float>> predict(vector<vector<float>>& input_data) const;	// 推論を行う
	/* 損失関数 */
	vector<float> loss(vector<vector<float>>& input_data, vector<int>& train_data) const;
	//float loss(vector<float>& input_data, int& train_data) const;	// 訓練データが単一のラベルの場合
	/* 精度を求める */
	float accuracy(vector<vector<float>>& input_data, vector<int>& train_data) const;
	//float accuracy(vector<vector<float>>& input_data, vector<int>& train_data) const;	// 訓練データが単一のラベルの場合
	/* 誤差逆伝播法 */
	void gradient(const vector<vector<float>>& input_data, const vector<int>& train_data);
	//vector<float> gradient(vector<vector<float>>& input_data, vector<int>& train_data) const;	// 訓練データが単一のラベルの場合
private:
	///* 教師データと評価データそれぞれの入力値と出力値 */
	//vector<vector<float>> x_train;
	//vector<float> t_train;	// 正解ラベルのみ(not one-hot)
	//vector<vector<float>> x_test;
	//vector<float> t_test;	// 正解ラベルのみ(not one-hot)

	/* 各層の重みとバイアス */
	vector<vector<vector<float>>> W;
	vector<vector<float>> b;

	/* 勾配 */
	vector<vector<vector<float>>> dW;
	vector<vector<float>> db;

	/* 各層の数 */
	int input_size;
	int hidden_size;
	int output_size;
};
