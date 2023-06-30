#pragma once
#include <vector>
#include "Object3d.h"
#include "Collision.h"
#include "mInput.h"
#include "PlayerInfo.h"

struct AttackCol {
	Object3d colObj_;
	Sphere col_;
};

struct AttackInfo {
	size_t maxTime;	//çUåÇÇÃÉtÉåÅ[ÉÄêî
};

class IAttack
{
protected:
	std::vector<std::unique_ptr<AttackCol>> attackCol_;
	AttackInfo attackInfo_;
	static PlayerInfo* playerInfo_;
public:
	IAttack(size_t colNum,size_t maxTime);

	void Update();
	void DrawCol();
public:
	AttackInfo GetInfo() { return attackInfo_; }
	static void SetPlayerInfo(PlayerInfo* info) { playerInfo_ = info; }
protected:
	virtual void MoveUpdate() = 0;
};

