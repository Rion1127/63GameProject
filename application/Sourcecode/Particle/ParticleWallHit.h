#pragma once
#include "IParticle.h"
#include "Timer.h"
class ParticleWallHit :
    public IParticle
{
public:
	ParticleWallHit();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
};

