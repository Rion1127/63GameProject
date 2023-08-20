#include "PlayerMove.h"

void PlayerMove::SetUp()
{
}

void PlayerMove::Update()
{
	if (player_->GetAddVec().x == 0 &&
		player_->GetAddVec().y == 0 &&
		player_->GetAddVec().z == 0)
	{
		player_->GoToState(PlayerState::Idle);
	}

	player_->DogeRoll();
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
