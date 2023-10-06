#pragma once
#include "IParticle.h"
#include <unordered_map>
#include <list>

class ParticleManager
{
private:
	std::list<std::shared_ptr<IEmitter>> emitters_;
public:
	static ParticleManager* GetInstance();

	void Update();

	void Draw();

	void AddParticle(std::string name, std::shared_ptr<IEmitter> emitter);

	void AllClear() { emitters_.clear(); };

	void AllReset();
private:
	ParticleManager();
};

