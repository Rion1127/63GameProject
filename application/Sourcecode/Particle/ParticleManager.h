#pragma once
#include "IParticle.h"
#include <unordered_map>
#include <list>

class ParticleManager
{
private:
	std::list<std::shared_ptr<Emitter>> emitters_;
public:
	static ParticleManager* GetInstance();

	void Update();

	void Draw();

	void AddParticle(std::string name, std::shared_ptr<Emitter> emitter);
	
	void AllClear() { emitters_.clear();};
private:
	ParticleManager();
};

