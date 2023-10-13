#include "PipelineManager.h"

std::map<std::string, std::unique_ptr<PipelineObject>> PipelineManager::pipelineObjects_;

void PipelineManager::Ini() {
	//モデル、スプライトのパイプライン初期化
	ObjShaderIni();
	//ポストエフェクトのパイプライン初期化
	PostEffectIni();
	//パーティクルのパイプライン初期化
	ParticleShaderIni();
}

void PipelineManager::ObjShaderIni()
{
	//オブジェクト3D
	AddPipeline("Object3D");
	GetPipelineObjects("Object3D")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Object3D")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Object3D")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Object3D")->Setshader("BasicVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Object3D")->Setshader("BasicPS.hlsl", ShaderType::PS);

	GetPipelineObjects("Object3D")->AddrootParams(4);

	Create("Object3D", BACK, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ALL, MODE_WRAP);
	//スプライト
	AddPipeline("Sprite");
	GetPipelineObjects("Sprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Sprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Sprite")->Setshader("SpriteVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Sprite")->Setshader("SpritePS.hlsl", ShaderType::PS);

	GetPipelineObjects("Sprite")->AddrootParams(3);

	Create("Sprite", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_WRAP);

	//トゥーンオブジェクト3D
	AddPipeline("Toon");
	GetPipelineObjects("Toon")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Toon")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Toon")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Toon")->Setshader("ToonVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Toon")->Setshader("ToonPS.hlsl", ShaderType::PS);

	GetPipelineObjects("Toon")->AddrootParams(4);

	Create("Toon", BACK, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ALL, MODE_WRAP);

	//オブジェクト3D
	AddPipeline("assimp");
	GetPipelineObjects("assimp")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("assimp")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("assimp")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	GetPipelineObjects("assimp")->AddInputLayout("BONEINDICES", DXGI_FORMAT_R32G32B32A32_UINT);
	GetPipelineObjects("assimp")->AddInputLayout("BONEWEIGHTS", DXGI_FORMAT_R32G32B32A32_FLOAT);

	GetPipelineObjects("assimp")->Setshader("assimpVS.hlsl", ShaderType::VS);
	GetPipelineObjects("assimp")->Setshader("assimpPS.hlsl", ShaderType::PS);

	GetPipelineObjects("assimp")->AddrootParams(5);

	Create("assimp", BACK, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ALL, MODE_WRAP);

	//天球用
	AddPipeline("SkySphere");
	GetPipelineObjects("SkySphere")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("SkySphere")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("SkySphere")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("SkySphere")->Setshader("SkySphereVS.hlsl", ShaderType::VS);
	GetPipelineObjects("SkySphere")->Setshader("SkySpherePS.hlsl", ShaderType::PS);

	GetPipelineObjects("SkySphere")->AddrootParams(4);

	Create("SkySphere", BACK, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ALL, MODE_WRAP);
}
void PipelineManager::PostEffectIni()
{
#pragma region ポストエフェクト
	//ポストエフェクト
	AddPipeline("PostEffect");
	GetPipelineObjects("PostEffect")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("PostEffect")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("PostEffect")->Setshader("PostEffectVS.hlsl", ShaderType::VS);
	GetPipelineObjects("PostEffect")->Setshader("PostEffectPS.hlsl", ShaderType::PS);

	GetPipelineObjects("PostEffect")->AddrootParams(2);

	Create("PostEffect", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region マルチテクスチャ
	AddPipeline("MultiTexture");
	GetPipelineObjects("MultiTexture")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("MultiTexture")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("MultiTexture")->Setshader("MultiTextureVS.hlsl", ShaderType::VS);
	GetPipelineObjects("MultiTexture")->Setshader("MultiTexturePS.hlsl", ShaderType::PS);

	GetPipelineObjects("MultiTexture")->AddrootParamsMultiTexture(2, 1);

	Create("MultiTexture", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region ブルーム
	AddPipeline("Bloom");
	GetPipelineObjects("Bloom")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Bloom")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Bloom")->Setshader("BloomVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Bloom")->Setshader("BloomPS.hlsl", ShaderType::PS);

	GetPipelineObjects("Bloom")->AddrootParamsMultiTexture(2, 1);

	Create("Bloom", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region ルミライト
	AddPipeline("HighLumi");
	GetPipelineObjects("HighLumi")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("HighLumi")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("HighLumi")->Setshader("HighLumiVS.hlsl", ShaderType::VS);
	GetPipelineObjects("HighLumi")->Setshader("HighLumiPS.hlsl", ShaderType::PS);

	GetPipelineObjects("HighLumi")->AddrootParams(2);

	Create("HighLumi", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region ガウシアンブラー
	AddPipeline("Gaussian");
	GetPipelineObjects("Gaussian")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Gaussian")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Gaussian")->Setshader("GaussianVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Gaussian")->Setshader("GaussianPS.hlsl", ShaderType::PS);

	GetPipelineObjects("Gaussian")->AddrootParams(2);

	Create("Gaussian", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region ノイズ
	AddPipeline("Noise");
	GetPipelineObjects("Noise")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Noise")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Noise")->Setshader("NoiseVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Noise")->Setshader("NoisePS.hlsl", ShaderType::PS);

	GetPipelineObjects("Noise")->AddrootParams(2);

	Create("Noise", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region ラインブラー
	//ポストエフェクト
	AddPipeline("LineBlur");
	GetPipelineObjects("LineBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("LineBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("LineBlur")->Setshader("LineBlurVS.hlsl", ShaderType::VS);
	GetPipelineObjects("LineBlur")->Setshader("LineBlurPS.hlsl", ShaderType::PS);

	GetPipelineObjects("LineBlur")->AddrootParams(2);

	Create("LineBlur", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region クロスフィルター
	AddPipeline("CrossFilter");
	GetPipelineObjects("CrossFilter")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("CrossFilter")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("CrossFilter")->Setshader("CrossFilterVS.hlsl", ShaderType::VS);
	GetPipelineObjects("CrossFilter")->Setshader("CrossFilterPS.hlsl", ShaderType::PS);

	GetPipelineObjects("CrossFilter")->AddrootParamsMultiTexture(3, 1);

	Create("CrossFilter", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region CG4_評価課題２
	AddPipeline("MultiRenderTexture");
	GetPipelineObjects("MultiRenderTexture")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("MultiRenderTexture")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("MultiRenderTexture")->Setshader("MultiRenderTextureVS.hlsl", ShaderType::VS);
	GetPipelineObjects("MultiRenderTexture")->Setshader("MultiRenderTexturePS.hlsl", ShaderType::PS);

	GetPipelineObjects("MultiRenderTexture")->AddrootParamsMultiTexture(2, 1);

	Create("MultiRenderTexture", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region ラインブラー
	//ポストエフェクト
	AddPipeline("RadialBlur");
	GetPipelineObjects("RadialBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("RadialBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("RadialBlur")->Setshader("RadialBlurVS.hlsl", ShaderType::VS);
	GetPipelineObjects("RadialBlur")->Setshader("RadialBlurPS.hlsl", ShaderType::PS);

	GetPipelineObjects("RadialBlur")->AddrootParams(2);

	Create("RadialBlur", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion
}

void PipelineManager::ParticleShaderIni()
{
	AddPipeline("Particle");
	GetPipelineObjects("Particle")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Particle")->AddInputLayout("SCALE", DXGI_FORMAT_R32_FLOAT, 0);
	GetPipelineObjects("Particle")->AddInputLayout("ROTATION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Particle")->AddInputLayout("ANCORPOINT", DXGI_FORMAT_R32G32_FLOAT);
	GetPipelineObjects("Particle")->AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);

	GetPipelineObjects("Particle")->Setshader("ParticleGS.hlsl", ShaderType::GS);
	GetPipelineObjects("Particle")->Setshader("ParticleVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Particle")->Setshader("ParticlePS.hlsl", ShaderType::PS);

	GetPipelineObjects("Particle")->AddrootParams(2);

	Create("Particle", NONE, TOPOLOGY_POINT, DEPTH_WRITE_MASK_ZERO, MODE_WRAP);

	AddPipeline("Particle_Depth_False");
	GetPipelineObjects("Particle_Depth_False")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Particle_Depth_False")->AddInputLayout("SCALE", DXGI_FORMAT_R32_FLOAT, 0);
	GetPipelineObjects("Particle_Depth_False")->AddInputLayout("ROTATION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Particle_Depth_False")->AddInputLayout("ANCORPOINT", DXGI_FORMAT_R32G32_FLOAT);
	GetPipelineObjects("Particle_Depth_False")->AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);

	GetPipelineObjects("Particle_Depth_False")->Setshader("ParticleGS.hlsl", ShaderType::GS);
	GetPipelineObjects("Particle_Depth_False")->Setshader("ParticleVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Particle_Depth_False")->Setshader("ParticlePS.hlsl", ShaderType::PS);

	GetPipelineObjects("Particle_Depth_False")->AddrootParams(2);

	Create("Particle_Depth_False", NONE, TOPOLOGY_POINT, DEPTH_ENABLE_FALSE, MODE_WRAP);

	AddPipeline("Smoke");
	GetPipelineObjects("Smoke")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Smoke")->AddInputLayout("SCALE", DXGI_FORMAT_R32_FLOAT, 0);
	GetPipelineObjects("Smoke")->AddInputLayout("ROTATION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Smoke")->AddInputLayout("ANCORPOINT", DXGI_FORMAT_R32G32_FLOAT);
	GetPipelineObjects("Smoke")->AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);

	GetPipelineObjects("Smoke")->Setshader("SmokeGS.hlsl", ShaderType::GS);
	GetPipelineObjects("Smoke")->Setshader("SmokeVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Smoke")->Setshader("SmokePS.hlsl", ShaderType::PS);

	GetPipelineObjects("Smoke")->AddrootParams(2);

	Create("Smoke", NONE, TOPOLOGY_POINT, DEPTH_ENABLE_FALSE, MODE_WRAP);

	AddPipeline("Fire");
	GetPipelineObjects("Fire")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Fire")->AddInputLayout("SCALE", DXGI_FORMAT_R32_FLOAT, 0);
	GetPipelineObjects("Fire")->AddInputLayout("ROTATION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Fire")->AddInputLayout("ANCORPOINT", DXGI_FORMAT_R32G32_FLOAT);
	GetPipelineObjects("Fire")->AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);

	GetPipelineObjects("Fire")->Setshader("FireGS.hlsl", ShaderType::GS);
	GetPipelineObjects("Fire")->Setshader("FireVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Fire")->Setshader("FirePS.hlsl", ShaderType::PS);

	GetPipelineObjects("Fire")->AddrootParams(2);

	Create("Fire", NONE, TOPOLOGY_POINT, DEPTH_WRITE_MASK_ZERO, MODE_WRAP);

	AddPipeline("SwordTrail");
	GetPipelineObjects("SwordTrail")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("SwordTrail")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("SwordTrail")->Setshader("SwordTrailVS.hlsl", ShaderType::VS);
	GetPipelineObjects("SwordTrail")->Setshader("SwordTrailPS.hlsl", ShaderType::PS);

	GetPipelineObjects("SwordTrail")->AddrootParams(3);

	Create("SwordTrail", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_WRAP);
}

void PipelineManager::Create(
	const std::string& pipelinename, const CULL_MODE& cullmode,
	const TOPOLOGY_TYPE& topologytype, const WRIGHT_MASK& depthWriteMasc,
	const TEXTURE_ADDRESS_MODE& uvMode)
{
	for (int32_t i = 0; i < 4; i++)
	{
		pipelineObjects_[pipelinename]->
			Create(BlendNum(i), cullmode, topologytype, depthWriteMasc, uvMode);
	}
}
void PipelineManager::AddPipeline(const std::string& pipelinename)
{
	std::unique_ptr<PipelineObject> obj = std::move(std::make_unique<PipelineObject>());
	obj->name_ = pipelinename;
	pipelineObjects_.insert(std::make_pair(pipelinename, std::move(obj)));
}
void PipelineManager::PreDraw(const std::string& pipelinename, const TopologyName& topologyName,
	const PipeLineState& state)
{
	auto& cmdList = *RDirectX::GetInstance()->GetCommandList();
	// パイプラインステートとルートシグネチャの設定コマンド
	if (state == PipeLineState::Add) {
		cmdList.SetPipelineState(
			PipelineManager::GetPipelineObjects(pipelinename)->GetPipelineStateAdd());
	}
	else if (state == PipeLineState::Alpha) {
		cmdList.SetPipelineState(
			PipelineManager::GetPipelineObjects(pipelinename)->GetPipelineStateAlpha());
	}
	else if (state == PipeLineState::Nega) {
		cmdList.SetPipelineState(
			PipelineManager::GetPipelineObjects(pipelinename)->GetPipelineStateNega());
	}
	else if (state == PipeLineState::Sub) {
		cmdList.SetPipelineState(
			PipelineManager::GetPipelineObjects(pipelinename)->GetPipelineStateSub());
	}

	cmdList.SetGraphicsRootSignature(
		PipelineManager::GetPipelineObjects(pipelinename)->GetRootSignature());

	// プリミティブ形状の設定コマンド
	cmdList.IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)topologyName); // 三角形リスト
}