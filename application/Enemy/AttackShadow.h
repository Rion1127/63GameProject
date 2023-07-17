#pragma once
#include "IAttack.h"
class AttackShadow final:
    public IAttack
{
private:
	float frontDist_;
public:
	AttackShadow(IActor* selfActor, IActor* lockOnActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

