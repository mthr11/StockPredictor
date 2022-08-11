#include "DataGenerator.h"
#include "Math.h"
#include <iostream>
#include <fstream>
#include <sstream>
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

int DataGenerator::load_from_api(const string symbol, vector<vector<float>>& x_train, vector<int>& t_train,
	vector<vector<float>>& x_test, vector<int>& t_test)
{
	string buf;
	string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + "&apikey=demo";
	
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

	if (j.size() != 0) {
		j = json::parse(buf);
		cout << j;
	}
	else {
		cout << buf << endl;
		return 0;
	}

	return 1;
}

int DataGenerator::load_from_file(const string file_name, vector<vector<float>>& x_train, vector<int>& t_train,
	vector<vector<float>>& x_test, vector<int>& t_test)
{
	ifstream ifs(file_name);
	if (!ifs)
	{
		cout << "Failed to file open." << endl;
		return 0;
	}
	
	ifs >> j;
	//cout << j;
	//cout << j["Time Series (Daily)"].size();
	//cout << j["Technical Analysis: SMA"].size();

	generate(x_train, t_train, x_test, t_test);

	return 1;
}

void DataGenerator::generate(vector<vector<float>>& x_train, vector<int>& t_train, vector<vector<float>>& x_test, vector<int>& t_test)
{
	json::iterator itr = j.begin();
	itr++;	// "Meta Data"を飛ばす

	json::reverse_iterator ritr = (*itr).rbegin(); // 日付最新順にループするための逆イテレータ

	int size = 130;	// 読み込むデータ数(130 = 約半年分)
	vector<vector<float>> buf;

	/* 始値、終値、出来高をbufに格納 */
	for (int t = 0; t < size; t++) {	// 日付オブジェクトをループ
		buf.push_back(vector<float>());
		//cout << ritr.key() << endl;

		for (auto& i : (*ritr).items()) {	// 指標値のkey, valueのペアをループ
			string k = i.key();
			string v;
			stringstream ss;
			float tmp;

			if (k == "1. open" || k == "4. close" || k == "5. volume") {
				v = i.value().dump();
				ss << v;
				ss.ignore();
				ss >> tmp;
				buf.back().push_back(tmp);
				//cout << tmp << endl;
			}
		}
		//if (buf.size() >= 50) break;
		ritr++;
	}

	///* 始値、終値、出来高をbufに格納 */
	//for (auto& date : *itr) {	// 日付オブジェクトをループ
	//	buf.push_back(vector<float>());

	//	for (auto& i : date.items()) {	// 指標値のkey, valueのペアをループ
	//		string k = i.key();
	//		string v;
	//		stringstream ss;
	//		float tmp;

	//		if (k == "1. open" || k == "4. close" || k == "5. volume") {
	//			v = i.value().dump();
	//			ss << v;
	//			ss.ignore();
	//			ss >> tmp;
	//			buf.back().push_back(tmp);
	//			//cout << tmp << endl;
	//		}
	//	}
	//	if (buf.size() >= 50) break;
	//}

	//for (auto& p : buf) {
	//	for (auto& q : p)
	//		cout << q << "\t";
	//	cout << endl;
	//}
	
	float theta = 0.03f;	// 閾値

	/* 訓練データに値を格納(最も古いデータと最新5つのデータは使わない) */
	for (int i = 5; i < buf.size() - 1; i++) {
		/* 入力値 */
		x_train.push_back(vector<float>());
		//x_train.back().push_back(buf[i][1] * 1e-2);	// 終値
		//x_train.back().push_back(buf[i][2] * 1e-8);	// 出来高
		x_train.back().push_back(buf[i][1] - buf[i][0]);	// 終値 - 始値
		x_train.back().push_back((buf[i][1] - buf[i + 1][1]) * 1000 / buf[i + 1][1]);	// 前日比

		/* 出力値(データiとデータi+5の終値の比が閾値を超えているかどうか) */
		float p = (buf[i - 5][1] - buf[i][1]) / buf[i][1];
		if (p >= theta) {
			t_train.push_back(1);
		}
		//else if (p <= -theta) {
		//	t_train.push_back(2);
		//}
		else {
			t_train.push_back(0);
		}
	}
}
