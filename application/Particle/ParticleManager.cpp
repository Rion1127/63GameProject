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

void ParticleManager::AddTestParticle(std::string particleName, size_t addNum, size_t time, Vector3 pos, Vector3 addVec, float scale)
{
	particles_.find(particleName)->second->Add(addNum, time, pos, addVec, scale);
}
