#pragma once
#include "Object3d.h"
#include "IActor.h"
#include "Collision.h"
#include "Gravity.h"
#include "Timer.h"
#include "IAttack.h"
#include "IBullet.h"

/**
 * @file IEnemy.h
 * @brief インターフェースクラス
 * @brief 敵を作る際に継承するクラス
 */

enum class EnemyType {
	Ground,
	Air
};

class Player;

class IEnemy : public IActor
{
protected:
	static Player* splayer_;

	std::string name_;
	EnemyType enemyType_;
	//モデル

	bool isSoftLockOn_;
	bool isHardLockOn_;
	bool isGravityImpact_;	//重力を有効にする
	bool isDead_;
	//軽い攻撃を受けた時のフラグ
	bool isKnock_;
	//フィニッシュ技をくらった時のフラグ
	bool isDown_;
	bool isBulletShot_;

	Vector3 EtoPVec_;

	//体力
	int32_t health_;
	int32_t maxHealth_;

	std::unique_ptr<IAttack> attack_;
public:
	IEnemy(EnemyType type, bool isGravityImpact, int32_t health);
	virtual ~IEnemy() {};
	static void SetPlayer(Player* player) { splayer_ = player; }
	void SetSoftIsLockOn(bool flag) { isSoftLockOn_ = flag; }
	void SetHardIsLockOn(bool flag) { isHardLockOn_ = flag; }
	void SetName(const std::string& name) { name_ = name; }

	void PreUpdate();
	void PostUpdate();
	virtual void Draw();
	void FloorColision(const Vector3& pos);
	void ColPosUpdate();
	void Damage(const Vector3& knockVec, int32_t damageValue, float cooltime);

	//ダメージを受けた時の反応（継承先固有の動き）
	virtual void DamageUpdate() = 0;
	virtual void SetIsNock(bool flag) = 0;
	virtual void SetIsDown(bool flag) = 0;
	virtual void BulletShot(std::list<std::unique_ptr<IBullet>>* bullets) = 0;
	virtual void DrawSprite() = 0;
public:
	Gravity GetGravity() { return gravity_; }
	bool GetIsSoftLockOn() { return isSoftLockOn_; }
	bool GetIsHardLockOn() { return isHardLockOn_; }
	bool GetIsDead() { return isDead_; }
	bool GetIsBulletShot() { return isBulletShot_; }
	TimerFloat GetDamageCoolTime() { return damageCoolTime_; }
	Vector3 GetKnockResist() { return knockResist_; }
	int32_t GetHealth() { return health_; }
	int32_t GetMaxHealth() { return maxHealth_; }
	IAttack* GetAttack() { return attack_.get(); }
	std::string GetName() { return name_; }
	EnemyType GetEnemyType() { return enemyType_; }
protected:
	virtual void MoveUpdate() = 0;

	void UpdateEtoPVec();
};

