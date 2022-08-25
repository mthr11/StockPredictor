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

	// �R���X�g���N�^�ƃf�X�g���N�^
	Predictor();
	~Predictor();

	// �Q�b�^�[�ƃZ�b�^�[
	EState getstate() { return state; }
	void setstate(EState st) { state = st; }

	// �w�K�Ɛ��_���s��
	int learn_and_predict(const string& api_key, const string& symbol, const float& percent, const int& day);

	// ���������
	void removeMultiLayerPerceptron();
	void removeDataGenerator();
private:
	EState state;

	// �P���f�[�^, �]���f�[�^, �~�j�o�b�`
	vector<vector<float>> x_train, x_test, x_batch;
	vector<int> t_train, t_test, t_batch;

	class MultiLayerPerceptron* nnet;
	class DataGenerator* dg;
};

