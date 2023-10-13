#pragma once
#include <vector>
#include "Object3d.h"
#include "Collision.h"
#include "mInput.h"
#include "PlayerInfo.h"
#include "IActor.h"
#include "Spline.h"
#include "Timer.h"
#include "GameSpeed.h"

enum class AttackType {
	Normal,
	Finish
};

class IAttack
{
private:
	struct AttackCol {
		Object3d colObj_;
		Sphere col_;
		int32_t damage = 10;
		//敵の攻撃当たり判定有効までの時間
		int32_t damageCoolTime = 20;
		//ノックバックの強さ
		Vector3 knockPower = { 0.5f,0.5f,0.5f };
		//上方向のノックバック
		float knockVecY = 0.5f;
		bool isCollision_ = false;
	};
	struct AttackInfo {
		float maxTime;	//攻撃のフレーム数
		float nowTime;
	};
protected:
	TimerFloat timer_;
	std::vector<std::unique_ptr<AttackCol>> attackCol_;
	AttackInfo attackInfo_;
	IActor* selfActor_;
	IActor* lockOnActor_;
	Vector3 attackVec_;
	//当たり判定の軌道
	Spline spline_;
	Vector3 swordPos_;
	AttackType attackType_;
public:
	IAttack(IActor* selfActor, int32_t colNum = 1,
		float maxTime = 20, int32_t damage = 10,
		int32_t damageCoolTime = 25, AttackType attackType = AttackType::Normal);
	virtual ~IAttack() {};
	void Update();
	void DrawCol();
	//lockOnActorへのベクトルを計算する
	Vector3 CalculateFrontVec();
	//lockOnActorへ回転させる
	void CalculateRotToLockOnActor(const Vector3& frontVec);
	void CalculateRotToLockOnActor();
public:
	void SetNowTime(float time) { attackInfo_.nowTime = time; }
	void SetLockOnActor(IActor* lockOnActor) { lockOnActor_ = lockOnActor; };
public:
	AttackInfo GetInfo() { return attackInfo_; }
	std::vector<std::unique_ptr<AttackCol>>* GetAttackCol() { return &attackCol_; }
	IActor* GetSelfActor() { return selfActor_; }
	TimerFloat GetTimer() { return timer_; }
	Vector3 GetSwordPos() { return swordPos_; }
	AttackType GetAttackType() { return attackType_; }
public:
	virtual void Init() = 0;
protected:
	virtual void MoveUpdate() = 0;
};

