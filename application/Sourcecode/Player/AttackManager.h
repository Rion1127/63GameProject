#pragma once
#include "IAttack.h"
#include "IEnemy.h"
#include "PlayerInfo.h"
#include <vector>
#include <map>
#include "mInput.h"
#include "Attack1.h"
#include "Attack2.h"
#include "Attack3.h"

#include "AttackAir1.h"
#include "AttackAir2.h"
#include "AttackAir3.h"

class Player;

class AttackManager
{
private:
	static Player* player_;
	PlayerState* playerState_;
	Controller* controller_ = nullptr;
	//攻撃のデータ・プール
	std::vector<std::unique_ptr<IAttack>> attacks_;

	std::unique_ptr<IAttack> nowAttack_;	//現在処理している攻撃
	std::unique_ptr<IAttack> nextAttack_;	//次に処理する攻撃

	const int32_t MAX_COMBO = 3;
	int32_t comboNum;

	int32_t timer_ = 0;

	bool isAttacking;
	bool isHardLock_;	//false 近い敵をロックオン, true ロックオンを固定
	IEnemy* lockOnEnemy_;
public:
	AttackManager();

	void Update();

	void DrawDebug();
public:
	bool GetIsAttacking() { return isAttacking; }
public:
	void SetLockOnEnemy(IEnemy* enemy) { lockOnEnemy_ = enemy; }
	void SetPlayer(PlayerState* playerState) { playerState_ = playerState; }
public:
	IAttack* GetNowAttack() { return nowAttack_.get(); }
	IEnemy* GetLockOnEnemy() { return lockOnEnemy_; }
	static void SetPlayer(Player* player) { player_ = player; }
};

