#include "PlayerMove.h"

/**
 * @file PlayerMove.cpp
 * @brief ステートマシン
 * @brief プレイヤーの移動している状態
 */

void PlayerMove::SetUp()
{
}

void PlayerMove::Update()
{
	player_->InputVecUpdate();

	if (player_->GetAddVec().x == 0 &&
		player_->GetAddVec().y == 0 &&
		player_->GetAddVec().z == 0)
	{
		player_->GoToState(PlayerState::Idle);
	}

	player_->DogeRoll();
	player_->Jump();
}

void PlayerMove::Draw()
{
}

void PlayerMove::CleanUp()
{
}

void PlayerMove::Move()
{
}
