#pragma once
#include "IParticle.h"

/**
 * @file ParticleLanding.h
 * @brief 着地時パーティクルクラス
 */

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

	int32_t vertexCount;

	float speed_;

	std::vector<LandingParticle> landingParticles_;
};

