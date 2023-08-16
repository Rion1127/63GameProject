#pragma once
#include "IParticle.h"
class ParticleEnemyDead :
	public IParticle
{
public:
	ParticleEnemyDead();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
};

class ParticleHeart :
	public IParticle
{
public:
	ParticleHeart();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
};