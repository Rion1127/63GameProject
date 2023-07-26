#include "JsonLoader.h"
#include <fstream>
#include <cassert>

JsonLoader* JsonLoader::GetInstance()
{
	static JsonLoader instance;
	return &instance;
}

JsonLoader::JsonLoader()
{
	kDefaultBaseDirectory = "Resources/JSON/";
}

void JsonLoader::LoadFile(std::string fileName)
{
	//連結してフルパスを得る
	const std::string fullpath = kDefaultBaseDirectory + fileName;

	//ファイルストリーム
	std::ifstream file;
	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	//ただ仕入れ別データファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();
	
	//"object"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		if (type.compare("MESH") == 0)
		{
			levelData->object.emplace_back(new Object3D);
			Object3D* newobj = levelData->object.back();

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			Vector3 pos = {
				(float)transform["translation"][1],
				(float)transform["translation"][2],
				-(float)transform["translation"][0],
			};
			newobj->SetPos(pos);
			//回転角
			Vector3 rot = {
				-(float)transform["rotation"][1],
				-(float)transform["rotation"][2],
				(float)transform["rotation"][0],
			};
			newobj->SetRot(rot);
			//スケーリング
			Vector3 scale = {
				(float)transform["scaling"][1],
				(float)transform["scaling"][2],
				(float)transform["scaling"][0],
			};
			newobj->SetScale(scale);

			if (object.contains("children"))
			{
				levelData->object.emplace_back(new Object3D);
				Object3D& newobj = *levelData->object.back();

				Object3D* parent = levelData->object.at(levelData->object.size() - 2);

				newobj.SetParent(parent->GetWorldTransform());

			}
		}
	}

	levelData_.push_back(*levelData);
	delete levelData;
}