#pragma once
#include "IAttack.h"
class AttackAir2 final :
    public IAttack
{
private:
	float frontDist_;
public:
	AttackAir2(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

