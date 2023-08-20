#include "PlayerFreeze.h"

void PlayerFreeze::SetUp()
{
	player_->SetIsCanMove(false);
}

void PlayerFreeze::Update()
{
	player_->FreezeUpdate();
}

void PlayerFreeze::Draw()
{
}

void PlayerFreeze::CleanUp()
{
	player_->SetIsCanMove(true);
}

void PlayerFreeze::Move()
{
}
