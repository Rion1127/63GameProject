#pragma once
#include "Pipeline.h"
#include <array>
class PipelineManager {
public:
	static void Ini();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="blendTipe"> = 0 BLEND_ALPHA</param>
	/// <param name="blendTipe"> = 1 BLEND_SUB</param>
	/// <param name="blendTipe"> = 2 BLEND_NEGA</param>
	/// <param name="blendTipe"> = 3 BLEND_NORMAL</param>
	/// <returns></returns>
	static Pipeline* GetObj3dPipeline(int blendTipe) {
		return &Object3dPipeline_.at(blendTipe);
	};
	static Pipeline* GetSpritePipeline(int blendTipe) {
		return &SpritePipeline_.at(blendTipe);
	};
	static ParticlePipeline* GetParticlePipeline(int blendTipe) {
		return &particlePipeline_.at(blendTipe);
	};
	static Pipeline* GetToonPipeline(int blendTipe) {
		return &toonPipeline_.at(blendTipe);
	};
private:
	static std::array<Pipeline, 4> Object3dPipeline_;
	static std::array<Pipeline, 4> SpritePipeline_;
	static std::array<ParticlePipeline, 4> particlePipeline_;
	static std::array<Pipeline, 4> toonPipeline_;

};