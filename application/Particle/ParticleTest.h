#pragma once
#include "IParticle.h"
class ParticleTest :
    public IParticle
{
public:
	ParticleTest();

	void Add(int life = 60, Vector3 pos = { 1,1,1 }, float scale = 1.f);
private:
	void MoveUpdate() override;

	const int vertexCount = 6;
};

