#include "GameSpeed.h"

float GameSpeed::gameSpeed_ = 1.f;
float GameSpeed::playerSpeed_ = 1.f;
float GameSpeed::enemySpeed_ = 1.f;

float GameSpeed::resultPlayerSpeed_ = 1.f;
float GameSpeed::resultEnemySpeed_ = 1.f;

void GameSpeed::Update()
{
	resultPlayerSpeed_ = gameSpeed_ * playerSpeed_;
	resultEnemySpeed_ = gameSpeed_ * enemySpeed_;
}
