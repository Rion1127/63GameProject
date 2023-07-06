#pragma once
#include "Sprite.h"
#include "IEnemy.h"
class EnemyHPGauge
{
private:
	std::unique_ptr<Sprite> hpBar_;
	std::unique_ptr<Sprite> hpBarBack_;

	IEnemy* lockOnEnemy_;
	size_t maxHp_;
	size_t nowHp_;
public:
	EnemyHPGauge();
	void Update();
	void Draw();
public:
	void SetLockOnEnemy(IEnemy* enemy) { lockOnEnemy_ = enemy; }
};

