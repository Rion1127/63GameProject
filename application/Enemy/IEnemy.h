#pragma once
#include "Object3d.h"
#include "IActor.h"
#include "Collision.h"
#include "Gravity.h"
#include "Timer.h"
#include "IAttack.h"

enum class EnemyType {
	Ground,	
	Air
};

class Player;

class IEnemy : public IActor
{
protected:
	static Player* splayer_;

	EnemyType enemyType_;
	//ÉÇÉfÉã
	
	bool isSoftLockOn_;
	bool isHardLockOn_;
	bool isGravityImpact_;	//èdóÕÇóLå¯Ç…Ç∑ÇÈ
	bool isDead_;
	bool isKnock_;
	
	//ëÃóÕ
	int32_t health_;
	int32_t maxHealth_;

	std::unique_ptr<IAttack> attack_;
public:
	IEnemy(EnemyType type,bool isGravityImpact, int32_t health);
	virtual ~IEnemy() {};
	static void SetPlayer(Player* player) { splayer_ = player; }
	void SetSoftIsLockOn(bool flag) { isSoftLockOn_ = flag; }
	void SetHardIsLockOn(bool flag) { isHardLockOn_ = flag; }
	
	void PreUpdate();
	void PostUpdate();
	virtual void Draw();
	void FloorColision();
	void ColPosUpdate();
	void HitPlayerAttack(Vector3 knockVec, int32_t damageValue,int32_t cooltime);

	virtual void SetIsNock(bool flag) = 0;
	virtual void DrawSprite() = 0;
public:
	Gravity GetGravity() { return gravity_; }
	bool GetIsSoftLockOn() { return isSoftLockOn_; }
	bool GetIsHardLockOn() { return isHardLockOn_; }
	bool GetIsDead() { return isDead_; }
	Timer GetDamageCoolTime() { return damageCoolTime_; }
	Vector3 GetKnockResist() { return knockResist_; }
	int32_t GetHealth() { return health_; }
	int32_t GetMaxHealth() { return maxHealth_; }
	IAttack* GetAttack() { return attack_.get(); }
protected:
	virtual void MoveUpdate() = 0;
};

