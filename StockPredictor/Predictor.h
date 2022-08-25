#pragma once
#include <iostream>
#include <vector>

using namespace std;

bool is_number(const string& str);
void to_lower(string& s);

class Predictor
{
public:
	enum EState {
		EInit,
		EAgain,
		EQuit
	};

	// コンストラクタとデストラクタ
	Predictor();
	~Predictor();

	// ゲッターとセッター
	EState getstate() { return state; }
	void setstate(EState st) { state = st; }

	// 学習と推論を行う
	int learn_and_predict(const string& api_key, const string& symbol, const float& percent, const int& day);

	// メモリ解放
	void removeMultiLayerPerceptron();
	void removeDataGenerator();
private:
	EState state;

	// 訓練データ, 評価データ, ミニバッチ
	vector<vector<float>> x_train, x_test, x_batch;
	vector<int> t_train, t_test, t_batch;

	class MultiLayerPerceptron* nnet;
	class DataGenerator* dg;
};

