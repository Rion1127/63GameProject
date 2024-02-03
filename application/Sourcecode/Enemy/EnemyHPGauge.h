#pragma once
#include "Sprite.h"
#include "IEnemy.h"
#include "Timer.h"

/**
 * @file EnemyHPGauge.h
 * @brief 敵のHPを表示するクラス
 */

class EnemyHPGauge
{
private:
	std::unique_ptr<Sprite> hpBar_;
	std::unique_ptr<Sprite> hpBarMiddle_;	//ダメージが減った分を赤く表示する
	std::unique_ptr<Sprite> hpBarBack_;

	Vector2 offsetPos_;
	Vector2 offsetRate_;
	Vector2 pos_;
	Vector2 scale_;
	Vector2 prevScale_;
	TimerFloat gaugeEaseTimer_;

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
public:
	Vector2 GetPos() { return pos_; }
	Vector2 GetLeftUpPos() {
		Vector2 result = pos_;

		result -= {
			hpBar_->GetTexture().size_.x* hpBar_->GetScale().x,
			hpBar_->GetTexture().size_.y* hpBar_->GetScale().y,
		};

		return result;
	}
};

