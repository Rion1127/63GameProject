#include "PlayerIdle.h"

/**
 * @file PlayerIdle.cpp
 * @brief ステートマシン
 * @brief プレイヤーが何もしていない状態
 */

void PlayerIdle::SetUp()
{
}

void PlayerIdle::Update()
{
	if (player_->GetInputVec().x != 0 ||
		player_->GetInputVec().y != 0)
	{
		player_->GoToState(PlayerState::Move);
	}

	player_->Guard();
	player_->Jump();
	player_->InputVecUpdate();
}

void PlayerIdle::Draw()
{
}

void PlayerIdle::CleanUp()
{
}

void PlayerIdle::Move()
{
}
