#pragma once
#include "IAttack.h"

class FirstAttack :
	public IAttack
{
private:
	float frontDist_;
public:
	FirstAttack();
private:
	void MoveUpdate() override;
};

