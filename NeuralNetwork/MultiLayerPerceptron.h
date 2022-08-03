#pragma once
#include <vector>

using namespace std;

class MultiLayerPerceptron
{
public:
	/* �R���X�g���N�^�ƃf�X�g���N�^ */
	MultiLayerPerceptron(int input_size, int hidden_size, int output_size);
	~MultiLayerPerceptron();

	vector<float> predict(vector<float>& input_data);	// ���_���s��
	float loss(vector<float>& input_data, vector<int>& train_data);	// �����֐��̒l���v�Z����
	float accuracy(vector<vector<float>>& input_data, vector<vector<int>>& train_data);	// �e���x���ɑ΂���m�������߂�
private:
	/* �e�w�̏d�݂ƃo�C�A�X */
	vector<vector<vector<float>>> W;
	vector<vector<float>> b;
};
