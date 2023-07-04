#include "ParticleManager.h"
#include "PipelineManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::PreDraw()
{
	PipelineManager::PreDraw("particle",POINTLIST);
}
