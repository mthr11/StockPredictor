#include "MultiLayerPerceptron.h"
#include "DataGenerator.h"
#include <nlohmann/json.hpp>
#include <curl.h>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace nlohmann;

bool is_number(const string& str)
{
	for (char const& c : str) {
		if (isdigit(c) == 0) return false;
	}
	return true;
}

void to_lower(string& s) {
	transform(s.begin(), s.end(), s.begin(), tolower);
}

int learn_and_predict(const string& api_key, const string& symbol, const float& percent, const int& day)
{
	/* 訓練データ, 評価データ, ミニバッチ */
	vector<vector<float>> x_train, x_test, x_batch;
	vector<int> t_train, t_test, t_batch;

	int epoch = 2;
	int batch_size = 20;

	MultiLayerPerceptron* nnet = new MultiLayerPerceptron(5, 8, 2);
	nnet->set_learning_rate(0.1f);

	DataGenerator* dg = new DataGenerator(api_key, symbol, percent, day);
	if (!dg->generate_from_api(x_train, t_train, x_test, t_test)) {
		return 0;
	}
	//if (!dg->generate_from_file(x_train, t_train, x_test, t_test)) {
	//	return 0;
	//}

	if (!true) {
		int i = 0;
		for (auto p : x_train) {
			cout << i + 1 << ":\t";
			for (auto q : p)
				cout << setfill(' ') << setw(10) << q << " ";
			cout << t_train[i] << endl;
			i++;
		}
	}
	if (!true) {
		int i = 0;
		for (auto p : x_test) {
			cout << i + 1 << ":\t";
			for (auto q : p)
				cout << setfill(' ') << setw(10) << q << " ";
			cout << t_test[i] << endl;
			i++;
		}
	}

	cout << "\nLearning now...\n";

	int iter_per_epoch = t_train.size() / batch_size;	// 1エポックあたりの学習回数

	if (true) {
		for (int i = 0; i < iter_per_epoch * epoch; i++) {
			dg->generate_minibatch(x_train, t_train, x_batch, t_batch, batch_size);

			nnet->gradient(x_batch, t_batch);
			nnet->gradient_descent();

			if (!(i % iter_per_epoch)) {
				cout << "\nEpoch: " << i / iter_per_epoch + 1;
				cout << "\nAccuracy(train): " << nnet->accuracy(x_batch, t_batch);
				cout << "\nPrecision(train): " << nnet->precision(x_batch, t_batch);
				cout << "\nAccuracy(test): " << nnet->accuracy(x_test, t_test);
				cout << "\nPrecision(test): " << nnet->precision(x_test, t_test) << endl;
			}
		}
	}

	cout << "\nComplete\n" << endl;

	delete dg;
	delete nnet;

	return 1;
}

enum EState {
	EInit,
	EAgain,
	EQuit
};

int main(void) {

	EState state = EState::EInit;
	string input;
	string api_key, symbol;
	float percent;
	int day;

	while (state != EState::EQuit) {
		if (state == EState::EInit) {
			cout << "API KEY > ";
			cin >> api_key;
			cout << "SYMBOL > ";
			to_lower(api_key);
			if (api_key == "demo") {
				symbol = "IBM";
				cout << symbol << endl;
			}
			else {
				cin >> symbol;
			}
			cout << "\nPredicts whether the stock price will rise XX % in YY days.\nXX > ";
			while (1) {
				cin >> input;
				if (is_number(input)) {
					percent = stof(input);
					break;
					// エラー処理：1～100%
				}
				else {
					cout << "Error: Please enter a number.\n> ";
				}
			}
			cout << "YY > ";
			while (1) {
				cin >> input;
				if (is_number(input)) {
					day = stoi(input);
					break;
					// エラー処理：1～300日
				}
				else {
					cout << "Error: Please enter a number.\n> ";
				}
			}
		}

		if (!learn_and_predict(api_key, symbol, percent, day)) {
			state = EState::EInit;
			continue;
		}

		cout << "Again?\n[1]Continue(same settings) [2]Change the settings [3]Quit\n> ";
		while (1) {
			cin >> input;
			if (is_number(input)) {
				int tmp = stoi(input);
				if (tmp < 1 || 3 < tmp) {
					cout << "Error: Please enter a number from 1 to 3.\n> ";
					continue;
				}
				else if(tmp == 1){
					state = EState::EAgain;
				}
				else if (tmp == 2) {
					state = EState::EInit;
				}
				else if (tmp == 3) {
					state = EState::EQuit;
				}
				break;
			}
			else {
				cout << "Error: Please enter a number.\n> ";
			}
		}

		cout << "\n";
	}

	cout << "\nThank you." << endl;
	
	while (1);
	return 0;
}
