#include "DataGenerator.h"
#include "Math.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <curl/curl.h>

DataGenerator::DataGenerator(const string& key, const string& symb, const float& per, const int& d, const int& batch)
	: api_key(key)
	, symbol(symb)
	, percent(per)
	, day(d)
	, batch_size(batch)
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

json DataGenerator::call_api_daily()
{
	string buf;
	string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + "&outputsize=full&apikey=" + api_key;
	
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

json DataGenerator::call_api_atr()
{
	string buf;
	string url = "https://www.alphavantage.co/query?function=ATR&symbol=" + symbol + "&interval=daily&time_period=14&apikey=" + api_key;

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

// デバッグ用
json DataGenerator::load_json(const string& file_name)
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

int DataGenerator::generate_from_api(vector<vector<float>>& x_train, vector<int>& t_train
	, vector<vector<float>>& x_test, vector<int>& t_test)
{
	int size = 300;	// 読み込むデータ数(130 = 約半年分)

	/*========== 日足データの読み込み ==========*/
	json j = call_api_daily();
	if (j == nullptr) {
		return 0;
	}

	json::iterator itr = j.begin();
	itr++;	// "Meta Data"を飛ばす

	json::reverse_iterator ritr = (*itr).rbegin(); // 日付最新順にループするための逆イテレータ

	/* 各データをvectorに格納 */
	for (int t = 0; t < size; t++) {	// 日付オブジェクトをループ
		daily.push_back(vector<float>());

		for (auto& i : (*ritr).items()) {	// 各データのkey, valueのペアをループ
			string v;
			stringstream ss;
			float tmp;

			v = i.value().dump();
			ss << v;
			ss.ignore();
			ss >> tmp;
			daily.back().push_back(tmp);
		}
		ritr++;
	}


	/*========== 14日ATRデータの読み込み ==========*/
	j = call_api_atr();
	if (j == nullptr) {
		return 0;
	}

	itr = j.begin();
	itr++;	// "Meta Data"を飛ばす
	ritr = (*itr).rbegin();

	/* データをvectorに格納 */
	for (int t = 0; t < size; t++) {	// 日付オブジェクトをループ
		string v;
		stringstream ss;
		float tmp;

		v = (*ritr).begin().value().dump();
		ss << v;
		ss.ignore();
		ss >> tmp;
		atr.push_back(tmp);

		ritr++;
	}

	if (!generate_data(x_train, t_train, x_test, t_test))
		return 0;

	return 1;
}

// デバッグ用
int DataGenerator::generate_from_file(vector<vector<float>>& x_train, vector<int>& t_train
		, vector<vector<float>>& x_test, vector<int>& t_test)
{
	int size = 300;	// 読み込むデータ数(130 = 約半年分)

	/*========== 日足データの読み込み ==========*/
	string file_name = "Learning/" + symbol + "_daily_full.json";

	json j = load_json(file_name);
	if (j == nullptr) {
		return 0;
	}

	json::iterator itr = j.begin();
	itr++;	// "Meta Data"を飛ばす

	json::reverse_iterator ritr = (*itr).rbegin(); // 日付最新順にループするための逆イテレータ

	/* 各データをvectorに格納 */
	for (int t = 0; t < size; t++) {	// 日付オブジェクトをループ
		daily.push_back(vector<float>());

		for (auto& i : (*ritr).items()) {	// 各データのkey, valueのペアをループ
			string v;
			stringstream ss;
			float tmp;

			v = i.value().dump();
			ss << v;
			ss.ignore();
			ss >> tmp;
			daily.back().push_back(tmp);
		}
		ritr++;
	}


	/*========== 5日ATRデータの読み込み ==========*/
	file_name = "Learning/" + symbol + "_daily_atr5.json";

	j = load_json(file_name);
	if (j == nullptr) {
		return 0;
	}

	itr = j.begin();
	itr++;	// "Meta Data"を飛ばす
	ritr = (*itr).rbegin();

	/* データをvectorに格納 */
	for (int t = 0; t < size; t++) {	// 日付オブジェクトをループ
		string v;
		stringstream ss;
		float tmp;

		v = (*ritr).begin().value().dump();
		ss << v;
		ss.ignore();
		ss >> tmp;
		atr.push_back(tmp);

		ritr++;
	}

	if (!generate_data(x_train, t_train, x_test, t_test))
		return 0;

	return 1;
}

int DataGenerator::generate_data(vector<vector<float>>& x_train, vector<int>& t_train
	, vector<vector<float>>& x_test, vector<int>& t_test)
{
	x_train.clear();
	t_train.clear();
	x_test.clear();
	t_test.clear();
	latest.clear();

	/* 変動率の計算 */
	vector<float> vol(daily.size() - 1);
	for (int i = 0; i < daily.size() - 1; i++) {
		vol[i] = (daily[i][3] - daily[i + 1][3]) * 100 / daily[i + 1][3];
	}

	/* 10日モメンタムの計算 */
	int interval = 10;
	vector<float> mom(daily.size() - interval);
	for (int i = 0; i < daily.size() - interval; i++) {
		mom[i] = daily[i][3] - daily[i + interval][3];
	}

	/* 正規化 */
	Math::normalize(vol);
	Math::normalize(mom);
	Math::normalize(atr);


	/*========== 訓練データに値を格納(最も古いデータと最新offset個のデータは使わない) ==========*/

	float theta = percent / 100.f;	// 閾値
	int offset = day;	// 何日先のデータと比較するか
	int test_size = 70;
	int test_p = 0;	// 評価データに入っている陽性データの数
	int test_n = 0;	// 評価データに入っている陰性データの数

	for (int i = daily.size() - 2 - interval; i >= offset; i--) {	// 日付が古い順にデータを参照
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
		(*x).back().push_back(vol[i]);	// 変動率
		(*x).back().push_back(vol[i + 1]);	// 前日の変動率
		(*x).back().push_back(mom[i]);	// 10日モメンタム
		(*x).back().push_back(mom[i + 1]);	// 前日の10日モメンタム
		(*x).back().push_back(atr[i]);	// 14日ATR
	}

	if (positive_data.size() < batch_size / 2) {
		cout << "Unable to generate available training data.\nPlease try to change the parameters." << endl;
		return 0;
	}

	/* 推論用の最新データを生成 */
	latest.push_back(vol[0]);
	latest.push_back(vol[1]);
	latest.push_back(mom[0]);
	latest.push_back(mom[1]);
	latest.push_back(atr[0]);

	return 1;
}

void DataGenerator::generate_minibatch(const vector<vector<float>>& src_x, const vector<int>& src_t
		, vector<vector<float>>& dst_x, vector<int>& dst_t, const int batch_size)
{
	dst_x.clear();
	dst_t.clear();

	int batch = batch_size / 2;

	/* 陽性データの選択 */
	if (batch > positive_data.size()) {	// バッチサイズが大きい場合
		batch = positive_data.size();
		for (int i = 0; i < batch; i++) {
			dst_x.push_back(src_x[positive_data[i]]);
			dst_t.push_back(1);
		}
	}
	else {
		for (int i = 0; i < batch; i++) {
			int r = get_randi(0, positive_data.size() - 1);

			dst_x.push_back(src_x[positive_data[r]]);
			dst_t.push_back(1);
		}
	}

	/* 陰性データの選択 */
	for (int i = 0; i < batch; i++) {
		int r = get_randi(0, negative_data.size() - 1);

			dst_x.push_back(src_x[negative_data[r]]);
			dst_t.push_back(0);
	}
}
