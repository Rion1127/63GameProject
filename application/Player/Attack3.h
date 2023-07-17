#pragma once
#include "IAttack.h"
class Attack3 final :
    public IAttack
{
private:
	float frontDist_;
public:
	Attack3(IActor* selfActor, IActor* lockOnActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

