#pragma once
#include "Sprite.h"
#include "AttackManager.h"
#include "IEnemy.h"
#include "PlayerInfo.h"
#include "Timer.h"
#include <array>
#include "Timer.h"

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
	struct SpecialCommand {
		ButtonCommand buttonCommand_;
		TimerFloat easeTimer_;
		bool isActive_;
		float easeStartButtonPosX_;
		float easeEndButtonPosX_;
		float easeStartFramePosX_;
		float easeEndFramePosX_;
		float easeStartTexPosX_;
		float easeEndTexPosX_;
		float easeDist_;
	};
private:
	std::unique_ptr<Sprite> commandTitle_;
	std::array<std::unique_ptr<ButtonCommand>, 4> buttonCommand_;
	std::array<std::unique_ptr<SpecialCommand>, 3> specialCommand_;
	Vector2 basePos_;
	Color mainColor_;
	Color specialFinishColor_;
	float anSelectColRate_;
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

