#include "PlayerAirAttack.h"

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
