#include "ParticleManager.h"
#include "PipelineManager.h"
#include "ParticleTest.h"
#include "ParticleHitAttack.h"
#include "ParticleEnemyDead.h"
#include "ParticleWallHit.h"

#include <time.h>
#include <imgui.h>
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
	clock_t start1 = clock();
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
	clock_t end1 = clock();
	double time1 = static_cast<double>(end1 - start1) / CLOCKS_PER_SEC * 1000.0;

	clock_t start2 = clock();
	std::list<std::unique_ptr<Emitter>>::iterator uitr;
	for (uitr = uniqueEmitters_.begin(); uitr != uniqueEmitters_.end();)
	{
		//パーティクルの数が0になったら消す
		if ((*uitr)->particle->GetParticleNum() == 0) {
			uitr = uniqueEmitters_.erase(uitr);
			continue;
		}

		(*uitr)->particle->Update();
		uitr++;
	}
	clock_t end2 = clock();
	double time2 = static_cast<double>(end2 - start2) / CLOCKS_PER_SEC * 1000.0;


	ImGui::Begin("debug2");
	float sharedAverage = (float)time1;
	ImGui::DragFloat("sharedUpdate", &sharedAverage);

	float uniqueAverage = (float)time2;
	ImGui::DragFloat("uniqueUpdate", &uniqueAverage);

	ImGui::End();
}

void ParticleManager::Draw()
{
	std::string shaderName = "Particle";
	
	for (auto& emitter : emitters_)
	{
		PipelineManager::PreDraw(emitter->particle->GetShaderName(), POINTLIST);
		emitter->particle->Draw();
	}

	for (auto& emitter : uniqueEmitters_)
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

void ParticleManager::AddParticle(std::string name, std::unique_ptr<Emitter> emitter)
{
	uniqueEmitters_.emplace_back(std::move(emitter));
	uniqueEmitters_.back()->particle->
		Add(uniqueEmitters_.back()->addNum,
			uniqueEmitters_.back()->time,
			uniqueEmitters_.back()->pos,
			uniqueEmitters_.back()->addVec,
			uniqueEmitters_.back()->scale);
}
