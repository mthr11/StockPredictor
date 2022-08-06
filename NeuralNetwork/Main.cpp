#include "MultiLayerPerceptron.h"
#include <iostream>

int main(void) {
	/* 訓練データ, 評価データ, ミニバッチ */
	vector<vector<float>> x_train, x_test, x_batch;
	vector<int> t_train, t_test, t_batch;

	int epoch = 50;
	int batch_size = 30;

	MultiLayerPerceptron* nnet = new MultiLayerPerceptron(4, 5, 3);

	if (!(load_data("iris_train.txt", x_train, t_train) && load_data("iris_test.txt", x_test, t_test)))
		return 0;

	int iter_per_epoch = t_train.size() / batch_size;

	for (int i = 0; i < iter_per_epoch * epoch; i++) {
		load_batch(x_train, t_train, x_batch, t_batch, batch_size);

		nnet->gradient(x_batch, t_batch);
		nnet->gradient_descent();

		if (!(i % iter_per_epoch)) {
			//cout << "Loss: " << nnet->loss(x_train, t_train);
			cout << "\nAccuracy(train): " << nnet->accuracy(x_train, t_train) << endl;
			cout << "Accuracy(test): " << nnet->accuracy(x_test, t_test) << endl;
		}
	}

	cout << "\nComplete" << endl;

	while (1);
	return 0;
}