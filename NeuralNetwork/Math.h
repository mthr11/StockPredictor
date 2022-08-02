#pragma once
#include <cmath>
#include <algorithm>
#include <numeric>
#include <vector>

using namespace std;

class Math
{
public:
	static float sigmoid(const float& x) {
		return 1 / (1 + exp(-x));
	}

	static float softmax(const vector<float>& a, const int& index) {
		float c = *max_element(a.begin(), a.end());
		vector<float> exp_a = a;

		for (auto& b : exp_a) {
			b = exp(b - c);
		}

		return exp_a[index] / accumulate(exp_a.begin(), exp_a.end(), 0.f);
	}

	/* [ベクトル] * [行列]を求める */
	static vector<float> dot(const vector<float>& a, const vector<vector<float>>& b) {
		vector<float> result = vector<float>(b[0].size());

		for (int i = 0; i < (int)b[0].size(); i++) {
			float tmp = a[0] * b[0][i] + a[1] * b[1][i];
			result.push_back(tmp);
		}

		return result;
	}
};
