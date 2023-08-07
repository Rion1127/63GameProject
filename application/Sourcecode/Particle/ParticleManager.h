#pragma once
#include "IParticle.h"
#include <unordered_map>
#include <list>

class ParticleManager
{
private:
	std::list<std::unique_ptr<IParticle>> particles_;
public:
	static ParticleManager* GetInstance();

	void Update();

	void Draw();

	template<class T>
	void AddParticle(std::string name, AddStatus status);

private:
	ParticleManager();
};

template<class T>
inline void ParticleManager::AddParticle(std::string name, AddStatus status)
{
	particles_.emplace_back(std::move(std::make_unique<T>()));
	particles_.back()->Add(status.addNum, status.time, status.pos, status.addVec, status.scale);
}
