#include "PipelineManager.h"

std::map<std::string, std::unique_ptr<PipelineObject>> PipelineManager::pipelineObjects_;

void PipelineManager::Ini() {
	//���f���A�X�v���C�g�̃p�C�v���C��������
	ObjShaderIni();
	//�|�X�g�G�t�F�N�g�̃p�C�v���C��������
	PostEffectIni();
	//�p�[�e�B�N���̃p�C�v���C��������
	ParticleShaderIni();
}

void PipelineManager::ObjShaderIni()
{
	//�I�u�W�F�N�g3D
	AddPipeline("Object3D");
	GetPipelineObjects("Object3D")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Object3D")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Object3D")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Object3D")->Setshader("BasicVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Object3D")->Setshader("BasicPS.hlsl", ShaderType::PS);

	GetPipelineObjects("Object3D")->AddrootParams(4);

	Create("Object3D", BACK, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ALL, MODE_WRAP);
	//�X�v���C�g
	AddPipeline("Sprite");
	GetPipelineObjects("Sprite")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Sprite")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Sprite")->Setshader("SpriteVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Sprite")->Setshader("SpritePS.hlsl", ShaderType::PS);

	GetPipelineObjects("Sprite")->AddrootParams(3);

	Create("Sprite", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_WRAP);

	//�g�D�[���I�u�W�F�N�g3D
	AddPipeline("Toon");
	GetPipelineObjects("Toon")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Toon")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Toon")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Toon")->Setshader("ToonVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Toon")->Setshader("ToonPS.hlsl", ShaderType::PS);

	GetPipelineObjects("Toon")->AddrootParams(4);

	Create("Toon", BACK, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ALL, MODE_WRAP);

	//�I�u�W�F�N�g3D
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
}
void PipelineManager::PostEffectIni()
{
#pragma region �|�X�g�G�t�F�N�g
	//�|�X�g�G�t�F�N�g
	AddPipeline("PostEffect");
	GetPipelineObjects("PostEffect")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("PostEffect")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("PostEffect")->Setshader("PostEffectVS.hlsl", ShaderType::VS);
	GetPipelineObjects("PostEffect")->Setshader("PostEffectPS.hlsl", ShaderType::PS);

	GetPipelineObjects("PostEffect")->AddrootParams(2);

	Create("PostEffect", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region �}���`�e�N�X�`��
	AddPipeline("MultiTexture");
	GetPipelineObjects("MultiTexture")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("MultiTexture")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("MultiTexture")->Setshader("MultiTextureVS.hlsl", ShaderType::VS);
	GetPipelineObjects("MultiTexture")->Setshader("MultiTexturePS.hlsl", ShaderType::PS);

	GetPipelineObjects("MultiTexture")->AddrootParamsMultiTexture(2, 1);

	Create("MultiTexture", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region �u���[��
	AddPipeline("Bloom");
	GetPipelineObjects("Bloom")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Bloom")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Bloom")->Setshader("BloomVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Bloom")->Setshader("BloomPS.hlsl", ShaderType::PS);

	GetPipelineObjects("Bloom")->AddrootParamsMultiTexture(2, 1);

	Create("Bloom",NONE, TOPOLOGY_TRIANGLE,DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region ���~���C�g
	AddPipeline("HighLumi");
	GetPipelineObjects("HighLumi")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("HighLumi")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("HighLumi")->Setshader("HighLumiVS.hlsl", ShaderType::VS);
	GetPipelineObjects("HighLumi")->Setshader("HighLumiPS.hlsl", ShaderType::PS);

	GetPipelineObjects("HighLumi")->AddrootParams(2);

	Create("HighLumi", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region �K�E�V�A���u���[
	AddPipeline("Gaussian");
	GetPipelineObjects("Gaussian")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Gaussian")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Gaussian")->Setshader("GaussianVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Gaussian")->Setshader("GaussianPS.hlsl", ShaderType::PS);

	GetPipelineObjects("Gaussian")->AddrootParams(2);

	Create("Gaussian", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region �m�C�Y
	AddPipeline("Noise");
	GetPipelineObjects("Noise")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("Noise")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("Noise")->Setshader("NoiseVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Noise")->Setshader("NoisePS.hlsl", ShaderType::PS);

	GetPipelineObjects("Noise")->AddrootParams(2);

	Create("Noise", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region ���C���u���[
	//�|�X�g�G�t�F�N�g
	AddPipeline("LineBlur");
	GetPipelineObjects("LineBlur")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("LineBlur")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("LineBlur")->Setshader("LineBlurVS.hlsl", ShaderType::VS);
	GetPipelineObjects("LineBlur")->Setshader("LineBlurPS.hlsl", ShaderType::PS);

	GetPipelineObjects("LineBlur")->AddrootParams(2);

	Create("LineBlur", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion

#pragma region �N���X�t�B���^�[
	AddPipeline("CrossFilter");
	GetPipelineObjects("CrossFilter")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("CrossFilter")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("CrossFilter")->Setshader("CrossFilterVS.hlsl", ShaderType::VS);
	GetPipelineObjects("CrossFilter")->Setshader("CrossFilterPS.hlsl", ShaderType::PS);

	GetPipelineObjects("CrossFilter")->AddrootParamsMultiTexture(3, 1);

	Create("CrossFilter", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion 

#pragma region CG4_�]���ۑ�Q
	AddPipeline("MultiRenderTexture");
	GetPipelineObjects("MultiRenderTexture")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	GetPipelineObjects("MultiRenderTexture")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	GetPipelineObjects("MultiRenderTexture")->Setshader("MultiRenderTextureVS.hlsl", ShaderType::VS);
	GetPipelineObjects("MultiRenderTexture")->Setshader("MultiRenderTexturePS.hlsl", ShaderType::PS);

	GetPipelineObjects("MultiRenderTexture")->AddrootParamsMultiTexture(2, 1);

	Create("MultiRenderTexture", NONE, TOPOLOGY_TRIANGLE, DEPTH_WRITE_MASK_ZERO, MODE_BORDER);
#pragma endregion 

#pragma region ���C���u���[
	//�|�X�g�G�t�F�N�g
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
	GetPipelineObjects("Particle")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32_FLOAT, 0);
	GetPipelineObjects("Particle")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32B32_FLOAT, 1);
	GetPipelineObjects("Particle")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT, 2);
	GetPipelineObjects("Particle")->AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);

	GetPipelineObjects("Particle")->Setshader("ParticleGS.hlsl", ShaderType::GS);
	GetPipelineObjects("Particle")->Setshader("ParticleVS.hlsl", ShaderType::VS);
	GetPipelineObjects("Particle")->Setshader("ParticlePS.hlsl", ShaderType::PS);

	GetPipelineObjects("Particle")->AddrootParams(2);

	Create("Particle", NONE, TOPOLOGY_POINT, DEPTH_WRITE_MASK_ALL, MODE_WRAP);
}

void PipelineManager::Create(
	const std::string& pipelinename, CULL_MODE cullmode,
	TOPOLOGY_TYPE topologytype, WRIGHT_MASK depthWriteMasc,
	TEXTURE_ADDRESS_MODE uvMode)
{
	for (size_t i = 0; i < 4; i++)
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
void PipelineManager::PreDraw(std::string pipelinename, TopologyName topologyName)
{
	auto& cmdList = *RDirectX::GetInstance()->GetCommandList();
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	cmdList.SetPipelineState(
		PipelineManager::GetPipelineObjects(pipelinename)->GetPipelineStateAlpha());

	cmdList.SetGraphicsRootSignature(
		PipelineManager::GetPipelineObjects(pipelinename)->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList.IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)topologyName); // �O�p�`���X�g
}