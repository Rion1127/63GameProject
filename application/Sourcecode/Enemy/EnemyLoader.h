#pragma once
#include "IEnemy.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <fstream>

/**
 * @file EnemyLoader.h
 * @brief csvファイルから敵の情報を読み込むクラス
 */

class EnemyLoader
{
private:
	struct EnemyData {
		std::string name;
		Vector3 pos;
		Vector3 rot;
	};
	struct PopData {
		uint32_t roundNum;
		std::vector<EnemyData> enemyData;
	};
private:
	std::unordered_map<std::string, std::vector<PopData>> popDatas_;
public:
	static EnemyLoader* GetInstance();
	void LoadEnemyPopFile(const std::string& fileName, const std::string& dataName);
	void SetEnemy(std::list<std::unique_ptr<IEnemy>>* enemys, const std::string& dataName, int32_t roundNum);
public:
	std::vector<PopData> GetEnemyData(std::string dataName) { return popDatas_[dataName]; }
	std::unordered_map<std::string, std::vector<PopData>> GetAllData() { return popDatas_; }
private:
	EnemyLoader() {};
	Vector3 GetPosLoad(std::istringstream& line_stream, std::string& word);
	Vector3 GetRotLoad(std::istringstream& line_stream, std::string& word);
};

