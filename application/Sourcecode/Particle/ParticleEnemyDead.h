#pragma once
#include "IParticle.h"
#include <vector>

/**
 * @file ParticleExplosion.h
 * @brief 敵が死んだときのパーティクルクラス
 */

class ParticleEnemyDead :
	public IParticle
{
private:
	struct EnemyDeadParticle : Particle
	{
		float baseScale;
		Vector3 velocity;
		Vector3 addRot;
	};
public:
	ParticleEnemyDead();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
	std::vector<EnemyDeadParticle> enemyDeadParticles_;
};

class ParticleHeart :
	public IParticle
{
private:
	struct HeartParticle : Particle
	{
		float baseScale;
		Vector3 velocity;
		Vector3 addvelo;
		Vector3 basePos;
	};
public:
	ParticleHeart();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;
	std::vector<HeartParticle> heartParticles_;
};