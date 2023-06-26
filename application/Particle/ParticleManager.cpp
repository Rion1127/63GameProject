#include "ParticleManager.h"
#include "PipelineManager.h"

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	//パイプラインに設定した内容で描画を始める
	RDirectX::GetInstance()->GetCommandList()->
		SetPipelineState(PipelineManager::GetPipelineObjects("Particle")->GetPipelineStateAlpha());
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootSignature(PipelineManager::GetPipelineObjects("Particle")->GetRootSignature());

	// プリミティブ形状の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // ポイントリスト
}
