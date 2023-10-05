#include "PlayerAttack.h"

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
