#include "Predictor.h"
#include "MultiLayerPerceptron.h"
#include "DataGenerator.h"

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

Predictor::Predictor()
	: state(EState::EInit)
{
}

Predictor::~Predictor()
{
	delete nnet;
	delete dg;
}

int Predictor::learn_and_predict(const string& api_key, const string& symbol, const float& percent, const int& day)
{
	if (state == EState::EInit) {
		nnet = new MultiLayerPerceptron(5, 20, 2);
		nnet->set_learning_rate(0.1f);

		dg = new DataGenerator(api_key, symbol, percent, day);
		//if (!dg->generate_from_api(x_train, t_train, x_test, t_test)) {
		//	return 0;
		//}
		if (!dg->generate_from_file(x_train, t_train, x_test, t_test)) {
			return 0;
		}
	}
	else if (state == EState::EAgain) {
		nnet->init_weight();
	}

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

	int epoch = 500;
	int batch_size = 16;
	int iter_per_epoch = t_train.size() / batch_size;	// 1エポックあたりの学習回数

	float prec;
	int cnt = 0;

	if (true) {
		for (int i = 0; i < iter_per_epoch * epoch; i++) {
			dg->generate_minibatch(x_train, t_train, x_batch, t_batch, batch_size);
			
			nnet->gradient(x_batch, t_batch);
			nnet->gradient_descent();

			if (!(i % iter_per_epoch)) {
				cout << "\nEpoch: " << i / iter_per_epoch + 1;
				//cout << "\nAccuracy(train)\tPrecision(train)\tAccuracy(test)\tPrecision(test)\n";
				cout << "\n" << setfill(' ') << left << setw(9) << nnet->loss(x_batch, t_batch);
				cout << setfill(' ') << left << setw(9) << nnet->accuracy(x_batch, t_batch);
				prec = nnet->precision(x_batch, t_batch);
				if (prec > 0.9) cnt++;
				cout << setfill(' ') << left << setw(9) << prec;
				cout << setfill(' ') << left << setw(9) << nnet->accuracy(x_test, t_test);
				cout << setfill(' ') << left << setw(9) << nnet->precision(x_test, t_test) << endl;
			}

			if (cnt >= 15) {
				vector<vector<float>> today = { dg->getdaily()[0] };
				vector<vector<float>> result = nnet->predict(today);
				for (auto& e : today[0])
					cout << e << " ";
				cout << "\nResult: ";
				for (auto& e : result[0])
					cout << e << " ";
				cout << endl;
				break;
			}
		}
	}

	// 正解率・精度の平均を出す

	cout << "\nComplete\n" << endl;

	return 1;
}

void Predictor::removeMultiLayerPerceptron()
{
	delete nnet;
}

void Predictor::removeDataGenerator()
{
	delete dg;
}
