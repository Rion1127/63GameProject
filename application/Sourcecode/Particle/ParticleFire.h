#pragma once
#include "IParticle.h"
class ParticleFire final :
    public IParticle
{
public:
	ParticleFire();

	void Add(int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale) override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
};

class ParticleFireCircle final :
	public IParticle
{
public:
	ParticleFireCircle();

	void Add(int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale) override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
};

