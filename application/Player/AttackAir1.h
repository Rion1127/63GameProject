#pragma once
#include "IAttack.h"
class AttackAir1 final :
    public IAttack
{
private:
	float frontDist_;
public:
	AttackAir1(IActor* selfActor, IActor* lockOnActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

