#pragma once
#include "IAttack.h"
class AttackFinishBreak :
    public IAttack
{
private:
	float frontDist_;
public:
	AttackFinishBreak(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

