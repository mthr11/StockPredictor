#pragma once
#include <vector>
#include <string>

using namespace std;

// 各種データの読み込み
int load_data(const string file_name, vector<vector<float>>& x, vector<int>& t);
// ミニバッチの取得
void load_batch(const vector<vector<float>>& src_x, const vector<int>& src_t,
		vector<vector<float>>& dst_x, vector<int>& dst_t, const int batch_size);

class MultiLayerPerceptron
{
public:
	// コンストラクタとデストラクタ
	MultiLayerPerceptron(int input, int hidden, int output);
	~MultiLayerPerceptron();

	void set_learning_rate(const float& lr) { learning_rate = lr; }

	// 推論を行う
	vector<vector<float>> predict(const vector<vector<float>>& input_data);
	// 損失関数
	float loss(const vector<vector<float>>& input_data, const vector<int>& train_data);
	// 精度を求める
	float accuracy(const vector<vector<float>>& input_data, const vector<int>& train_data);
	// 誤差逆伝播法
	void gradient(const vector<vector<float>>& input_data, const vector<int>& train_data);
	// 数値微分(誤差逆伝播法の勾配確認用)
	void numerical_gradient(const vector<vector<float>>& input_data, const vector<int>& train_data, vector<float>& x);
	// 勾配降下法
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
