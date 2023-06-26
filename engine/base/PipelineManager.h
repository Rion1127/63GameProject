#pragma once
#include "Pipeline.h"
#include <array>
#include <string>
#include <map>

enum TopologyName {
	POINTLIST = 1,
	LINELIST = 2,
	LINESTRIP = 3,
	TRIANGLELIST = 4,
	TRIANGLESTRIP = 5,
};

class PipelineManager {
private:
	static std::map<std::string, std::unique_ptr<PipelineObject>> pipelineObjects_;
public:
	static void Ini();

	static void Create(
		const std::string& pipelinename, CULL_MODE cullmode,
		TOPOLOGY_TYPE topologytype, WRIGHT_MASK depthWriteMasc,
		TEXTURE_ADDRESS_MODE uvMode);
	static void AddPipeline(const std::string& pipelinename);

	static void PreDraw(std::string pipelinename, TopologyName topologyName);
private:
	static void ObjShaderIni();
	static void PostEffectIni();
public:
	//ÉQÉbÉ^Å[
	static PipelineObject* GetPipelineObjects(const std::string& name) { return pipelineObjects_[name].get(); }
};