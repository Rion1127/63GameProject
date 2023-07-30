#include "ParticleManager.h"
#include "PipelineManager.h"
#include "ParticleTest.h"
#include "ParticleHitAttack.h"
#include "ParticleEnemyDead.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

ParticleManager::ParticleManager()
{
	particles_.insert(std::make_pair("Test", std::move(std::make_unique<ParticleTest>())));
	particles_.insert(std::make_pair("HitAttack", std::move(std::make_unique<ParticleHitAttack>())));
	particles_.insert(std::make_pair("EnemyDead",std::move(std::make_unique<ParticleEnemyDead>())));
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
	std::string shaderName = "Particle";
	for (auto& particle : particles_)
	{
		PipelineManager::PreDraw(particle.second->GetShaderName(), POINTLIST);

		particle.second->Draw();
	}
}

void ParticleManager::AddParticle(std::string particleName, int32_t addNum, int32_t time, Vector3 pos, Vector3 addVec, float scale)
{
	particles_.find(particleName)->second->Add(addNum, time, pos, addVec, scale);
}
