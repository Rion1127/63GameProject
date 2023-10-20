#include "PlayerFreeze.h"

/**
 * @file PlayerFreeze.cpp
 * @brief ステートマシン
 * @brief プレイヤーが硬直している状態
 */

void PlayerFreeze::SetUp()
{
	player_->SetIsCanMove(false);
}

void PlayerFreeze::Update()
{
	player_->FreezeUpdate();
}

void PlayerFreeze::Draw()
{
}

void PlayerFreeze::CleanUp()
{
	player_->SetIsCanMove(true);
}

void PlayerFreeze::Move()
{
}
