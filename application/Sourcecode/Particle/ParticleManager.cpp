#include "ParticleManager.h"
#include "PipelineManager.h"
#include "ParticleTest.h"
#include "ParticleHitAttack.h"
#include "ParticleEnemyDead.h"
#include "ParticleWallHit.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

ParticleManager::ParticleManager()
{
}

void ParticleManager::Update()
{
	std::list<std::unique_ptr<IParticle>>::iterator itr;
	for (itr = particles_.begin(); itr != particles_.end();)
	{
		//パーティクルの数が0になったら消す
		if ((*itr)->GetParticleNum() == 0) {
			itr = particles_.erase(itr);
			continue;
		}

		(*itr)->Update();
		itr++;
	}
}

void ParticleManager::Draw()
{
	std::string shaderName = "Particle";
	
	for (auto& particle : particles_)
	{
		PipelineManager::PreDraw(particle->GetShaderName(), POINTLIST);
		particle->Draw();
	}
}