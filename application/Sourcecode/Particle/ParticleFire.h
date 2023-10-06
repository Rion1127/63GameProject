#pragma once
#include "IParticle.h"
class ParticleFire final :
	public IParticle
{
private:
	struct FireParticle : public IParticle::Particle
	{
		Vector3 basePos;
		Vector3 addRot;
		float baseScale;
	};
public:
	ParticleFire();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;

	std::vector<FireParticle> fireParticles_;
};

class ParticleFireCircle final :
	public IParticle
{
private:
	struct FireCircleParticle : public IParticle::Particle
	{
		Vector3 basePos;
		Vector3 addRot;
		float baseScale;
	};
public:
	ParticleFireCircle();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
	Vector3* parentPos_;

	std::vector<FireCircleParticle> fireCircleParticles_;
};

