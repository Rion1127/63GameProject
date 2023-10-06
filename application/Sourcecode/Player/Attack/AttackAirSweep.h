#pragma once
#include "IAttack.h"
#include "Timer.h"
class AttackAirSweep :
	public IAttack
{
private:
	float frontDist_;
	//当たり判定が回転するスピード
	float rotateSpeed_;
	float colRadius_;
public:
	AttackAirSweep(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;

};

