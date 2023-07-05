#pragma once
#include "IParticle.h"
class ParticleTest :
    public IParticle
{
public:
	ParticleTest();

	void Add(size_t addNum = 1,size_t life = 60, Vector3 pos = { 1,1,1 }, float scale = 1.f);
private:
	void MoveUpdate() override;

	size_t vertexCount = 6;
};

