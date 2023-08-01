#pragma once
#include "IEnemy.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <fstream>
class EnemyLoader
{
private:
	struct PopData {
		uint32_t roundNum;
		std::vector<std::unique_ptr<IEnemy>> enemys;
	};
private:
	std::unordered_map<std::string,std::vector<PopData>> popDatas_;
public:
	static EnemyLoader* GetInstance();
	void LoadEnemyPopFile(const std::string& fileName, const std::string& dataName);
private:
	EnemyLoader() {};
	Vector3 GetPosLoad(std::istringstream& line_stream, std::string& word,char _Delim);
};

