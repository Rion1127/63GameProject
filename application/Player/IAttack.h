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
	int32_t damage = 10;
	//敵の攻撃当たり判定有効までの時間
	int32_t damageCoolTime = 20;
	//ノックバックの強さ
	Vector3 knockPower = { 0.5f,0.5f,0.5f };
	//上方向のノックバック
	float knockVecY = 0.5f;
};

struct AttackInfo {
	int32_t maxTime;	//攻撃のフレーム数
	int32_t nowTime;
};

class IAttack
{
protected:
	std::vector<std::unique_ptr<AttackCol>> attackCol_;
	AttackInfo attackInfo_;
	IActor* selfActor_;
	IActor* lockOnActor_;
	Vector3 attackVec_;
public:
	IAttack(IActor* selfActor, IActor* lockOnActor,int32_t colNum = 1, int32_t maxTime = 20, int32_t damage = 10,int32_t damageCoolTime = 25);
	virtual ~IAttack(){};
	void Update();
	void DrawCol();
public:
	void SetNowTime(int32_t time) { attackInfo_.nowTime = time; }
public:
	AttackInfo GetInfo() { return attackInfo_; }
	std::vector<std::unique_ptr<AttackCol>>* GetAttackCol() { return &attackCol_; }
	IActor* GetSelfActor() { return selfActor_; }
public:
	virtual void Init() = 0;
protected:
	virtual void MoveUpdate() = 0;
};

