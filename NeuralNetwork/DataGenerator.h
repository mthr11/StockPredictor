#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

class DataGenerator
{
public:
	// �R���X�g���N�^�ƃf�X�g���N�^
	DataGenerator();
	~DataGenerator();

	// �R�[���o�b�N�֐�
	static size_t write(char* data, size_t size, size_t nmemb, string* buffer);

	int load_from_api(const string symbol, vector<vector<float>>& x_train, vector<int>& t_train,
			vector<vector<float>>& x_test, vector<int>& t_test);
	int load_from_file(const string file_name, vector<vector<float>>& x_train, vector<int>& t_train,
			vector<vector<float>>& x_test, vector<int>& t_test);
	void generate(vector<vector<float>>& x_train, vector<int>& t_train, vector<vector<float>>& x_test, vector<int>& t_test);
private:
	json j;
};

