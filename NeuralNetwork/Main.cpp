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

	int epoch = 10;
	int batch_size = 15;

	MultiLayerPerceptron* nnet = new MultiLayerPerceptron(2, 10, 2);
	nnet->set_learning_rate(0.1f);

	DataGenerator* dg = new DataGenerator();
	//if (!dg->load_from_api("IBM", x_train, t_train, x_test, t_test)) {
	//	return 0;
	//}
	if (!dg->load_from_file("spy_daily_full.json", x_train, t_train, x_test, t_test)) {
		return 0;
	}

	if (true) {
		int i = 0;
		for (auto p : x_train) {
			cout << i + 1 << ":\t";
			for (auto q : p)
				cout << q << "\t";
			cout << t_train[i] << endl;
			i++;
		}
	}

	//if (!(load_data("iris_train.txt", x_train, t_train) && load_data("iris_test.txt", x_test, t_test)))
	//	return 0;

	int iter_per_epoch = t_train.size() / batch_size;

	if (!true) {
		for (int i = 0; i < iter_per_epoch * epoch; i++) {
			load_batch(x_train, t_train, x_batch, t_batch, batch_size);

			nnet->gradient(x_batch, t_batch);
			nnet->gradient_descent();

			if (!(i % iter_per_epoch)) {
				//cout << "Loss: " << nnet->loss(x_train, t_train);
				cout << "\nEpoch: " << i / iter_per_epoch + 1;
				cout << "\nAccuracy(train): " << nnet->accuracy(x_train, t_train);
				cout << "\nAccuracy(test): " << nnet->accuracy(x_test, t_test) << endl;
			}
		}
	}

	if (true) {
		for (int i = 0; i < 10 * epoch; i++) {
			nnet->gradient(x_train, t_train);
			nnet->gradient_descent();

			if (!(i % 10)) {
				//cout << "Loss: " << nnet->loss(x_train, t_train);
				cout << "\nEpoch: " << i / 10 + 1;
				cout << "\nAccuracy(train): " << nnet->accuracy(x_train, t_train);
				cout << "\nPrecision(train): " << nnet->precision(x_train, t_train);
			}
		}
	}

	cout << "\nComplete" << endl;

	//vector<char> responseData;

	//std::ofstream ofs;
	//ofs.open("tmp.json", std::ios::out);
	//if (ofs.is_open()) {
	//	ofs << responseData.data() << std::endl;
	//}

	while (1);
	return 0;
}
