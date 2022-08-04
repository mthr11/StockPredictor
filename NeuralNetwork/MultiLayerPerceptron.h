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

	vector<vector<float>> predict(vector<vector<float>>& input_data) const;	// ���_���s��
	/* �����֐� */
	vector<float> loss(vector<vector<float>>& input_data, vector<int>& train_data) const;
	//float loss(vector<float>& input_data, int& train_data) const;	// �P���f�[�^���P��̃��x���̏ꍇ
	/* ���x�����߂� */
	float accuracy(vector<vector<float>>& input_data, vector<int>& train_data) const;
	//float accuracy(vector<vector<float>>& input_data, vector<int>& train_data) const;	// �P���f�[�^���P��̃��x���̏ꍇ
	/* �덷�t�`�d�@ */
	void gradient(const vector<vector<float>>& input_data, const vector<int>& train_data);
	//vector<float> gradient(vector<vector<float>>& input_data, vector<int>& train_data) const;	// �P���f�[�^���P��̃��x���̏ꍇ
private:
	///* ���t�f�[�^�ƕ]���f�[�^���ꂼ��̓��͒l�Əo�͒l */
	//vector<vector<float>> x_train;
	//vector<float> t_train;	// �������x���̂�(not one-hot)
	//vector<vector<float>> x_test;
	//vector<float> t_test;	// �������x���̂�(not one-hot)

	/* �e�w�̏d�݂ƃo�C�A�X */
	vector<vector<vector<float>>> W;
	vector<vector<float>> b;

	/* ���z */
	vector<vector<vector<float>>> dW;
	vector<vector<float>> db;

	/* �e�w�̐� */
	int input_size;
	int hidden_size;
	int output_size;
};
