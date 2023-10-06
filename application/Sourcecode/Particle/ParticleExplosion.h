#pragma once
#include "IParticle.h"
#include <vector>

class ParticleSmallExplosion :
	public IParticle
{
private:
	struct SmallExplosionParticle : public Particle
	{
		Vector3 basePos;
		Vector3 addRot;
	};
public:
	ParticleSmallExplosion();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;

	float endRate_;
	float randOffset_;

	std::vector<SmallExplosionParticle> smallExplosionParticles_;
};


class ParticleExplosion :
	public IParticle
{
private:
	struct ExplosionParticle : public Particle
	{
		Vector3 basePos;
		Vector3 addRot;
		float baseScale;
	};
public:
	ParticleExplosion();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;

	float rgbColor_;
	float endRate_;

	std::shared_ptr<OneceEmitter> otherEmitter_;

	std::vector<ExplosionParticle> explosionParticles_;
};

