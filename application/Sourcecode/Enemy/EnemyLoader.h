#pragma once
#include "IEnemy.h"
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
class EnemyLoader
{
private:
	struct PopData {
		uint32_t roundNum;
		std::list<std::unique_ptr<IEnemy>> enemys;
	};
private:
	std::vector<PopData> popDatas_;
public:
	static EnemyLoader* GetInstance();
	void LoadEnemyPopFile(const std::string& fileName);
private:
	EnemyLoader();
	Vector3 GetPosLoad(std::istringstream& line_stream, std::string& word,char _Delim);
};

