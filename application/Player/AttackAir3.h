#pragma once
#include "IAttack.h"
class AttackAir3 :
	public IAttack
{
private:
	float frontDist_;
public:
	AttackAir3();
private:
	void MoveUpdate() override;
};

