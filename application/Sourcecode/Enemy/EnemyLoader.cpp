#include "EnemyLoader.h"

#include "EnemyDummy.h"
#include "EnemyAirDummy.h"
#include "EnemyShadow.h"

EnemyLoader* EnemyLoader::GetInstance()
{
	static EnemyLoader instance;
	return &instance;
}

void EnemyLoader::LoadEnemyPopFile(const std::string& fileName, const std::string& dataName)
{

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
			//���W�ǂݍ���
			Vector3 pos = GetPosLoad(line_stream, word, ',');

			//�G�̎��
			std::getline(line_stream, word, ',');
			std::string enemyType = word.c_str();

			if (enemyType == "Dummy")
			{
				popDatas.back().enemys.emplace_back(std::move(std::make_unique<EnemyDummy>(pos)));
			}
			else if (enemyType == "Shadow")
			{
				popDatas.back().enemys.emplace_back(std::move(std::make_unique<EnemyShadow>(pos)));
			}
		}
	}

	popDatas_.insert(std::make_pair(dataName, std::move(popDatas)));
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
