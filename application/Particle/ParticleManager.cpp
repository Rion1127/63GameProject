#include "ParticleManager.h"
#include "PipelineManager.h"
#include "ParticleTest.h"
#include "ParticleHitAttack.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

ParticleManager::ParticleManager()
{
	particles_.insert(std::make_pair("Test", std::move(std::make_unique<ParticleTest>())));
	particles_.insert(std::make_pair("HitAttack",std::move(std::make_unique<ParticleHitAttack>())));
}

void ParticleManager::Update()
{
	for (auto& particle : particles_)
	{
		particle.second->Update();
	}
}

void ParticleManager::Draw()
{
	for (auto& particle : particles_)
	{
		particle.second->Draw();
	}
}

void ParticleManager::AddParticle(std::string particleName, int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale)
{
	particles_.find(particleName)->second->Add(addNum, time, pos, addVec, scale);
}
