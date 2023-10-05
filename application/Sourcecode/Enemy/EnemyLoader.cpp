#include "EnemyLoader.h"

#include "EnemyDummy.h"
#include "EnemyAirDummy.h"
#include "EnemyShadow.h"
#include "EnemyRedNocturne.h"

EnemyLoader* EnemyLoader::GetInstance()
{
	static EnemyLoader instance;
	return &instance;
}

void EnemyLoader::LoadEnemyPopFile(const std::string& fileName, const std::string& dataName)
{
	if (popDatas_.find(dataName) != popDatas_.end())
	{
		popDatas_.erase("");
		popDatas_.erase(dataName);
	}


	std::vector<PopData> popDatas;
	std::stringstream enemyPopCommands_;
	//�t�@�C�����J��
	std::ifstream file;
	file.open(fileName);
	assert(file.is_open());
	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	enemyPopCommands_ << file.rdbuf();
	//�t�@�C�������
	file.close();

	//1�s���̕����������ϐ�
	std::string line;
	//�R�}���h���s���[�v
	while (std::getline(enemyPopCommands_, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		std::string word;
		//�A��؂�ōs�̐擪��������擾
		std::getline(line_stream, word, ',');
		// "//"����n�܂�s�̓R�����g
		if (word.find("//") == 0)
		{
			//�R�����g�s���΂�
			continue;
		}
		if (word.find("ROUND") == 0)
		{
			std::string roundNum = word;
			//���E���h�̐��l�������c��
			roundNum.erase(0, 5);
			//�P���E���h���̃f�[�^�𐶐�
			popDatas.emplace_back();
			popDatas.back().roundNum = (uint32_t)std::atof(roundNum.c_str());
		}

		//SET�R�}���h
		if (word.find("SET") == 0)
		{
			EnemyData enemydata;
			//���W�ǂݍ���
			Vector3 pos = GetPosLoad(line_stream, word, ',');
			enemydata.pos = pos;

			Vector3 rot = GetRotLoad(line_stream, word, ',');
			enemydata.rot = rot;

			//�G�̎��
			std::getline(line_stream, word, ',');
			std::string enemyType = word.c_str();

			enemydata.name = enemyType;

			//�G�𐶐�����̂ɕK�v�ȃf�[�^��}��
			popDatas.back().enemyData.emplace_back(enemydata);
		}
	}
	//��̃t�@�C����dataName���y�A�ɂ��ĕۑ�
	popDatas_.insert(std::make_pair(dataName, std::move(popDatas)));
}

void EnemyLoader::SetEnemy(std::list<std::unique_ptr<IEnemy>>* enemys, const std::string& dataName, int32_t roundNum)
{
	if (dataName == "") return;
	int32_t min = 0;
	int32_t max = (int32_t)popDatas_[dataName].size();
	int32_t roundNum_ = Clamp(roundNum - 1, min, max);
	//���₷���悤�ɎQ�Ɠn������
	std::vector<EnemyData>& enemyData = popDatas_[dataName][roundNum_].enemyData;
	enemys->clear();
	//�G���Z�b�g���Ă���
	for (uint32_t i = 0; i < enemyData.size(); i++)
	{
		std::unique_ptr<IEnemy> newEnemy;
		if (enemyData[i].name == "Dummy")
		{
			newEnemy = std::make_unique<EnemyDummy>(enemyData[i].pos);
			enemys->emplace_back(std::move(newEnemy));
		}
		else if (enemyData[i].name == "Shadow")
		{
			newEnemy = std::make_unique<EnemyShadow>(enemyData[i].pos, enemyData[i].rot);
			enemys->emplace_back(std::move(newEnemy));
		}
		else if (enemyData[i].name == "RedNocturne")
		{
			newEnemy = std::make_unique<EnemyRedNocturne>(enemyData[i].pos, enemyData[i].rot);
			enemys->emplace_back(std::move(newEnemy));
		}
	}
}

Vector3 EnemyLoader::GetPosLoad(std::istringstream& line_stream, std::string& word, char _Delim)
{
	//x���W
	std::getline(line_stream, word, ',');
	float x = (float)std::atof(word.c_str());
	//y���W
	std::getline(line_stream, word, ',');
	float y = (float)std::atof(word.c_str());
	//Z���W
	std::getline(line_stream, word, ',');
	float z = (float)std::atof(word.c_str());

	return Vector3(x, y, z);
}

Vector3 EnemyLoader::GetRotLoad(std::istringstream& line_stream, std::string& word, char _Delim)
{
	//x���W
	std::getline(line_stream, word, ',');
	float x = Radian((float)std::atof(word.c_str()));
	//y���W
	std::getline(line_stream, word, ',');
	float y = Radian((float)std::atof(word.c_str()));
	//Z���W
	std::getline(line_stream, word, ',');
	float z = Radian((float)std::atof(word.c_str()));

	return Vector3(x, y, z);
}
