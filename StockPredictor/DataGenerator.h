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
	DataGenerator(const string& key, const string& symb, const float& per, const int& d, const int& batch);
	~DataGenerator();

	// �R�[���o�b�N�֐�
	static size_t write(char* data, size_t size, size_t nmemb, string* buffer);

	// �Q�b�^�[
	vector<vector<float>> getdaily() { return daily; }

	// API�Ăяo��
	json call_api_daily();
	json call_api_atr();
	// �t�@�C���ǂݍ���(�f�o�b�O�p)
	json load_json(const string& file_name);

	// �e��f�[�^�𐶐�����
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
	int batch_size;

	vector<vector<float>> daily;	// �����f�[�^(�ŐV��)��ێ�����o�b�t�@
	vector<float> atr;	// ATR�f�[�^(�ŐV��)��ێ�����o�b�t�@
	vector<int> positive_data, negative_data;	// ���ꂼ��z���A�A���̃f�[�^�ԍ���ێ�����
};
