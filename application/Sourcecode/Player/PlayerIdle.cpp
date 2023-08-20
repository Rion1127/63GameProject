#include "PlayerIdle.h"

void PlayerIdle::SetUp()
{
}

void PlayerIdle::Update()
{
	if (player_->GetAddVec().x != 0 ||
		player_->GetAddVec().y != 0 ||
		player_->GetAddVec().z != 0)
	{
		player_->GoToState(PlayerState::Move);
	}

	player_->Guard();
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
