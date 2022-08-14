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
	json load_json(const string file_name);
	int generate_from_file(vector<vector<float>>& x_train, vector<int>& t_train
			, vector<vector<float>>& x_test, vector<int>& t_test);
	void generate_minibatch(const vector<vector<float>>& src_x, const vector<int>& src_t
			, vector<vector<float>>& dst_x, vector<int>& dst_t, const int batch_size);
private:
	int positive_num;	// �z��(�������x����1)�̐�

	vector<int> positive_data, negative_data;	// ���ꂼ��z���A�A���̃f�[�^�ԍ���ێ�����
};

