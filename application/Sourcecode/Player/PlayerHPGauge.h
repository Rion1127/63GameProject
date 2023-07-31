#pragma once
#include "Sprite.h"
class PlayerHPGauge
{
private:
	std::unique_ptr<Sprite> hpBar_;
	std::unique_ptr<Sprite> hpBarMiddle_;	//ダメージが減った分を赤く表示する
	std::unique_ptr<Sprite> hpBarBack_;

	Vector2 offsetPos_;
	Vector2 offsetRate_;
	Vector2 pos_;
	Vector2 scale_;

	int32_t maxHp_;
	int32_t nowHp_;
public:
	PlayerHPGauge();
	void Update(int32_t maxHp, int32_t nowHp);
	void Draw();
	void Damage();
};

