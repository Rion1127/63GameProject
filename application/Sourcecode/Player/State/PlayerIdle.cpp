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
	//プレイヤーを正面に向かせる
	player_->SetObjAngleX(0);
	player_->SetObjAngleZ(0);

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
