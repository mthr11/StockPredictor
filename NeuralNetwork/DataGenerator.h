#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

class DataGenerator
{
public:
	// コンストラクタとデストラクタ
	DataGenerator(const string& key, const string& symb, const float& per, const int& d);
	~DataGenerator();

	// コールバック関数
	static size_t write(char* data, size_t size, size_t nmemb, string* buffer);

	// ゲッター
	vector<vector<float>> getdaily() { return daily; }

	// API呼び出し
	json call_api_daily();
	json call_api_atr();
	// ファイル読み込み
	json load_json(const string& file_name);

	// 各種データを生成する
	int generate_from_api(vector<vector<float>>& x_train, vector<int>& t_train
			, vector<vector<float>>& x_test, vector<int>& t_test);
	int generate_from_file(vector<vector<float>>& x_train, vector<int>& t_train
			, vector<vector<float>>& x_test, vector<int>& t_test);
	int generate_data(vector<vector<float>>& x_train, vector<int>& t_train
			, vector<vector<float>>& x_test, vector<int>& t_test);
	void generate_minibatch(const vector<vector<float>>& src_x, const vector<int>& src_t
			, vector<vector<float>>& dst_x, vector<int>& dst_t, const int batch_size);
private:
	string api_key;
	string symbol;
	float percent;
	int day;

	vector<vector<float>> daily;	// 日足データを保持するバッファ
	vector<float> atr;	// ATRデータを保持するバッファ
	vector<int> positive_data, negative_data;	// それぞれ陽性、陰性のデータ番号を保持する
};

