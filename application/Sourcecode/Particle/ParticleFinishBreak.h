#pragma once
#include "IParticle.h"
class ParticleFinishBreak :
    public IParticle
{
private:
	struct FinishBreakParticle : public Particle
	{
		Vector3 basePos;
		Vector3 addRot;
		Vector3 addVec;
		float baseScale;
	};
public:
	ParticleFinishBreak();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;

	float speed_;

	std::vector<FinishBreakParticle> landingParticles_;
};

