#pragma once
#include <vector>
#include "Object3d.h"
#include "Collision.h"
#include "mInput.h"
#include "PlayerInfo.h"
#include "IActor.h"
#include "Spline.h"

/**
 * @file IBullet.h
 * @brief インターフェースクラス
 * @brief 弾・遠距離魔法を作る際に継承するクラス
 */

class IBullet
{
private:
	struct AttackCol {
		Object3d colObj_;
		Sphere col_;
		int32_t damage = 10;
		//敵の攻撃当たり判定有効までの時間
		float damageCoolTime = 20;
		//ノックバックの強さ
		Vector3 knockPower = { 0.5f,0.5f,0.5f };
		//上方向のノックバック
		float knockVecY = 0.5f;
	};
	struct AttackInfo {
		int32_t maxTime;	//攻撃のフレーム数
		int32_t nowTime;
	};
protected:
	std::unique_ptr<AttackCol> attackCol_;
	AttackInfo attackInfo_;
	IActor* selfActor_;
	IActor* lockOnActor_;
	Vector3 attackVec_;
	Spline spline_;

	Timer aliveTimer_;
	bool isDead_;
	int32_t costMP_;
public:
	IBullet(IActor* selfActor,
		int32_t maxTime = 20, int32_t damage = 10, float damageCoolTime = 25.f, int32_t costMP = 10);
	virtual ~IBullet() {};
	void Update();
	void DrawCol();
	Vector3 CalculateFrontVec();
	void CalculateRotToLockOnActor(const Vector3& frontVec);
public:
	void SetNowTime(int32_t time) { attackInfo_.nowTime = time; }
	void SetLockOnActor(IActor* lockOnActor) { lockOnActor_ = lockOnActor; };
	void SetIsDead(bool flag) { isDead_ = flag; }
public:
	AttackInfo GetInfo() { return attackInfo_; }
	std::unique_ptr<AttackCol>* GetAttackCol() { return &attackCol_; }
	IActor* GetSelfActor() { return selfActor_; }
	bool GetIsDead() { return isDead_; }
	int32_t GetCostMP() { return costMP_; }
public:
	virtual void Init() = 0;
protected:
	virtual void MoveUpdate() = 0;
};

