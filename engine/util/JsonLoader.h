#pragma once
#include <string>
#include "Object3D.h"
#include "json.hpp"
#include "Camera.h"

/**
 * @file JsonLoader.h
 * @brief nlohmann::jsonを用いた.jsonファイルの読み込み
 * @brief 地形オブジェの読み込みを行っている
 */

struct CameraInfo {
	Vector3 pos;
	Vector3 rot;
};

struct LevelData {
	std::vector<std::unique_ptr<Object3d>> object;
	CameraInfo cameraInfo;
	std::string fileName;
};
class JsonLoader
{
private:
	//ディレクトリ名
	std::string defaultBaseDirectory;
	//読み込んだオブジェデータ
	std::map<std::string, std::unique_ptr<LevelData>> levelData_;
public:
	static JsonLoader* GetInstance();
	JsonLoader();

	//JSONファイル読み込み dataNameで名前をつける
	void LoadFile(const std::string& fileName, const std::string& dataName);
	void SetObjects(std::unordered_map<std::string, std::unique_ptr<Object3d>>* objects, const std::string& levelDataName);
	void SetCamera(Camera* camera, const std::string& levelDataName);
};

