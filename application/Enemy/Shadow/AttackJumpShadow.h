#pragma once
#include "IAttack.h"
class AttackJumpShadow :
	public IAttack
{
private:
	float frontDist_;
public:
	AttackJumpShadow(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

