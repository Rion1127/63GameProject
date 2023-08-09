#pragma once
#include "IParticle.h"
#include "Timer.h"
class ParticleWallHit :
    public IParticle
{
public:
	ParticleWallHit();

	void Add(int32_t addNum, int32_t time,
		Vector3 pos, Vector3 addVec,
		float scale, Vector3* parentPos = nullptr) override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
};

