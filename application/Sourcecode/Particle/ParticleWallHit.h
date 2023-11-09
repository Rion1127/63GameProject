#pragma once
#include "IParticle.h"
#include "Timer.h"

/**
 * @file ParticleWallHit.h
 * @brief 壁に当たった時のエフェクト
 */

class ParticleWallHit :
	public IParticle
{
private:
	struct WallHitParticle : public Particle
	{
		Vector3 basePos;
		float baseScale;
	};
public:
	ParticleWallHit();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;

	std::vector<WallHitParticle> wallHitParticles_;
};

