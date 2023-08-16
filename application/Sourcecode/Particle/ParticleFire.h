#pragma once
#include "IParticle.h"
class ParticleFire final :
    public IParticle
{
public:
	ParticleFire();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
};

class ParticleFireCircle final :
	public IParticle
{
public:
	ParticleFireCircle();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
	Vector3* parentPos_;
};

