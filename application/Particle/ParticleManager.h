#pragma once
#include "IParticle.h"
#include <list>
class ParticleManager
{
public:
	static ParticleManager* GetInstance();
	ParticleManager();

	void Update();

	void Draw();

private:
	std::list<std::unique_ptr<IParticle>> particles_;
	

};

