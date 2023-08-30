#pragma once
#include "IParticle.h"
class ParticleExplosion :
    public IParticle
{
public:
	ParticleExplosion();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;

	float endRate_;
};

