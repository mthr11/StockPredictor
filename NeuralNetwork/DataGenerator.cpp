#include "DataGenerator.h"
#include "Math.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <curl.h>

DataGenerator::DataGenerator()
{
}

DataGenerator::~DataGenerator()
{
}

size_t DataGenerator::write(char* data, size_t size, size_t nmemb, string* buffer) {
	if (buffer != NULL)
		buffer->append(data, size * nmemb);

	return size * nmemb;
}

json DataGenerator::call_api_daily(const string symbol)
{
	string buf;
	string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + "&outputsize=full&&apikey=6S48KGZ8LLN8T841";
	
	/* curlのセットアップ */
	CURL* curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

	/* 実行 */
	CURLcode res = curl_easy_perform(curl);
	if (res) {
		return 0;
	}

	curl_easy_cleanup(curl);

	json j = json::parse(buf);

	if (j.size() == 1) {
		cout << j << endl;
		j = nullptr;
	}

	return j;
}

json DataGenerator::call_api_atr5(const string symbol)
{
	string buf;
	string url = "https://www.alphavantage.co/query?function=ATR&symbol=" + symbol + "&interval=daily&time_period=5&apikey=6S48KGZ8LLN8T841";

	/* curlのセットアップ */
	CURL* curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

	/* 実行 */
	CURLcode res = curl_easy_perform(curl);
	if (res) {
		return 0;
	}

	curl_easy_cleanup(curl);

	json j = json::parse(buf);

	if (j.size() == 1) {
		cout << j << endl;
		j = nullptr;
	}

	return j;
}

json DataGenerator::load_json(const string file_name)
{
	ifstream ifs(file_name);
	if (!ifs)
	{
		cout << "Failed to file open." << endl;
		return nullptr;
	}
	
	json j;
	ifs >> j;

	return j;
}

int DataGenerator::generate_from_api(const string symbol, vector<vector<float>>& x_train, vector<int>& t_train
	, vector<vector<float>>& x_test, vector<int>& t_test)
{
	int size = 300;	// 読み込むデータ数(130 = 約半年分)

	/*========== 日足データの読み込み ==========*/
	json j = call_api_daily(symbol);
	if (j == nullptr) {
		return 0;
	}

	json::iterator itr = j.begin();
	itr++;	// "Meta Data"を飛ばす

	json::reverse_iterator ritr = (*itr).rbegin(); // 日付最新順にループするための逆イテレータ

	/* 各データをvectorに格納 */
	for (int t = 0; t < size; t++) {	// 日付オブジェクトをループ
		daily.push_back(vector<float>());
		//cout << ritr.key() << endl;

		for (auto& i : (*ritr).items()) {	// 各データのkey, valueのペアをループ
			string v;
			stringstream ss;
			float tmp;

			v = i.value().dump();
			ss << v;
			ss.ignore();
			ss >> tmp;
			daily.back().push_back(tmp);
			//cout << tmp << "\t";
		}
		ritr++;
		//cout << endl;
	}


	/*========== 5日ATRデータの読み込み ==========*/
	if (true) {
		j = call_api_atr5(symbol);
		if (j == nullptr) {
			return 0;
		}

		itr = j.begin();
		itr++;	// "Meta Data"を飛ばす
		ritr = (*itr).rbegin();

		/* データをvectorに格納 */
		for (int t = 0; t < size; t++) {	// 日付オブジェクトをループ
			//cout << ritr.key() << endl;
			string v;
			stringstream ss;
			float tmp;

			v = (*ritr).begin().value().dump();
			ss << v;
			ss.ignore();
			ss >> tmp;
			atr.push_back(tmp);
			//cout << tmp << endl;

			ritr++;
		}
	}

	generate_data(x_train, t_train, x_test, t_test);

	return 1;
}

int DataGenerator::generate_from_file(vector<vector<float>>& x_train, vector<int>& t_train
		, vector<vector<float>>& x_test, vector<int>& t_test)
{
	int size = 300;	// 読み込むデータ数(130 = 約半年分)

	/*========== 日足データの読み込み ==========*/
	json j = load_json("spy_daily_full.json");
	if (j == nullptr) {
		return 0;
	}

	json::iterator itr = j.begin();
	itr++;	// "Meta Data"を飛ばす

	json::reverse_iterator ritr = (*itr).rbegin(); // 日付最新順にループするための逆イテレータ

	/* 各データをvectorに格納 */
	for (int t = 0; t < size; t++) {	// 日付オブジェクトをループ
		daily.push_back(vector<float>());
		//cout << ritr.key() << endl;

		for (auto& i : (*ritr).items()) {	// 各データのkey, valueのペアをループ
			string v;
			stringstream ss;
			float tmp;

			v = i.value().dump();
			ss << v;
			ss.ignore();
			ss >> tmp;
			daily.back().push_back(tmp);
			//cout << tmp << "\t";
		}
		ritr++;
		//cout << endl;
	}


	/*========== 5日ATRデータの読み込み ==========*/
	if (true) {
		j = load_json("spy_daily_atr5.json");
		if (j == nullptr) {
			return 0;
		}

		itr = j.begin();
		itr++;	// "Meta Data"を飛ばす
		ritr = (*itr).rbegin();

		/* データをvectorに格納 */
		for (int t = 0; t < size; t++) {	// 日付オブジェクトをループ
			//cout << ritr.key() << endl;
			string v;
			stringstream ss;
			float tmp;

			v = (*ritr).begin().value().dump();
			ss << v;
			ss.ignore();
			ss >> tmp;
			atr.push_back(tmp);
			//cout << tmp << endl;

			ritr++;
		}
	}

	generate_data(x_train, t_train, x_test, t_test);

	return 1;
}

void DataGenerator::generate_data(vector<vector<float>>& x_train, vector<int>& t_train
	, vector<vector<float>>& x_test, vector<int>& t_test)
{
	/*========== 訓練データに値を格納(最も古いデータと最新5つのデータは使わない) ==========*/

	float theta = 0.02f;	// 閾値
	int offset = 5;	// 何個先のデータと比較するか
	int test_size = 50;
	int test_p = 0;	// 評価データに入っている陽性データの数
	int test_n = 0;	// 評価データに入っている陰性データの数

	for (int i = daily.size() - 6; i >= offset; i--) {	// 日付が古い順にデータを参照
		vector<vector<float>>* x = &x_train;
		vector<int>* t = &t_train;

		/* 出力値 */
		float p = (daily[i - offset][3] - daily[i][3]) / daily[i][3];	// 2データの終値の比が閾値を超えているかどうか
		if (p >= theta) {
			if (test_p < test_size / 2) {	// 評価データに入れるかどうか
				test_p++;
				x = &x_test;
				t = &t_test;
			}
			else {
				positive_data.push_back((*x).size());
			}
			(*t).push_back(1);
		}
		else {
			if (test_n < test_size / 2) {	// 評価データに入れるかどうか
				test_n++;
				x = &x_test;
				t = &t_test;
			}
			else {
				negative_data.push_back((*x).size());
			}
			(*t).push_back(0);
		}

		/* 入力値 */
		(*x).push_back(vector<float>());
		(*x).back().push_back(abs(daily[i][3] - daily[i][0]) / (daily[i][1] - daily[i][2]));	// |終値 - 始値| / (高値 - 安値)
		(*x).back().push_back((daily[i][3] - daily[i + 1][3]) * 100 / daily[i + 1][3]);	// 変動率
		(*x).back().push_back((daily[i + 1][3] - daily[i + 2][3]) * 100 / daily[i + 2][3]);	// 前日の変動率
		(*x).back().push_back((daily[i][3] - daily[i + 5][3]) / 10);	// 5日モメンタム
		(*x).back().push_back(atr[i]);
	}
}

void DataGenerator::generate_minibatch(const vector<vector<float>>& src_x, const vector<int>& src_t
		, vector<vector<float>>& dst_x, vector<int>& dst_t, const int batch_size)
{
	int batch = batch_size / 2;

	/* 陽性データの選択 */
	if (batch > positive_data.size()) {
		batch = positive_data.size();
		for (auto i : positive_data) {
			dst_x.push_back(src_x[i]);
			dst_t.push_back(1);
			//cout << r << ":\t" << dst_x.back().back() << "\t" << dst_t.back() << endl;
		}
	}
	else {
		for (int i = 0; i < batch; i++) {
			int r = get_randi(0, positive_data.size() - 1);

			dst_x.push_back(src_x[positive_data[r]]);
			dst_t.push_back(1);
			//cout << r << ":\t" << dst_x.back().back() << "\t" << dst_t.back() << endl;
		}
	}

	/* 陰性データの選択 */
	for (int i = 0; i < batch; i++) {
		int r = get_randi(0, negative_data.size() - 1);

		dst_x.push_back(src_x[negative_data[r]]);
		dst_t.push_back(0);
		//cout << r << ":\t" << dst_x.back().back() << "\t" << dst_t.back() << endl;
	}
}
