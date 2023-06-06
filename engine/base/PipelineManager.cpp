#include "PipelineManager.h"

std::array<Pipeline, 4> PipelineManager::Object3dPipeline_;
std::array<Pipeline, 4> PipelineManager::SpritePipeline_;
std::array<ParticlePipeline, 4> PipelineManager::particlePipeline_;
std::array<Pipeline, 4> PipelineManager::toonPipeline_;

void PipelineManager::Ini() {
	for (int i = 0; i < 4; i++) {
		Object3dPipeline_.at(i).Object3dIni(i);
		SpritePipeline_.at(i).SpriteIni(i);
		particlePipeline_.at(i).Init(i);
		toonPipeline_.at(i).ToonIni(i);
	}
};