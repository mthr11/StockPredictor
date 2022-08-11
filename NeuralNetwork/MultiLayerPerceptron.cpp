#include "MultiLayerPerceptron.h"
#include "Math.h"
#include <iostream>
#include <fstream>
#include <sstream>

int load_data(const string file_name, vector<vector<float>>& x, vector<int>& t)
{
	fstream ifs(file_name);

	if (!ifs) {
		cout << "Failed to file open." << endl;
		return 0;
	}

	int i = 0;
	string str;

	/* 1s“Ç‚İ‚İ */
	while (getline(ifs, str)) {
		int j = 0;
		string token;
		stringstream ss(str);

		x.push_back(vector<float>());

		/* ‹ó”’‹æØ‚è‚Å“Ç‚İ‚İ */
		while (getline(ss, token, ' ')) {
			float tmp = stof(token);
			if (j < 4) {
				x[i].push_back(tmp);
				//cout << x[i].back() << "\t";
			}
			else {
				tmp--;
				t.push_back((int)tmp);
				//cout << t.back() << endl;
			}
			j++;
		}
		i++;
	}

	return 1;
}

void load_batch(const vector<vector<float>>& src_x, const vector<int>& src_t,
		vector<vector<float>>& dst_x, vector<int>& dst_t, const int batch_size)
{
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < batch_size / 3; i++) {
			int r = get_randi(0, src_t.size() / 3 - 1);
			r += src_t.size() / 3 * j;

			dst_x.push_back(src_x[r]);
			dst_t.push_back(src_t[r]);
			//cout << r << " " << dst_x.back().back() << " " << dst_t.back() << endl;
		}
	}
}

MultiLayerPerceptron::MultiLayerPerceptron(int input, int hidden, int output)
	:learning_rate(0.1f)
{
	input_size = input;
	hidden_size = hidden;
	output_size = output;

	/* ƒƒ“ƒo•Ï”‰Šú‰» */
	W = vector<vector<vector<float>>>(2);
	W[0] = vector<vector<float>>(input_size);
	W[1] = vector<vector<float>>(hidden_size);

	b = vector<vector<float>>(2);
	b[0] = vector<float>(hidden_size, 0);
	b[1] = vector<float>(output_size, 0);

	dW = vector<vector<vector<float>>>(2);
	db = vector<vector<float>>(2);

	/* d‚İ‚ğ—”‚Å‰Šú‰» */
	for(int i = 0;i<input_size;i++){
		for (int j = 0; j < hidden_size; j++) {
			float r = (float)get_randf(-1, 1);
			W[0][i].push_back(r);
			//cout << r << " ";
		}
		//cout << endl;
	}
	//cout << endl;
	for (int i = 0; i < hidden_size; i++) {
		for (int j = 0; j < output_size; j++) {
			float r = (float)get_randf(-1, 1);
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
	/* “ü—Í‘w‚©‚ç1‘w–Ú‚Ö‚ÌŒvZ */
	vector<vector<float>> a1 = Math::dot(input_data, W[0]);
	for (auto& p : a1)
		p = p + b[0];
	vector<vector<float>> z1 = a1;
	for (auto& p : z1)
		for (auto& q : p)
			q = Math::sigmoid(q);

	/* 1‘w–Ú‚©‚ço—Í‘w‚Ö‚ÌŒvZ */
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
		sum += Math::cross_entropy_loss(output_data[i], train_data[i]);

	return sum / (float)input_data.size();	// ƒf[ƒ^1‚Â‚ ‚½‚è‚ÌŒë·‚ğ‹‚ß‚é
}

float MultiLayerPerceptron::accuracy(const vector<vector<float>>& input_data, const vector<int>& train_data)
{
	vector<vector<float>> output_data = predict(input_data);

	int cnt_tptn = 0;
	for (int i = 0; i < (int)input_data.size(); i++) {
		auto max_o = max_element(output_data[i].begin(), output_data[i].end());

		if (max_o - output_data[i].begin() == train_data[i])
			cnt_tptn++;
	}

	return (float)cnt_tptn / (float)input_data.size();
}

float MultiLayerPerceptron::precision(const vector<vector<float>>& input_data, const vector<int>& train_data)
{
	vector<vector<float>> output_data = predict(input_data);

	int cnt_tp = 0;
	int cnt_tpfp = 0;
	for (int i = 0; i < (int)input_data.size(); i++) {
		auto max_o = max_element(output_data[i].begin(), output_data[i].end());

		if (max_o - output_data[i].begin() == 1)
			cnt_tpfp++;
		if (max_o - output_data[i].begin() == 1 && train_data[i] == 1)
			cnt_tp++;
	}

	return (float)cnt_tp / (float)cnt_tpfp;
}

void MultiLayerPerceptron::gradient(const vector<vector<float>>& input_data, const vector<int>& train_data)
{
	/*========== forward ==========*/
	/* “ü—Í‘w‚©‚ç1‘w–Ú‚Ö‚ÌŒvZ */
	vector<vector<float>> a1 = Math::dot(input_data, W[0]);
	for (auto& p : a1)
		p = p + b[0];
	vector<vector<float>> z1 = a1;
	for (auto& p : z1)
		for (auto& q : p)
			q = Math::sigmoid(q);

	/* 1‘w–Ú‚©‚ço—Í‘w‚Ö‚ÌŒvZ */
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

	if (!true) {
		cout << "\noutput:\n";
		for (auto& p : output_data) {
			for (auto& q : p)
				cout << q << " ";
			cout << endl;
		}
	}

	/*========== backward ==========*/
	/* Softmax with Loss Layer */
	vector<vector<float>> da2 = output_data;
	for (int i = 0; i < (int)input_data.size(); i++) {
		for (int j = 0; j < (int)output_data[0].size(); j++) {
			if (j == train_data[i])
				da2[i][j]--;
			da2[i][j] /= (float)input_data.size();	// ƒoƒbƒ`ƒTƒCƒY‚ÅŠ„‚é
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

	/* Œù”zŠm”F */
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

	//for (auto& p : W[0]) {
	//	for (auto& q : p)
	//		cout << q << " ";
	//	cout << endl;
	//}
}
