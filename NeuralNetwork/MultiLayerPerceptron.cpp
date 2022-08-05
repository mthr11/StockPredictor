#include "MultiLayerPerceptron.h"
#include "Math.h"
#include <random>
#include <iostream>

MultiLayerPerceptron::MultiLayerPerceptron(int input, int hidden, int output)
	:learning_rate(0.1f)
{
	input_size = input;
	hidden_size = hidden;
	output_size = output;

	/* メンバ変数初期化 */
	W = vector<vector<vector<float>>>(2);
	b = vector<vector<float>>(2);
	dW = vector<vector<vector<float>>>(2);
	db = vector<vector<float>>(2);

	W[0] = vector<vector<float>>(input_size);
	W[1] = vector<vector<float>>(hidden_size);
	b[0] = vector<float>(hidden_size, -10);
	b[1] = vector<float>(output_size, -10);

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

	//W[0] = { {0.1f,0.2f,0.3f,0.4f,0.5f},{0.2f,0.3f,0.4f,0.5f,0.6f}, {0.3f,0.4f,0.5f,0.6f,0.7f}, {0.4f,0.5f,0.6f,0.7f,0.8f} };
	//W[1] = { {0.1f,0.2f,0.3f},{0.2f,0.3f,0.4f},{0.3f,0.4f,0.5f},{0.4f,0.5f,0.6f},{0.5f,0.6f,0.7f} };
}

MultiLayerPerceptron::~MultiLayerPerceptron()
{
}

vector<vector<float>> MultiLayerPerceptron::predict(const vector<vector<float>>& input_data)
{
	/* 入力層から1層目への計算 */
	vector<vector<float>> a1 = Math::dot(input_data, W[0]);
	for (auto& p : a1)
		p = p + b[0];
	vector<vector<float>> z1 = a1;
	for (auto& p : z1)
		for (auto& q : p)
			q = Math::sigmoid(q);

	/* 1層目から出力層への計算 */
	vector<vector<float>> a2 = Math::dot(z1, W[1]);
	for (auto& p : a2)
		p = p + b[1];
	vector<vector<float>> output_data = a2;
	for (int i = 0; i < (int)a2.size(); i++) {
		for (int j = 0; j < (int)a2[0].size(); j++) {
			output_data[i][j] = Math::softmax(a2[i], j);
		}
	}

	//cout << "output:\n";
	//for (auto& p : output_data){
	//	for (auto& q : p)
	//		cout << q << " ";
	//	cout << endl;
	//}

	return output_data;
}

float MultiLayerPerceptron::loss(const vector<vector<float>>& input_data, const vector<int>& train_data)
{
	vector<vector<float>> output_data = predict(input_data);
	float sum = 0.f;

	for (int i = 0; i < (int)input_data.size(); i++)
		sum += Math::CrossEntropyLoss(output_data[i], train_data[i]);

	return sum / (float)input_data.size();	// データ1つあたりの誤差を求める
}

float MultiLayerPerceptron::accuracy(const vector<vector<float>>& input_data, const vector<int>& train_data)
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

void MultiLayerPerceptron::gradient(const vector<vector<float>>& input_data, const vector<int>& train_data)
{
	/*========== forward ==========*/
	/* 入力層から1層目への計算 */
	vector<vector<float>> a1 = Math::dot(input_data, W[0]);
	for (auto& p : a1)
		p = p + b[0];
	vector<vector<float>> z1 = a1;
	for (auto& p : z1)
		for (auto& q : p)
			q = Math::sigmoid(q);

	/* 1層目から出力層への計算 */
	vector<vector<float>> a2 = Math::dot(z1, W[1]);
	for (auto& p : a2)
		p = p + b[1];
	vector<vector<float>> output_data = a2;
	for (int i = 0; i < (int)a2.size(); i++) {
		for (int j = 0; j < (int)a2[0].size(); j++) {
			output_data[i][j] = Math::softmax(a2[i], j);
		}
	}

	//cout << "z1:\n";
	//for (auto& p : z1) {
	//	for (auto& q : p)
	//		cout << q << " ";
	//	cout << endl;
	//}

	cout << "\noutput:\n";
	for (auto& p : output_data) {
		for (auto& q : p)
			cout << q << " ";
		cout << endl;
	}

	/*========== backward ==========*/
	/* Softmax with Loss Layer */
	vector<vector<float>> da2 = output_data;
	for (int i = 0; i < (int)input_data.size(); i++) {
		for (int j = 0; j < (int)output_data[0].size(); j++) {
			if (j == train_data[i])
				da2[i][j]--;
			da2[i][j] /= (float)input_data.size();	// バッチサイズで割る
		}
	}
	//cout << "\nda2:\n";
	//for (auto& p : da2) {
	//	for (auto& q : p)
	//		cout << q << " ";
	//	cout << endl;
	//}

	/* Affine Layer2 */
	vector<vector<float>> dz = Math::dot(da2, Math::transpose(W[1]));
	//cout << "\ndz:\n";
	//for (auto& p : dz) {
	//	for (auto& q : p)
	//		cout << q << " ";
	//	cout << endl;
	//}
	dW[1] = Math::dot(Math::transpose(z1), da2);
	//cout << "\ndW1:\n";
	//for (auto& p : dW[1]) {
	//	for (auto& q : p)
	//		cout << q << " ";
	//	cout << endl;
	//}
	db[1] = vector<float>(da2[0].size(), 0);
	for (auto& p : da2)
		db[1] = db[1] + p;
	//cout << "\ndb1:\n";
	//for (auto& p : db[1]) 
	//	cout << p << " ";
	//cout << endl;

	/* Sigmoid Layer */
	vector<vector<float>> da1 = vector<vector<float>>(z1.size());
	for (int i = 0; i < (int)z1.size(); i++) {
		for (int j = 0; j < (int)z1[0].size(); j++) {
			da1[i].push_back(dz[i][j] * z1[i][j] * (1 - z1[i][j]));
		}
	}
	//cout << "\nda1:\n";
	//for (auto& p : da1) {
	//	for (auto& q : p)
	//		cout << q << " ";
	//	cout << endl;
	//}

	/* Affine Layer1 */
	dW[0] = Math::dot(Math::transpose(input_data), da1);
	//cout << "\ndW0:\n";
	//for (auto& p : dW[0]) {
	//	for (auto& q : p)
	//		cout << q << " ";
	//	cout << endl;
	//}
	db[0] = vector<float>(da1[0].size(), 0);
	for (auto& p : da1)
		db[0] = db[0] + p;
	//cout << "\ndb0:\n";
	//for (auto& p : db[0])
	//	cout << p << " ";
	//cout << endl;

	/* 勾配確認 */
	//cout << "\nGRADIENT CHECK";
	//cout << "\ndW1" << endl;
	//for (auto& p : W[1])
	//	numerical_gradient(input_data, train_data, p);
	//cout << "\ndb1" << endl;
	//numerical_gradient(input_data, train_data, b[1]);

	//cout << "\ndW0" << endl;
	//for (auto& p : W[0])
	//	numerical_gradient(input_data, train_data, p);
	//cout << "\ndb0" << endl;
	//numerical_gradient(input_data, train_data, b[0]);
}

void MultiLayerPerceptron::numerical_gradient(const vector<vector<float>>& input_data, const vector<int>& train_data, vector<float>& x)
{
	float h = 1e-3f;
	for (auto& p : x) {
		float tmp = p;

		p = tmp + h;
		float fxh1 = loss(input_data, train_data);
		p = tmp - h;
		float fxh2 = loss(input_data, train_data);
		
		cout << (fxh1 - fxh2) / (2.f * h) << " ";
		p = tmp;
	}
	cout << endl;
}

void MultiLayerPerceptron::gradient_descent()
{
	for (int i = 0; i < (int)W.size(); i++)
		for (int j = 0; j < (int)W[i].size(); j++)
			for (int k = 0; k < (int)W[i][0].size(); k++)
				W[i][j][k] -= learning_rate * dW[i][j][k];

	for (int i = 0; i < (int)b.size(); i++)
		for (int j = 0; j < (int)b[i].size(); j++)
			b[i][j] -= learning_rate * db[i][j];
}
