#include "ParticleManager.h"
#include "PipelineManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Update()
{
	for (auto& particle : particles_)
	{
		particle->Update();
	}
}

void ParticleManager::PreDraw()
{
	
}

void ParticleManager::Draw()
{
	for (auto& particle : particles_)
	{
		particle->Draw();
	}
}
