#include "ParticleManager.h"
#include "PipelineManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::PreDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	//�p�C�v���C���ɐݒ肵�����e�ŕ`����n�߂�
	RDirectX::GetInstance()->GetCommandList()->
		SetPipelineState(PipelineManager::GetPipelineObjects("Particle")->GetPipelineStateAlpha());
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootSignature(PipelineManager::GetPipelineObjects("Particle")->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // �|�C���g���X�g
}
