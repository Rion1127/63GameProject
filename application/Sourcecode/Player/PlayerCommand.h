#pragma once
#include "Sprite.h"
#include "AttackManager.h"
#include "IEnemy.h"
#include "PlayerInfo.h"
class PlayerCommand
{
private:
	enum class Command {
		Attack,
		Magic
	};
private:
	std::unique_ptr<Sprite> sprite_;

private:
	IEnemy* lockOnEnemy_;
	PlayerState* playerState_;
	AttackManager attack_;

	Command selectCommand_;
public:
	PlayerCommand();

	void Update();

	void Draw();

public:
	void SetLockOnEnemy(IEnemy* enemy) { lockOnEnemy_ = enemy; }
	void SetPlayerInfo(PlayerState* state) { playerState_ = state; }
};

