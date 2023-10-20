#pragma once
#include "IParticle.h"

/**
 * @file ParticleDash.h
 * @brief 移動している時のパーティクルクラス
 */

class ParticleDash :
    public IParticle
{
private:
	struct DashParticle : public Particle
	{
		Vector3 basePos;
		Vector3 addRot;
		Vector3 addVec;
		float baseScale;
	};
public:
	ParticleDash();

	void Add() override;
private:
	void MoveUpdate() override;

	void Test(std::vector<Particle> particle) { particle; }

	int32_t vertexCount;

	float speed_;

	std::vector<DashParticle> landingParticles_;
};

