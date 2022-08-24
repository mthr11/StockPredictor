#include "Predictor.h"
#include "MultiLayerPerceptron.h"
#include "DataGenerator.h"
#include <iomanip>
#include <ios>

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
	int epoch = 500;
	int batch_size = 16;

	if (state == EState::EInit) {
		nnet = new MultiLayerPerceptron(4, 20, 2);
		nnet->set_learning_rate(0.1f);

		dg = new DataGenerator(api_key, symbol, percent, day, batch_size);

		cout << "\nCalling API now...\n";

		if (!dg->generate_from_api(x_train, t_train, x_test, t_test)) {
			return 0;
		}
		//if (!dg->generate_from_file(x_train, t_train, x_test, t_test)) {
		//	return 0;
		//}
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

	cout << "\nLearning now...\n\n";

	int iter_per_epoch = t_train.size() / batch_size;	// 1エポックあたりの学習回数

	float loss, train_prec, test_prec;
	loss = train_prec = test_prec = 0.f;
	int cnt = 0;

	if (true) {
		cout << fixed << setprecision(4);

		/* 学習 */
		int i;
		for (i = 0; i < iter_per_epoch * epoch; i++) {
			dg->generate_minibatch(x_train, t_train, x_batch, t_batch, batch_size);
			
			nnet->gradient(x_batch, t_batch);
			nnet->gradient_descent();

			if (!(i % iter_per_epoch)) {
				cout << "*";
				if ((i / iter_per_epoch + 1) % 50 == 0) cout << "\n";

				loss = nnet->loss(x_batch, t_batch);
				train_prec = nnet->precision(x_batch, t_batch);
				test_prec = nnet->precision(x_test, t_test);

				//cout << "\nEpoch: " << i / iter_per_epoch + 1 << "\n";
				//cout << "\nAccuracy(train)\tPrecision(train)\tAccuracy(test)\tPrecision(test)\n";
				//cout << setfill(' ') << setw(7) << loss;
				//cout << setfill(' ') << setw(7) << nnet->accuracy(x_batch, t_batch);
				//cout << setfill(' ') << setw(7) << train_prec;
				//cout << setfill(' ') << setw(7) << nnet->accuracy(x_test, t_test);
				//cout << setfill(' ') << setw(7) << test_prec << endl;
				
				if (loss < 0.3f) cnt++;
			}

			if (cnt >= 5) {
				break;
			}
		}

		/* 推論結果出力 */
		vector<vector<float>> today = { dg->getdaily()[0] };
		vector<vector<float>> result = nnet->predict(today);

		cout << "\n\n======RESULT======";
		cout << "\nBetter to buy:    \t" << setfill(' ') << right << setw(8) << result[0][1] * 100 << "%";
		cout << "\nNot better to buy:\t" << setfill(' ') << right << setw(8) << result[0][0] * 100 << "%";
		cout << "\n(Epoch: " << min(epoch, i / iter_per_epoch + 1) << ", Precision: " << test_prec * 100 << "%)" << endl;
	}

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
