#pragma once
#include "IParticle.h"
class ParticleTest final:
    public IParticle
{
public:
	ParticleTest();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount = 6;
};

