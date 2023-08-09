#pragma once
#include "IParticle.h"
class ParticleHitAttack final:
    public IParticle
{
public:
	ParticleHitAttack();

	void Add(int32_t addNum, int32_t time,
		Vector3 pos, Vector3 addVec,
		float scale, Vector3* parentPos = nullptr) override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
};