#pragma once
#include "IAttack.h"
class AttackAir2 :
    public IAttack
{
private:
	float frontDist_;
public:
	AttackAir2();
private:
	void MoveUpdate() override;
};

