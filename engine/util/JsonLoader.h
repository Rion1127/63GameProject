#pragma once
#include <string>
#include "Object3D.h"
#include "json.hpp"

struct LevelData {
	std::vector<Object3D*> object;
	std::vector<std::string> fileName;
};
class JsonLoader
{
private:
	std::string kDefaultBaseDirectory;

	std::vector<LevelData> levelData_;
public:
	static JsonLoader* GetInstance();
	JsonLoader();


	void LoadFile(std::string fileName);
	void SetObjects(std::vector<Object3D*>* objects, int dataNum) {

		LevelData& data = levelData_.at(dataNum);
		int num = data.object.size();

		for (int i = 0; i < num; i++)
		{
			objects->emplace_back(data.object.at(i));
		}
	}
private:
	
private:
	//ÉQÉbÉ^Å[
	std::vector<LevelData> GetLevelData() { return levelData_; }

};

