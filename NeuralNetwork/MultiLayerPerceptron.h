#pragma once
#include <vector>
#include <string>

using namespace std;

// �e��f�[�^�̓ǂݍ���
int load_data(const string file_name, vector<vector<float>>& x, vector<int>& t);
// �~�j�o�b�`�̎擾
void load_batch(const vector<vector<float>>& src_x, const vector<int>& src_t,
		vector<vector<float>>& dst_x, vector<int>& dst_t, const int batch_size);

class MultiLayerPerceptron
{
public:
	// �R���X�g���N�^�ƃf�X�g���N�^
	MultiLayerPerceptron(int input, int hidden, int output);
	~MultiLayerPerceptron();

	void set_learning_rate(const float& lr) { learning_rate = lr; }

	// ���_���s��
	vector<vector<float>> predict(const vector<vector<float>>& input_data);
	// �����֐�
	float loss(const vector<vector<float>>& input_data, const vector<int>& train_data);
	// ���x�����߂�
	float accuracy(const vector<vector<float>>& input_data, const vector<int>& train_data);
	// �덷�t�`�d�@
	void gradient(const vector<vector<float>>& input_data, const vector<int>& train_data);
	// ���l����(�덷�t�`�d�@�̌��z�m�F�p)
	void numerical_gradient(const vector<vector<float>>& input_data, const vector<int>& train_data, vector<float>& x);
	// ���z�~���@
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
