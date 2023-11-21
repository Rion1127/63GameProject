#include "PlayerMagic.h"

/**
 * @file PlayerMagic.cpp
 * @brief ステートマシン
 * @brief プレイヤーが攻撃（魔法）している状態
 */

void PlayerMagic::SetUp()
{
	player_->SetIsCanMove(false);
}

void PlayerMagic::Update()
{
	if (player_->GetFreezeTime()->GetIsEnd()) {
		player_->GoToState(PlayerState::Idle);
	}

	player_->SetObjAngleX(0);
	player_->SetObjAngleZ(0);

	player_->PlayerRotUpdate();
}

void PlayerMagic::Draw()
{
}

void PlayerMagic::CleanUp()
{
	player_->SetIsCanMove(true);
}
