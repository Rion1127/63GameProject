#pragma once
#include "IEnemy.h"
#include <vector>

#include "EnemyDummy.h"
class EnemyManager
{
private:
	std::vector<std::unique_ptr<IEnemy>> enemys_;
	std::vector<std::unique_ptr<IEnemy>> AirEnemys_;

	std::unique_ptr<Object3d> lockOnObj_;
	IEnemy* lockOnEnemy_;
public:
	EnemyManager();
	void PreUpdate();
	void PostUpdate();
	void Draw();
public:
	std::vector<std::unique_ptr<IEnemy>>* GetEnemy() { return &enemys_; };
public:
	void SetLockOnEnemy(IEnemy* enemy) { lockOnEnemy_ = enemy; };
};

