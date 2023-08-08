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
	std::list<std::shared_ptr<Emitter>>::iterator itr;
	for (itr = emitters_.begin(); itr != emitters_.end();)
	{
		//パーティクルの数が0になったら消す
		if ((*itr)->particle->GetParticleNum() == 0) {
			itr = emitters_.erase(itr);
			continue;
		}

		(*itr)->particle->Update();
		itr++;
	}
}

void ParticleManager::Draw()
{
	std::string shaderName = "Particle";
	
	for (auto& emitter : emitters_)
	{
		PipelineManager::PreDraw(emitter->particle->GetShaderName(), POINTLIST);
		emitter->particle->Draw();
	}
}

void ParticleManager::AddParticle(std::string name, std::shared_ptr<Emitter> emitter)
{
	emitters_.emplace_back();
	emitters_.back() = emitter;
	emitters_.back()->particle->Add(emitter->addNum, emitter->time, emitter->pos, emitter->addVec, emitter->scale);
}
