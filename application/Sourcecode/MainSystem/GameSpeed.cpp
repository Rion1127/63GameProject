#include "GameSpeed.h"

float GameSpeed::kGameSpeed_ = 1.f;
float GameSpeed::kPlayerSpeed_ = 1.f;
float GameSpeed::kEnemySpeed_ = 1.f;
float GameSpeed::kResultPlayerSpeed_ = 1.f;
float GameSpeed::kResultEnemySpeed_ = 1.f;

void GameSpeed::Update()
{
	kResultPlayerSpeed_ = kGameSpeed_ * kPlayerSpeed_;
	kResultEnemySpeed_ = kGameSpeed_ * kEnemySpeed_;
}
