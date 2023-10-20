#include "PlayerDogeRoll.h"

/**
 * @file PlayerDogeRoll.cpp
 * @brief ステートマシン
 * @brief プレイヤーが回避（ドッジロール）している状態
 */

void PlayerDogeRoll::SetUp()
{
}

void PlayerDogeRoll::Update()
{
	player_->DogeRollUpdate();
	player_->PlayerRotUpdate();
}

void PlayerDogeRoll::Draw()
{
}

void PlayerDogeRoll::CleanUp()
{
}

void PlayerDogeRoll::Move()
{
}
