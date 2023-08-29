#pragma once
#include "IAttack.h"
class AttackSlide :
    public IAttack
{
private:
	float frontDist_;
public:
	AttackSlide(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

