#pragma once
#include "IParticle.h"
#include "Timer.h"
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
		Timer timer_;
	};
public:
	ParticleHitAttack();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;

	std::vector<HitAttackParticle> hitAttackParticles_;
	
	Color color_;
};

class ParticleHitCircle final :
	public IParticle
{
private:
	struct HitAttackParticle : public Particle
	{
		Vector3 addRot;
		float baseScale;
		float endScale;
	};
public:
	ParticleHitCircle();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
	std::vector<HitAttackParticle> hitCircleParticles_;

	float endScale_;
};