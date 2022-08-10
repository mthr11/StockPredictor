#pragma once
#include <cmath>
#include <algorithm>
#include <random>
#include <numeric>
#include <vector>

using namespace std;

/*========== ���Z�q�I�[�o�[���[�h ==========*/

/* �x�N�g���̘a�����߂� */
template <class T>
vector<T> operator+(const vector<T>& a, const vector<T>& b) {
	vector<T> result = a;

	for (int i = 0; i < (int)a.size(); i++) {
		result[i] += b[i];
	}

	return result;
}

inline static double get_randf(const double min, const double max) {
	/* �[������(�����Z���k�E�c�C�X�^)�̐��� */
	random_device rnd;
	mt19937 mt(rnd());
	uniform_real_distribution<> rndf(min, max);

	return rndf(mt);
}

inline static int get_randi(const int min, const int max) {
	/* �[������(�����Z���k�E�c�C�X�^)�̐��� */
	random_device rnd;
	mt19937 mt(rnd());
	uniform_int_distribution<> rndi(min, max);

	return rndi(mt);
}

	
class Math
{
public:
	/* �V�O���C�h�֐� */
	static float sigmoid(const float& x) {
		return 1 / (1 + exp(-x));
	}

	/* �\�t�g�}�b�N�X�֐� */
	static float softmax(const vector<float>& a, const int& index) {
		float c = *max_element(a.begin(), a.end());
		vector<float> exp_a = a;

		for (auto& b : exp_a)
			b = exp(b - c);

		return exp_a[index] / accumulate(exp_a.begin(), exp_a.end(), 0.f);
	}

	/* �����G���g���s�[�덷(�P���f�[�^���P��̃��x���̏ꍇ) */
	static float cross_entropy_loss(const vector<float>& y, const int& t) {
		float delta = 1e-7f;
		return -log(y[t] + delta);
	}

	/* �s��� */
	static vector<vector<float>> dot(const vector<vector<float>>& a, const vector<vector<float>>& b) {
		vector<vector<float>> result(a.size());

		for (int i = 0; i < (int)b[0].size(); i++) {
			for (int j = 0; j < (int)a.size(); j++) {
				float tmp = 0;
				for (int k = 0; k < (int)b.size(); k++) {
					tmp += a[j][k] * b[k][i];
				}
				result[j].push_back(tmp);
			}
		}

		return result;
	}

	/* �]�u�s�� */
	static vector<vector<float>> transpose(const vector<vector<float>>& x) {
		vector<vector<float>> result(x[0].size());

		for (int i = 0; i < (int)x[0].size(); i++) {
			for (int j = 0; j < (int)x.size(); j++) {
				result[i].push_back(x[j][i]);
			}
		}

		return result;
	}
};
