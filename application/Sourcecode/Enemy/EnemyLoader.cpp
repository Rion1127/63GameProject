#include "EnemyLoader.h"

#include "EnemyDummy.h"
#include "EnemyAirDummy.h"
#include "EnemyShadow.h"

EnemyLoader* EnemyLoader::GetInstance()
{
	static EnemyLoader instance;
	return &instance;
}

void EnemyLoader::LoadEnemyPopFile(const std::string& fileName)
{
	PopData popData;
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
		//POP�R�}���h
		if (word.find("POP") == 0)
		{
			//���W�ǂݍ���
			Vector3 pos = GetPosLoad(line_stream, word, ',');

			//�G�̎��
			std::getline(line_stream, word, ',');
			std::string enemyType = word.c_str();


			if (enemyType == "Dummy")
			{
				popData.enemys.emplace_back(std::move(std::make_unique<EnemyDummy>(pos)));
			}
			else if (enemyType == "Shadow")
			{
				popData.enemys.emplace_back(std::move(std::make_unique<EnemyShadow>(pos)));
			}
		}
		else if (word.find("WAIT") == 0)
		{
			std::getline(line_stream, word, ',');
			//�҂�����
			int32_t waitTime = atoi(word.c_str());

			//�R�}���h���[�v�𔲂���
			break;
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

	return Vector3(x,y,z);
}
