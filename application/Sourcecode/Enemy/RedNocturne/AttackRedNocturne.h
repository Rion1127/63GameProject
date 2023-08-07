#pragma once
#include "IAttack.h"
class AttackRedNocturne final:
    public IAttack
{
private:
	float frontDist_;
	float bulletSpeed_;
public:
	AttackRedNocturne(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

