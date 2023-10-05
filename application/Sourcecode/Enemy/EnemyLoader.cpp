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
	//ファイルを開く
	std::ifstream file;
	file.open(fileName);
	assert(file.is_open());
	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands_ << file.rdbuf();
	//ファイルを閉じる
	file.close();

	//1行分の文字列を入れる変数
	std::string line;
	//コマンド実行ループ
	while (std::getline(enemyPopCommands_, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//、区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');
		// "//"から始まる行はコメント
		if (word.find("//") == 0)
		{
			//コメント行を飛ばす
			continue;
		}
		if (word.find("ROUND") == 0)
		{
			std::string roundNum = word;
			//ラウンドの数値だけを残す
			roundNum.erase(0, 5);
			//１ラウンド分のデータを生成
			popDatas.emplace_back();
			popDatas.back().roundNum = (uint32_t)std::atof(roundNum.c_str());
		}

		//SETコマンド
		if (word.find("SET") == 0)
		{
			EnemyData enemydata;
			//座標読み込み
			Vector3 pos = GetPosLoad(line_stream, word, ',');
			enemydata.pos = pos;

			Vector3 rot = GetRotLoad(line_stream, word, ',');
			enemydata.rot = rot;

			//敵の種類
			std::getline(line_stream, word, ',');
			std::string enemyType = word.c_str();

			enemydata.name = enemyType;

			//敵を生成するのに必要なデータを挿入
			popDatas.back().enemyData.emplace_back(enemydata);
		}
	}
	//一つのファイルとdataNameをペアにして保存
	popDatas_.insert(std::make_pair(dataName, std::move(popDatas)));
}

void EnemyLoader::SetEnemy(std::list<std::unique_ptr<IEnemy>>* enemys, const std::string& dataName, int32_t roundNum)
{
	if (dataName == "") return;
	int32_t min = 0;
	int32_t max = (int32_t)popDatas_[dataName].size();
	int32_t roundNum_ = Clamp(roundNum - 1, min, max);
	//見やすいように参照渡しする
	std::vector<EnemyData>& enemyData = popDatas_[dataName][roundNum_].enemyData;
	enemys->clear();
	//敵をセットしていく
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
	//x座標
	std::getline(line_stream, word, ',');
	float x = (float)std::atof(word.c_str());
	//y座標
	std::getline(line_stream, word, ',');
	float y = (float)std::atof(word.c_str());
	//Z座標
	std::getline(line_stream, word, ',');
	float z = (float)std::atof(word.c_str());

	return Vector3(x, y, z);
}

Vector3 EnemyLoader::GetRotLoad(std::istringstream& line_stream, std::string& word, char _Delim)
{
	//x座標
	std::getline(line_stream, word, ',');
	float x = Radian((float)std::atof(word.c_str()));
	//y座標
	std::getline(line_stream, word, ',');
	float y = Radian((float)std::atof(word.c_str()));
	//Z座標
	std::getline(line_stream, word, ',');
	float z = Radian((float)std::atof(word.c_str()));

	return Vector3(x, y, z);
}
