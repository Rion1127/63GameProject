#pragma once
#include "IAttack.h"
class Attack3 :
    public IAttack
{
private:
	float frontDist_;
public:
	Attack3();
private:
	void MoveUpdate() override;
};

