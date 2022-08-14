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

//int DataGenerator::load_from_api(const string symbol, vector<vector<float>>& x_train, vector<int>& t_train,
//	vector<vector<float>>& x_test, vector<int>& t_test)
//{
//	string buf;
//	string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + "&apikey=demo";
//	
//	/* curlのセットアップ */
//	CURL* curl = curl_easy_init();
//
//	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
//
//	/* 実行 */
//	CURLcode res = curl_easy_perform(curl);
//	if (res) {
//		return 0;
//	}
//
//	curl_easy_cleanup(curl);
//
//	if (buf.size() != 0) {
//		json j = json::parse(buf);
//		cout << j;
//	}
//	else {
//		cout << buf << endl;
//		return 0;
//	}
//
//	return 1;
//}

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

int DataGenerator::generate_from_file(vector<vector<float>>& x_train, vector<int>& t_train
		, vector<vector<float>>& x_test, vector<int>& t_test)
{
	int size = 260;	// 読み込むデータ数(130 = 約半年分)
	vector<vector<float>> daily;
	vector<float> sma;

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

	//for (auto& p : buf) {
	//	for (auto& q : p)
	//		cout << q << "\t";
	//	cout << endl;
	//}


	/*========== 短期(5日)SMAデータの読み込み ==========*/
	j = load_json("spy_daily_sma5.json");
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
		sma.push_back(tmp);
		//cout << tmp << endl;

		ritr++;
	}

	
	/*========== 訓練データに値を格納(最新5つのデータと最も古いデータは使わない) ==========*/

	float theta = 0.03f;	// 閾値
	int offset = 5;
	
	for (int i = offset; i < daily.size() - 1; i++) {
		/* 入力値 */
		x_train.push_back(vector<float>());
		//x_train.back().push_back(buf[i][1] * 1e-2);	// 終値
		//x_train.back().push_back(daily[i][4] * 1e-8);	// 出来高
		//x_train.back().push_back(buf[i][1] - buf[i][0]);	// 終値 - 始値
		x_train.back().push_back(abs(daily[i][3] - daily[i][0]) / (daily[i][1] - daily[i][2]));	// |終値 - 始値| / (高値 - 安値)
		x_train.back().push_back((daily[i][3] - daily[i + 1][3]) * 100 / daily[i + 1][3]);	// 前日比
		x_train.back().push_back((daily[i][3] + daily[i][0]) / 2 - sma[i]);	// 終値, 始値の平均とSMAの差

		/* 出力値(データiとデータi+5の終値の比が閾値を超えているかどうか) */
		float p = (daily[i - offset][3] - daily[i][3]) / daily[i][3];
		if (p >= theta) {
			t_train.push_back(1);
			positive_data.push_back(i - offset);
		}
		//else if (p <= -theta) {
		//	t_train.push_back(2);
		//}
		else {
			t_train.push_back(0);
			negative_data.push_back(i - offset);
		}
	}

	cout << positive_data.size() << endl;

	return 1;
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
