#pragma once
#include "IBullet.h"
class MagicBrrizard :
    public IBullet
{
private:
	float frontDist_;
	float bulletSpeed_;
	std::shared_ptr<ContinuousEmitter> fireEmitter_;
	Timer freezeTimer_;
public:
	~MagicBrrizard();
	MagicBrrizard(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

