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
	DataGenerator();
	~DataGenerator();

	// コールバック関数
	static size_t write(char* data, size_t size, size_t nmemb, string* buffer);

	int load_from_api(const string symbol, vector<vector<float>>& x_train, vector<int>& t_train,
			vector<vector<float>>& x_test, vector<int>& t_test);
	json load_json(const string file_name);
	int generate_from_file(vector<vector<float>>& x_train, vector<int>& t_train
			, vector<vector<float>>& x_test, vector<int>& t_test);
	void generate_minibatch(const vector<vector<float>>& src_x, const vector<int>& src_t
			, vector<vector<float>>& dst_x, vector<int>& dst_t, const int batch_size);
private:
	int positive_num;	// 陽性(正解ラベルが1)の数

	vector<int> positive_data, negative_data;	// それぞれ陽性、陰性のデータ番号を保持する
};

