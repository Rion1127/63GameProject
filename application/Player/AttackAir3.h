#pragma once
#include "IAttack.h"
class AttackAir3 final :
	public IAttack
{
private:
	float frontDist_;
public:
	AttackAir3();
private:
	void Init() override;
	void MoveUpdate() override;
};

