#include "ParticleManager.h"
#include "PipelineManager.h"
#include "ParticleTest.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

ParticleManager::ParticleManager()
{
	particles_.emplace_back(std::move(std::make_unique<ParticleTest>()));
}

void ParticleManager::Update()
{
	for (auto& particle : particles_)
	{
		particle->Update();
	}
}

void ParticleManager::Draw()
{
	for (auto& particle : particles_)
	{
		particle->Draw();
	}
}
