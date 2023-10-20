#include "PlayerJump.h"

/**
 * @file PlayerKnock.cpp
 * @brief ステートマシン
 * @brief プレイヤーがジャンプしている状態
 */

void PlayerJump::SetUp()
{
}

void PlayerJump::Update()
{
	player_->JumpUpdate();
	player_->InputVecUpdate();
}

void PlayerJump::Draw()
{
}

void PlayerJump::CleanUp()
{
}

void PlayerJump::Move()
{
}
