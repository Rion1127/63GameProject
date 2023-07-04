#pragma once
#include "IParticle.h"
#include <list>
class ParticleManager
{
public:
	static ParticleManager* GetInstance();

	void Update();

	void PreDraw();
	void Draw();

private:
	std::list<std::unique_ptr<IParticle>> particles_;
	

};

