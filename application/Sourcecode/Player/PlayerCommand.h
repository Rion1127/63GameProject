#pragma once
#include "Sprite.h"
#include "AttackManager.h"
#include "IEnemy.h"
#include "PlayerInfo.h"
#include "Timer.h"
#include <array>

/**
 * @file PlayerCommand.h
 * @brief プレイヤーのコマンドシステムを処理している
 */

struct CommandSprite
{
private:
	enum ButtonName {
		Abutton,
		Bbutton,
		Xbutton,
		Ybutton,
	};
	struct ButtonCommand{
		ButtonName buttonName_;
		Sprite commandFrame_;
		Sprite commandButton_;
		Sprite commandTex_;
	};
private:
	std::unique_ptr<Sprite> commandTitle_;
	std::array<std::unique_ptr<ButtonCommand>,4> buttonCommand_;
	Vector2 basePos_;
public:
	CommandSprite();
	void Update(PlayerState state,int32_t commbo);

	void DrawSprite();

	void ResetEase();
public:
	
};

class Player;

class PlayerCommand
{
private:
	enum class Command {
		Attack,
		Magic,
		END,
	};
private:
	std::unique_ptr<CommandSprite> mainCommandSprite_;
	IEnemy* lockOnEnemy_;
	Player* player_;

	AttackManager attackManager_;
	Command selectCommand_;
	int16_t commandNum_;
public:
	PlayerCommand();

	void Update();

	void Draw();
	void DrawSprite();
public:
	void SetLockOnEnemy(IEnemy* enemy) { lockOnEnemy_ = enemy; }
	void SetPlayer(Player* player) {player_ = player;}
public:
	AttackManager* GetAttackManager() { return &attackManager_; }
	IEnemy* GetLockOnEnemy() { return lockOnEnemy_; }
};

