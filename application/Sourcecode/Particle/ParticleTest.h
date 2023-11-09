#pragma once
#include "IParticle.h"

/**
 * @file ParticleTest.h
 * @brief テスト用エフェクトクラス
 */

class ParticleTest final :
	public IParticle
{
private:
	struct TestParticle : public Particle
	{
		Vector3 basePos;
		float baseScale;
	};
public:
	ParticleTest();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount = 6;

	std::vector<TestParticle> testParticles_;
};

