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
	kDefaultBaseDirectory = "application/Resources/JSON/";
}

void JsonLoader::LoadFile(std::string fileName, std::string dataName)
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
	std::unique_ptr<LevelData> levelData = std::move(std::make_unique<LevelData>());
	levelData->fileName = fileName;
	//"object"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();


		if (type.compare("MESH") == 0)
		{
			std::unique_ptr<Object3d> newobj = std::move(std::make_unique<Object3d>());
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
				Radian(-(float)transform["rotation"][1]),
				Radian(-(float)transform["rotation"][2]),
				Radian((float)transform["rotation"][0]),
			};
			newobj->SetRot(rot);
			//スケーリング
			Vector3 scale = {
				(float)transform["scaling"][1],
				(float)transform["scaling"][2],
				(float)transform["scaling"][0],
			};
			newobj->SetScale(scale);
			//モデルを読み込む
			std::string modelName_ = object["model_name"].get<std::string>();
			newobj->SetModel(Model::CreateOBJ_uniptr(modelName_, true));
			//表示フラグを代入
			nlohmann::json& visible = object["isvisible"];
			bool isVisible = visible.get<uint32_t>();

			newobj->SetIsVisible(isVisible);
			levelData->object.push_back(std::move(newobj));
		}
		else if (type.compare("CAMERA") == 0)
		{
			nlohmann::json& transform = object["transform"];
			Vector3 pos = {
				(float)transform["translation"][1],
				(float)transform["translation"][2],
				-(float)transform["translation"][0],
			};
			//回転角
			Vector3 rot = {
				Radian(-(float)transform["rotation"][0] + 90.f),
				Radian(-(float)transform["rotation"][2] + 90.f),
				Radian((float)transform["rotation"][1]),
			};
			levelData->cameraInfo.pos = pos;
			levelData->cameraInfo.rot = rot;
		}

	}

	if (levelData_.size() > 0) {
		//同じファイルを読み込んだ場合、新しい情報に入れ替える
		if (levelData_.find(dataName)->second->fileName == fileName) {
			levelData_.erase(dataName);
		}
	}

	levelData_.insert(std::make_pair(dataName, std::move(levelData)));
}

void JsonLoader::SetObjects(std::vector<std::unique_ptr<Object3d>>* objects, std::string levelDataName) {

	LevelData* data = levelData_.at(levelDataName).get();
	size_t num = data->object.size();

	objects->clear();
	for (size_t i = 0; i < num; i++)
	{
		//読み込んだ情報を代入してく
		std::unique_ptr<Object3d> newObj = std::move(std::make_unique<Object3d>());
		newObj->SetPos(data->object.at(i)->GetPos());
		newObj->SetRot(data->object.at(i)->GetRot());
		newObj->SetScale(data->object.at(i)->GetScale());
		std::string modelName_ = data->object.at(i)->GetModel()->GetModelName();
		newObj->SetModel(Model::CreateOBJ_uniptr(modelName_, false));
		newObj->SetIsVisible(data->object.at(i)->GetIsVisible());
		objects->push_back(std::move(newObj));
	}
}

void JsonLoader::SetCamera(Camera* camera, std::string levelDataName)
{
	LevelData* data = levelData_.at(levelDataName).get();
	camera->eye_ = data->cameraInfo.pos;
	camera->rot_ = data->cameraInfo.rot;
}
