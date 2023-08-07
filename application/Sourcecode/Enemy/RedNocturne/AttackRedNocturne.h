#pragma once
#include "IBullet.h"
class AttackRedNocturne final:
    public IBullet
{
private:
	float frontDist_;
	float bulletSpeed_;
public:
	AttackRedNocturne(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

