#include "PlayerMagic.h"

void PlayerMagic::SetUp()
{
	player_->SetIsCanMove(false);
}

void PlayerMagic::Update()
{
	if (player_->GetFreezeTime()->GetIsEnd()) {
		player_->GoToState(PlayerState::Idle);
	}
}

void PlayerMagic::Draw()
{
}

void PlayerMagic::CleanUp()
{
	player_->SetIsCanMove(true);
}
