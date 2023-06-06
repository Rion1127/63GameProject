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
	DirectXCommon::GetInstance()->GetCommandList()->
		SetPipelineState(PipelineManager::GetParticlePipeline(3)->gerPipelineState());
	DirectXCommon::GetInstance()->GetCommandList()->
		SetGraphicsRootSignature(PipelineManager::GetParticlePipeline(3)->GetRootSignature());

	// プリミティブ形状の設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // ポイントリスト
}
