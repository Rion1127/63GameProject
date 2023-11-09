#pragma once
#include "Pipeline.h"
#include <array>
#include <string>
#include <map>

/**
 * @file PipelineManager.h
 * @brief パイプラインを生成、管理しているマネージャークラス
 */

enum TopologyName {
	POINTLIST = 1,
	LINELIST = 2,
	LINESTRIP = 3,
	TRIANGLELIST = 4,
	TRIANGLESTRIP = 5,
};
enum class PipeLineState {
	Add,
	Alpha,
	Nega,
	Sub
};

class PipelineManager {
private:
	static std::map<std::string, std::unique_ptr<PipelineObject>> pipelineObjects_;
public:
	static void Ini();

	static void Create(
		const std::string& pipelinename, const CULL_MODE& cullmode,
		const TOPOLOGY_TYPE& topologytype, const WRIGHT_MASK& depthWriteMasc,
		const TEXTURE_ADDRESS_MODE& uvMode);
	static void AddPipeline(const std::string& pipelinename);

	static void PreDraw(const std::string& pipelinename, const TopologyName& topologyName,
		const PipeLineState& state = PipeLineState::Alpha);
private:
	static void ObjShaderIni();
	static void PostEffectIni();
	static void ParticleShaderIni();
public:
	//ゲッター
	static PipelineObject* GetPipelineObjects(const std::string& name) { return pipelineObjects_[name].get(); }
};