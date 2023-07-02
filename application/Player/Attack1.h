#pragma once
#include "IAttack.h"

class Attack1 final :
	public IAttack
{
private:
	float frontDist_;
public:
	Attack1();
private:
	void Init() override;
	void MoveUpdate() override;
};

