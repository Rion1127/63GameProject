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

void ParticleManager::AllReset()
{
	std::list<std::shared_ptr<IEmitter>>::iterator itr;
	for (itr = emitters_.begin(); itr != emitters_.end();)
	{
		(*itr)->isActive = false;
		itr++;
	}
}

ParticleManager::ParticleManager()
{
}

void ParticleManager::Update()
{
	std::list<std::shared_ptr<IEmitter>>::iterator itr;
	for (itr = emitters_.begin(); itr != emitters_.end();)
	{
		//パーティクルの数が0になったら
		if ((*itr)->particle->GetParticleNum() == 0) {
			itr = emitters_.erase(itr);
			continue;
		}

		if ((*itr)->isActive) {
			//座標を更新
			(*itr)->popCoolTime_.AddTime(1);
			if ((*itr)->popCoolTime_.GetIsEnd()) {
				(*itr)->particle->Add();

				(*itr)->popCoolTime_.Reset();
			}
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
		PipelineManager::PreDraw(emitter->particle->GetShaderName(), POINTLIST, emitter->particle->GetPipelineState());
		emitter->particle->Draw();
	}
}

void ParticleManager::AddParticle(std::string name, std::shared_ptr<IEmitter> emitter)
{
	emitters_.emplace_back();
	emitters_.back() = emitter;
	emitters_.back()->particle->SetEmitter(emitter.get());
	emitters_.back()->particle->Add();
}
