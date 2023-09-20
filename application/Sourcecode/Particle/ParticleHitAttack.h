#pragma once
#include "IParticle.h"
class ParticleHitAttack final:
    public IParticle
{
private:
	struct HitAttackParticle : public Particle
	{
		Vector3 basePos;
		Vector3 velocity;
		Vector3 addvelocity;
		Vector3 addRot;
		float baseScale;
	};
public:
	ParticleHitAttack();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;

	std::vector<HitAttackParticle> hitAttackParticles_;
};