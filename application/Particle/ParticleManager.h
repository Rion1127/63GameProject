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

	void AddParticle(std::string particleName, int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale);

private:
	ParticleManager();
};

