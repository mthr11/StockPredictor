#include "MultiLayerPerceptron.h"
#include "DataGenerator.h"
#include <iostream>
#include <curl.h>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace nlohmann;

int main(void) {
	/* 訓練データ, 評価データ, ミニバッチ */
	vector<vector<float>> x_train, x_test, x_batch;
	vector<int> t_train, t_test, t_batch;

	int epoch = 20;
	int batch_size = 20;

	MultiLayerPerceptron* nnet = new MultiLayerPerceptron(5, 8, 2);
	nnet->set_learning_rate(0.1f);

	DataGenerator* dg = new DataGenerator();
	//if (!dg->generate_from_api("MSFT", x_train, t_train, x_test, t_test)) {
	//	return 0;
	//}
	if (!dg->generate_from_file("spy", x_train, t_train, x_test, t_test)) {
		return 0;
	}

	if (true) {
		int i = 0;
		for (auto p : x_train) {
			cout << i + 1 << ":\t";
			for (auto q : p)
				cout << setfill(' ') << setw(10) << q << " ";
			cout << t_train[i] << endl;
			i++;
		}
	}
	if (true) {
		int i = 0;
		for (auto p : x_test) {
			cout << i + 1 << ":\t";
			for (auto q : p)
				cout << setfill(' ') << setw(10) << q << " ";
			cout << t_test[i] << endl;
			i++;
		}
	}

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

	cout << "\nComplete" << endl;

	delete dg;
	delete nnet;

	while (1);
	return 0;
}
