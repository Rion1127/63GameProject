#include "ParticleManager.h"
#include "PipelineManager.h"
#include "ParticleTest.h"
#include "ParticleHitAttack.h"
#include "ParticleEnemyDead.h"
#include "ParticleWallHit.h"
#include "ParticleFire.h"
#include "ParticleFinishBreak.h"

/**
 * @file ParticleManager.cpp
 * @brief パーティクルを管理するクラス
 */

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

void ParticleManager::InitParticle(const std::string& name)
{
	auto& emitter = emitters_.back();
	if (name == "WallHit") {
		emitter->particle = std::make_unique<ParticleWallHit>();
		emitter->addNum = 1;
		emitter->time = 60;
		emitter->scale = 1.0f;
	}
	else if (name == "HitAttack") {
		emitter->particle = std::make_unique<ParticleHitAttack>();
		emitter->addNum = 3;
		emitter->time = 40;
		emitter->scale = 1.0f;
		Vector3 addVec = { 0.05f,0.05f,0.05f };
		emitter->addVec = addVec;
	}
	else if (name == "EnemyDead") {
		emitter->particle = std::make_unique<ParticleEnemyDead>();
		emitter->addNum = 32;
		emitter->time = 80;
		emitter->addVec = { 0.8f,0.8f, 0.8f };
		emitter->scale = 1.0f;
	}
	else if (name == "fireBall") {
		emitter->particle = std::make_unique<ParticleFire>();
		emitter->addVec = { 0.2f,0.2f, 0.2f, };
		emitter->addNum = 5;
		emitter->isActive = true;
		emitter->time = 20;
		emitter->scale = 1.f;
	}
	else if (name == "fireCharge") {
		emitter->particle = std::make_unique<ParticleFire>();
		emitter->addVec = { 0.25f,0.3f, 0.25f, };
		emitter->addNum = 4;
		emitter->time = 30;
		emitter->scale = 0.3f;
	}
	else if (name == "fireCircle") {
		emitter->particle = std::make_unique<ParticleFireCircle>();
		emitter->addVec = { 0.0f,0.0f, 0.0f, };
		emitter->addNum = 2;
		emitter->time = 180;
		emitter->scale = 1.0f;
	}
	else if (name == "groundStump") {
		emitter->particle = std::make_unique<ParticleGroundStump>();
		emitter->addVec = { 0.0f,0.0f, 0.0f, };
		emitter->addNum = 6;
		emitter->time = 180;
		emitter->scale = 13.0f;

		particleDebri_.emplace_back(std::make_unique<ParticleDebri>(objDataPool_.GetObj("debri"), 20, emitter->pos,120.f));
	}
	else if (name == "wave")
	{
		emitter->particle = std::make_unique<ParticleFinishBreak>();
		emitter->addNum = 1;
		emitter->popCoolTime_.SetLimitTime(7);
		emitter->time = 25;
		emitter->scale = 6.f;
	}
}

void ParticleManager::Update()
{
	HitStopUpdate();

	std::list<std::shared_ptr<IEmitter>>::iterator itr;
	for (itr = emitters_.begin(); itr != emitters_.end();)
	{
		if (GetIsHitStopping() == false) {
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
		}
		(*itr)->particle->TransferBuff();
		itr++;
	}

	for (int32_t i = 0; i < particleDebri_.size();i++ )
	{
		particleDebri_[i]->Update();
		if (particleDebri_[i]->isGetEnd())
		{
			particleDebri_.erase(particleDebri_.begin() + i);
		}
	}
}

void ParticleManager::Draw()
{
	for (int32_t i = 0; i < particleDebri_.size(); i++)
	{
		PipelineManager::PreDraw("Object3D", TRIANGLELIST);
		particleDebri_[i]->Draw();
	}
	std::string shaderName = "Particle";
	for (auto& emitter : emitters_)
	{
		PipelineManager::PreDraw(emitter->particle->GetShaderName(), POINTLIST, emitter->particle->GetPipelineState());
		emitter->particle->Draw();
	}
}

void ParticleManager::AddParticle(const std::string& name, const std::shared_ptr<IEmitter>& emitter)
{
	emitters_.emplace_back();
	emitters_.back() = emitter;
	InitParticle(name);
	emitters_.back()->particle->SetEmitter(emitter.get());
	emitters_.back()->particle->Add();
}
