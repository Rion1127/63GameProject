#pragma once
#include "IAttack.h"
class Attack2 final :
    public IAttack
{
private:
	float frontDist_;
public:
	Attack2();
private:
	void Init() override;
	void MoveUpdate() override;
};

