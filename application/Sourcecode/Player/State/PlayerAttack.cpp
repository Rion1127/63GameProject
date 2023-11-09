#include "PlayerAttack.h"

/**
 * @file PlayerAttack.cpp
 * @brief ステートマシン
 * @brief プレイヤーが攻撃（地上）している状態
 */

void PlayerAttack::SetUp()
{
}

void PlayerAttack::Update()
{
	bool isAttacking = player_->GetAttackManager()->GetIsAttacking();
	if (isAttacking == false) {
		player_->GoToState(PlayerState::Idle);
	}
}

void PlayerAttack::Draw()
{
}

void PlayerAttack::CleanUp()
{
}

void PlayerAttack::Move()
{
}
