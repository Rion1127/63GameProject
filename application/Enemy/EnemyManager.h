#pragma once
#include "IEnemy.h"
#include <vector>
class EnemyManager
{
private:
	std::vector<std::unique_ptr<IEnemy>> enemys_;
public:
	void Update();
	void Draw();
};

