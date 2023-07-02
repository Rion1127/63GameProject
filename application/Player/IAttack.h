#pragma once
#include <vector>
#include "Object3d.h"
#include "Collision.h"
#include "mInput.h"
#include "PlayerInfo.h"
#include "IActor.h"

struct AttackCol {
	Object3d colObj_;
	Sphere col_;
	float damage = 10;
	//“G‚ÌUŒ‚“–‚½‚è”»’è—LŒø‚Ü‚Å‚ÌŠÔ
	int32_t damageCoolTime = 20;
};

struct AttackInfo {
	size_t maxTime;	//UŒ‚‚ÌƒtƒŒ[ƒ€”
	size_t nowTime;
};

class IAttack
{
protected:
	std::vector<std::unique_ptr<AttackCol>> attackCol_;
	AttackInfo attackInfo_;
	static PlayerInfo* splayerInfo_;
	static IActor* lockOnActor_;
	Vector3 attackVec_;
public:
	IAttack(size_t colNum = 1,size_t maxTime = 20,float damage = 10,int32_t damageCoolTime = 25);
	
	void Update();
	void DrawCol();
public:
	void SetNowTime(size_t time) { attackInfo_.nowTime = time; }
public:
	AttackInfo GetInfo() { return attackInfo_; }
	std::vector<std::unique_ptr<AttackCol>>* GetAttackCol() { return &attackCol_; }
	static PlayerInfo* GetPlayerInfo() { return splayerInfo_; }
public:
	static void SetPlayerInfo(PlayerInfo* info) { splayerInfo_ = info; }
	static void SetLockOnActor(IActor* info) { lockOnActor_ = info; }
public:
	virtual void Init() = 0;
protected:
	virtual void MoveUpdate() = 0;
};

