#pragma once
#include "IAttack.h"
class Attack2 :
    public IAttack
{
private:
	float frontDist_;
public:
	Attack2();
private:
	void MoveUpdate() override;
};

