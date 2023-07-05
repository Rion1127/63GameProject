#pragma once
#include "IParticle.h"
class ParticleHitAttack :
    public IParticle
{
public:
	ParticleHitAttack();

	void Add(size_t addNum, size_t time, Vector3 pos, Vector3 addVec, float scale) override;
private:
	void MoveUpdate() override;

	size_t vertexCount = 6;
};

