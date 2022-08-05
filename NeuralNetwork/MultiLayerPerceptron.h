#pragma once
#include <vector>
#include <unordered_map>

using namespace std;

class MultiLayerPerceptron
{
public:
	/* �R���X�g���N�^�ƃf�X�g���N�^ */
	MultiLayerPerceptron(int input, int hidden, int output);
	~MultiLayerPerceptron();

	/* ���_���s�� */
	vector<vector<float>> predict(const vector<vector<float>>& input_data);
	/* �����֐� */
	float loss(const vector<vector<float>>& input_data, const vector<int>& train_data);
	/* ���x�����߂� */
	float accuracy(const vector<vector<float>>& input_data, const vector<int>& train_data);
	/* �덷�t�`�d�@ */
	void gradient(const vector<vector<float>>& input_data, const vector<int>& train_data);
	/* ���l����(�덷�t�`�d�@�̌��z�m�F�p) */
	void numerical_gradient(const vector<vector<float>>& input_data, const vector<int>& train_data, vector<float>& x);
	/* ���z�~���@ */
	void gradient_descent();
private:
	/* �e�w�̏d�݂ƃo�C�A�X */
	vector<vector<vector<float>>> W;
	vector<vector<float>> b;

	/* ���z */
	vector<vector<vector<float>>> dW;
	vector<vector<float>> db;

	float learning_rate;	// �w�K��

	/* �e�w�̐� */
	int input_size;
	int hidden_size;
	int output_size;
};
