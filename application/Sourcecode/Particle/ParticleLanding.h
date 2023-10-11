#pragma once
#include "IParticle.h"
class ParticleLanding :
    public IParticle
{
private:
	struct LandingParticle : public Particle
	{
		Vector3 basePos;
		Vector3 addRot;
		Vector3 addVec;
		float baseScale;
	};
public:
	ParticleLanding();

	void Add() override;
private:
	void MoveUpdate() override;

	void Test(std::vector<Particle> particle) { particle; }

	int32_t vertexCount;

	float speed_;

	std::vector<LandingParticle> landingParticles_;
};

