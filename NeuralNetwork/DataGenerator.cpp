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
	itr++;

	for (auto& p : *itr) {
		x_train.push_back(vector<float>());
		int r = get_randi(0, 1);
		t_train.push_back(r);

		for (auto& q : p.items()) {
			string s = q.value().dump();
			float f;
			stringstream ss;
			ss << s;
			ss.ignore();
			ss >> f;
			x_train.back().push_back(f);
			//cout << f << endl;
		}
	}
}
