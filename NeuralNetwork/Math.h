#pragma once
#include <cmath>
#include <algorithm>
#include <numeric>
#include <vector>

using namespace std;

/*========== 演算子オーバーロード ==========*/

/* ベクトルの和を求める */
template <class T>
vector<T> operator+(const vector<T>& a, const vector<T>& b) {
	vector<T> result = a;

	for (int i = 0; i < (int)a.size(); i++) {
		result[i] += b[i];
	}

	return result;
}
	
class Math
{
public:
	/* シグモイド関数 */
	static float sigmoid(const float& x) {
		return 1 / (1 + exp(-x));
	}

	/* ソフトマックス関数 */
	static float softmax(const vector<float>& a, const int& index) {
		float c = *max_element(a.begin(), a.end());
		vector<float> exp_a = a;

		for (auto& b : exp_a)
			b = exp(b - c);

		return exp_a[index] / accumulate(exp_a.begin(), exp_a.end(), 0.f);
	}

	/* 交差エントロピー誤差(訓練データが単一のラベルの場合) */
	static float CrossEntropyEroor(const vector<float>& y, const int& t) {
		return -log(y[t] + FLT_MIN);
	}

	/* 交差エントロピー誤差(訓練データがone-hotの場合) */
	static float CrossEntropyEroor(const vector<float>& y, const vector<int>& t) {
		auto itr = find(t.begin(), t.end(), 1);
		return CrossEntropyEroor(y, itr - t.begin());
	}

	/* [行ベクトル] * [行列]を求める */
	static vector<float> dot(const vector<float>& a, const vector<vector<float>>& b) {
		vector<float> result;

		for (int i = 0; i < (int)b[0].size(); i++) {
			float tmp = 0;
			for (int j = 0; j < (int)b.size(); j++) {
				tmp += a[j] * b[j][i];
			}
			result.push_back(tmp);
		}

		return result;
	}
};
