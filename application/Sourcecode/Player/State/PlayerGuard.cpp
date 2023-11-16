#include "PlayerGuard.h"

/**
 * @file PlayerGuard.cpp
 * @brief ステートマシン
 * @brief プレイヤーが防御している状態
 */

void PlayerGuard::SetUp()
{
}

void PlayerGuard::Update()
{
	player_->GuardUpdate();
	player_->PlayerRotUpdate();
}

void PlayerGuard::Draw()
{
}

void PlayerGuard::CleanUp()
{
}

void PlayerGuard::Move()
{
}
