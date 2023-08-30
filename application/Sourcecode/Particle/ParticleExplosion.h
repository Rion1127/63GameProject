#pragma once
#include "IParticle.h"

class ParticleSmallExplosion :
	public IParticle
{
public:
	ParticleSmallExplosion();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;

	float endRate_;
	float randOffset_;
};


class ParticleExplosion :
    public IParticle
{
public:
	ParticleExplosion();

	void Add() override;
private:
	void MoveUpdate() override;

	int32_t vertexCount;

	float rgbColor_;
	float endRate_;

	std::shared_ptr<OneceEmitter> otherEmitter_;
};

