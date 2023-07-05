#pragma once
#include "IParticle.h"
#include <unordered_map>
class ParticleManager
{
private:
	std::unordered_map<std::string,std::unique_ptr<IParticle>> particles_;
public:
	static ParticleManager* GetInstance();

	void Update();

	void Draw();

	void AddTestParticle(std::string particleName, size_t addNum, size_t time, Vector3 pos, Vector3 addVec, float scale);

private:
	ParticleManager();
};

