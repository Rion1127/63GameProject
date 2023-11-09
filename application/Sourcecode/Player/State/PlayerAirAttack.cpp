#include "PlayerAirAttack.h"

/**
 * @file PlayerAirAttack.cpp
 * @brief ステートマシン
 * @brief プレイヤーが攻撃（空中）している状態
 */

void PlayerAirAttack::SetUp()
{
}

void PlayerAirAttack::Update()
{
	bool isAttacking = player_->GetAttackManager()->GetIsAttacking();
	if (isAttacking == false) {
		player_->GoToState(PlayerState::Jump);
	}
}

void PlayerAirAttack::Draw()
{
}

void PlayerAirAttack::CleanUp()
{
}

void PlayerAirAttack::Move()
{
}
