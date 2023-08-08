#pragma once
#include "IParticle.h"
#include <unordered_map>
#include <list>

class ParticleManager
{
private:
	std::list<std::shared_ptr<Emitter>> emitters_;
	std::list<std::unique_ptr<Emitter>> uniqueEmitters_;
public:
	static ParticleManager* GetInstance();

	void Update();

	void Draw();

	void AddParticle(std::string name, std::shared_ptr<Emitter> emitter);
	void AddParticle(std::string name, std::unique_ptr<Emitter> emitter);

	void AllClear() { emitters_.clear(); uniqueEmitters_.clear(); };
private:
	ParticleManager();
};

