#pragma once
#include "IEnemy.h"
#include <vector>
#include <list>
#include "Timer.h"
#include "EnemyHPGauge.h"
class EnemyManager
{
private:
	std::list<std::unique_ptr<IEnemy>> enemys_;

	std::unique_ptr<Object3d> lockOnObj_;
	IEnemy* lockOnEnemy_;
	Timer lockOnobjTimer_;
	EnemyHPGauge hpGauge_;
public:
	EnemyManager();
	void PreUpdate();
	void PostUpdate();
	void Draw();
	void SpriteDraw();
public:
	std::list<std::unique_ptr<IEnemy>>* GetEnemy() { return &enemys_; };
public:
	void SetLockOnEnemy(IEnemy* enemy) { lockOnEnemy_ = enemy; };
};

