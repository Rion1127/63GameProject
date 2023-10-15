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
	IEnemy* lockOnEnemy_;
	//攻撃のデータ・プール
	std::vector<std::unique_ptr<IAttack>> attacks_;

	std::unique_ptr<IAttack> nowAttack_;	//現在処理している攻撃
	std::unique_ptr<IAttack> nextAttack_;	//次に処理する攻撃

	const int32_t MAX_COMBO = 3;
	int32_t comboNum;

	bool isAttacking;
	bool isHardLock_;	//false 近い敵をロックオン, true ロックオンを固定

	float PtoELength_;

	bool isNextAttack_;
public:
	AttackManager();

	void Attack();
	void Update();

	void DrawDebug();
private:
	void CalculatePtoELength();
	void FirstAttackUpdate();	//1コンボ目更新
	void SwitchAttack();
public:
	bool GetIsAttacking() { return isAttacking; }
public:
	void SetLockOnEnemy(IEnemy* enemy) { lockOnEnemy_ = enemy; }
public:
	IAttack* GetNowAttack() { return nowAttack_.get(); }
	IEnemy* GetLockOnEnemy() { return lockOnEnemy_; }
	static void SetPlayer(Player* player) { player_ = player; }
};

