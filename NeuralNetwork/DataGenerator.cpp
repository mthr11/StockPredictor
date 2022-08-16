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
	
	/* curl�̃Z�b�g�A�b�v */
	CURL* curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

	/* ���s */
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

	/* curl�̃Z�b�g�A�b�v */
	CURL* curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

	/* ���s */
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
	int size = 300;	// �ǂݍ��ރf�[�^��(130 = �񔼔N��)

	/*========== �����f�[�^�̓ǂݍ��� ==========*/
	json j = call_api_daily(symbol);
	if (j == nullptr) {
		return 0;
	}

	json::iterator itr = j.begin();
	itr++;	// "Meta Data"���΂�

	json::reverse_iterator ritr = (*itr).rbegin(); // ���t�ŐV���Ƀ��[�v���邽�߂̋t�C�e���[�^

	/* �e�f�[�^��vector�Ɋi�[ */
	for (int t = 0; t < size; t++) {	// ���t�I�u�W�F�N�g�����[�v
		daily.push_back(vector<float>());
		//cout << ritr.key() << endl;

		for (auto& i : (*ritr).items()) {	// �e�f�[�^��key, value�̃y�A�����[�v
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


	/*========== 5��ATR�f�[�^�̓ǂݍ��� ==========*/
	if (true) {
		j = call_api_atr5(symbol);
		if (j == nullptr) {
			return 0;
		}

		itr = j.begin();
		itr++;	// "Meta Data"���΂�
		ritr = (*itr).rbegin();

		/* �f�[�^��vector�Ɋi�[ */
		for (int t = 0; t < size; t++) {	// ���t�I�u�W�F�N�g�����[�v
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
	int size = 300;	// �ǂݍ��ރf�[�^��(130 = �񔼔N��)

	/*========== �����f�[�^�̓ǂݍ��� ==========*/
	json j = load_json("spy_daily_full.json");
	if (j == nullptr) {
		return 0;
	}

	json::iterator itr = j.begin();
	itr++;	// "Meta Data"���΂�

	json::reverse_iterator ritr = (*itr).rbegin(); // ���t�ŐV���Ƀ��[�v���邽�߂̋t�C�e���[�^

	/* �e�f�[�^��vector�Ɋi�[ */
	for (int t = 0; t < size; t++) {	// ���t�I�u�W�F�N�g�����[�v
		daily.push_back(vector<float>());
		//cout << ritr.key() << endl;

		for (auto& i : (*ritr).items()) {	// �e�f�[�^��key, value�̃y�A�����[�v
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


	/*========== 5��ATR�f�[�^�̓ǂݍ��� ==========*/
	if (true) {
		j = load_json("spy_daily_atr5.json");
		if (j == nullptr) {
			return 0;
		}

		itr = j.begin();
		itr++;	// "Meta Data"���΂�
		ritr = (*itr).rbegin();

		/* �f�[�^��vector�Ɋi�[ */
		for (int t = 0; t < size; t++) {	// ���t�I�u�W�F�N�g�����[�v
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
	/*========== �P���f�[�^�ɒl���i�[(�ł��Â��f�[�^�ƍŐV5�̃f�[�^�͎g��Ȃ�) ==========*/

	float theta = 0.02f;	// 臒l
	int offset = 5;	// ����̃f�[�^�Ɣ�r���邩
	int test_size = 50;
	int test_p = 0;	// �]���f�[�^�ɓ����Ă���z���f�[�^�̐�
	int test_n = 0;	// �]���f�[�^�ɓ����Ă���A���f�[�^�̐�

	for (int i = daily.size() - 6; i >= offset; i--) {	// ���t���Â����Ƀf�[�^���Q��
		vector<vector<float>>* x = &x_train;
		vector<int>* t = &t_train;

		/* �o�͒l */
		float p = (daily[i - offset][3] - daily[i][3]) / daily[i][3];	// 2�f�[�^�̏I�l�̔䂪臒l�𒴂��Ă��邩�ǂ���
		if (p >= theta) {
			if (test_p < test_size / 2) {	// �]���f�[�^�ɓ���邩�ǂ���
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
			if (test_n < test_size / 2) {	// �]���f�[�^�ɓ���邩�ǂ���
				test_n++;
				x = &x_test;
				t = &t_test;
			}
			else {
				negative_data.push_back((*x).size());
			}
			(*t).push_back(0);
		}

		/* ���͒l */
		(*x).push_back(vector<float>());
		(*x).back().push_back(abs(daily[i][3] - daily[i][0]) / (daily[i][1] - daily[i][2]));	// |�I�l - �n�l| / (���l - ���l)
		(*x).back().push_back((daily[i][3] - daily[i + 1][3]) * 100 / daily[i + 1][3]);	// �ϓ���
		(*x).back().push_back((daily[i + 1][3] - daily[i + 2][3]) * 100 / daily[i + 2][3]);	// �O���̕ϓ���
		(*x).back().push_back((daily[i][3] - daily[i + 5][3]) / 10);	// 5���������^��
		(*x).back().push_back(atr[i]);
	}
}

void DataGenerator::generate_minibatch(const vector<vector<float>>& src_x, const vector<int>& src_t
		, vector<vector<float>>& dst_x, vector<int>& dst_t, const int batch_size)
{
	int batch = batch_size / 2;

	/* �z���f�[�^�̑I�� */
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

	/* �A���f�[�^�̑I�� */
	for (int i = 0; i < batch; i++) {
		int r = get_randi(0, negative_data.size() - 1);

		dst_x.push_back(src_x[negative_data[r]]);
		dst_t.push_back(0);
		//cout << r << ":\t" << dst_x.back().back() << "\t" << dst_t.back() << endl;
	}
}
