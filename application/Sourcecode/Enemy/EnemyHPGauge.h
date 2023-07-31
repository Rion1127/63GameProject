#pragma once
#include "Sprite.h"
#include "IEnemy.h"
class EnemyHPGauge
{
private:
	std::unique_ptr<Sprite> hpBar_;
	std::unique_ptr<Sprite> hpBarMiddle_;	//�_���[�W������������Ԃ��\������
	std::unique_ptr<Sprite> hpBarBack_;

	Vector2 offsetPos_;
	Vector2 offsetRate_;
	Vector2 pos_;
	Vector2 scale_;

	IEnemy* lockOnEnemy_;
	int32_t maxHp_;
	int32_t nowHp_;
public:
	EnemyHPGauge();
	void Update();
	void Draw();
	void Damage();
public:
	void SetLockOnEnemy(IEnemy* enemy) { lockOnEnemy_ = enemy; }
};

