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

class ParticleDebri {
private:
	struct Debri {
		Vector3 addVec;
		Vector3 addRot;
		Vector3 pos;
		Gravity gravity;
		Object3d* obj;
		WorldTransform WT;
	};
private:
	std::vector<Debri> debris_;
	TimerFloat timer_;
public:
	ParticleDebri(Object3d* obj,int32_t addNum,Vector3 pos,float activeTime);
	void Update();
	void Draw();
public:
	bool isGetEnd() { return timer_.GetIsEnd(); }
};