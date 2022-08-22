#include "MultiLayerPerceptron.h"
#include "DataGenerator.h"
#include "Predictor.h"
#include <iostream>

int main(void)
{
	Predictor* pred = new Predictor();

	string input;
	string api_key, symbol;
	float percent;
	int day;

	while (pred->getstate() != Predictor::EQuit) {
		if (pred->getstate() == Predictor::EInit) {
			cout << "\nAPI KEY > ";
			cin >> api_key;
			cout << "SYMBOL > ";

			to_lower(api_key);

			/* API KEYがdemoの場合 */
			if (api_key == "demo") {
				symbol = "IBM";
				cout << symbol << endl;
			}
			else {
				cin >> symbol;
			}

			cout << "\nPredicts whether the stock price will rise X % in D days.\nD > ";

			while (1) {	// エラー処理
				cin >> input;
				if (is_number(input)) {
					day = stoi(input);
					if (day < 1 || 30 < day) {
						cout << "Error: Please enter a number from 1 to 30.\n> ";
						continue;
					}
					break;
				}
				else {
					cout << "Error: Please enter a number.\n> ";
				}
			}

			cout << "X > ";

			while (1) {	// エラー処理
				cin >> input;
				if (is_number(input)) {
					percent = stof(input);
					if (percent < 1 || 100 < percent) {
						cout << "Error: Please enter a number from 1 to 100.\n> ";
						continue;
					}
					break;
				}
				else {
					cout << "Error: Please enter a number.\n> ";
				}
			}
		}

		if (!pred->learn_and_predict(api_key, symbol, percent, day)) {
			pred->setstate(Predictor::EInit);
			continue;
		}

		cout << "\nAgain?\n[1]Continue(same settings) [2]Change the settings [3]Quit\n> ";

		while (1) {	// エラー処理と選択肢処理
			cin >> input;
			if (is_number(input)) {
				int tmp = stoi(input);
				if (tmp < 1 || 3 < tmp) {
					cout << "Error: Please enter a number from 1 to 3.\n> ";
					continue;
				}
				else if(tmp == 1){
					pred->setstate(Predictor::EAgain);
				}
				else if (tmp == 2) {
					pred->removeMultiLayerPerceptron();
					pred->removeDataGenerator();
					pred->setstate(Predictor::EInit);
				}
				else if (tmp == 3) {
					pred->setstate(Predictor::EQuit);
				}
				break;
			}
			else {
				cout << "Error: Please enter a number.\n> ";
			}
		}
	}

	delete pred;
	
	return 0;
}
