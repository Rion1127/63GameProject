#pragma once
#include "IParticle.h"
class ParticleHitAttack final:
    public IParticle
{
public:
	ParticleHitAttack();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
};