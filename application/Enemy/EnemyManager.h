#pragma once
#include "IEnemy.h"
#include <vector>

#include "EnemyDummy.h"
class EnemyManager
{
private:
	std::vector<std::unique_ptr<IEnemy>> enemys_;
public:
	EnemyManager();
	void Update();
	void Draw();
};

