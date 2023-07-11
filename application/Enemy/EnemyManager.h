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

	IEnemy* lockOnEnemy_;
	Timer lockOnobjTimer_;
	EnemyHPGauge hpGauge_;
	std::vector<std::unique_ptr<Sprite>> lockOnSprite_;
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

