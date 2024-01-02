#pragma once
#include "IParticle.h"
#include "Gravity.h"
class ParticleGroundStump :
    public IParticle
{
private:
	struct GroundStumpParticle : public Particle
	{
		Vector3 addRot;
		Vector3 addVec;
		Gravity gravity;
	};
public:
	ParticleGroundStump();

	void Add() override;
private:
	void MoveUpdate() override;
	
	int32_t vertexCount;
	std::vector<GroundStumpParticle> landingParticles_;
};

class ParticleStumpSmoke :
	public IParticle
{
private:
	struct StumpSmokeParticle : public Particle
	{
		Vector3 addRot;
		Vector3 addVec;
		Gravity gravity;
	};
public:
	ParticleStumpSmoke();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
	std::vector<StumpSmokeParticle> landingParticles_;
};