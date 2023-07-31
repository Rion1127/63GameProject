#pragma once
#include "IParticle.h"
class ParticleEnemyDead :
	public IParticle
{
public:
	ParticleEnemyDead();

	void Add(int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale) override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
};

class ParticleHeart :
	public IParticle
{
public:
	ParticleHeart();

	void Add(int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale) override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
};