#pragma once
#include "Sprite.h"
#include "AttackManager.h"
#include "IEnemy.h"
#include "PlayerInfo.h"
#include "Timer.h"
class PlayerCommand
{
private:
	enum class Command {
		Attack,
		Magic,
		END
	};
private:
	std::unique_ptr<Sprite> sprite_;
	std::unique_ptr<Sprite> commandTex1_;

	std::array<std::unique_ptr<Sprite>, 2> frame_;
	std::array<std::unique_ptr<Sprite>, 2> commandTex_;

	std::array<Vector2,2> basePos_;

	Timer easeTimer_;
private:
	IEnemy* lockOnEnemy_;
	PlayerState* playerState_;
	AttackManager attack_;

	Command selectCommand_;
	int16_t commandNum_;
public:
	PlayerCommand();

	void Update();
	void SpriteUpdate();

	void Draw();
	void DrawSprite();

public:
	void SetLockOnEnemy(IEnemy* enemy) { lockOnEnemy_ = enemy; }
	void SetPlayerInfo(PlayerState* state) { playerState_ = state; }
	AttackManager* GetAttackManager() { return &attack_; }
	IEnemy* GetLockOnEnemy() { return lockOnEnemy_; }
};

