#pragma once
#include "BaseAttack.h"
#include "IAttack.h"
#include "IEnemy.h"
#include "PlayerInfo.h"
#include <vector>
#include <map>
#include <unordered_map>
#include "mInput.h"

/**
 * @file AttackManager.h
 * @brief プレイヤーの基本攻撃を管理するクラス
 */

class Player;

class AttackDataPool {
private:
	//攻撃のデータ・プール
	std::unordered_map<std::string, BaseAttack::AttackData> attacks_;
	//データプールから攻撃を指定する文字列を指定する
	std::unordered_map<std::string, std::string> attackKeys_;

	//ディレクトリ名
	std::string attackInfoDir_;
	std::string attackKeyDir_;
public:
	AttackDataPool();
	
	void LoadAttackFile(std::string fileName);
	void LoadattackKeys(std::string fileName);
public:
	std::unordered_map<std::string, BaseAttack::AttackData> GetAttacks() { return attacks_; }
	std::unordered_map<std::string, std::string> GetKeyName() { return attackKeys_; }
};

class AttackManager
{
private:
	static Player* player_;
	IEnemy* lockOnEnemy_;

	AttackDataPool datapool_;
	
	std::unique_ptr<BaseAttack> nowAttack_;	//現在処理している攻撃
	std::unique_ptr<BaseAttack> nextAttack_;	//次に処理する攻撃

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
	BaseAttack* GetNowAttack() { return nowAttack_.get(); }
	IEnemy* GetLockOnEnemy() { return lockOnEnemy_; }
	static void SetPlayer(Player* player) { player_ = player; }
};

