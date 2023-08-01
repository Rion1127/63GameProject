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
		//POPコマンド
		if (word.find("POP") == 0)
		{
			//座標読み込み
			Vector3 pos = GetPosLoad(line_stream, word, ',');

			//敵の種類
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
			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//コマンドループを抜ける
			break;
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

	return Vector3(x,y,z);
}
