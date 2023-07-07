#include "IPostEffect.h"
#include "WinAPI.h"
#include <d3dcompiler.h>

#include "DirectX.h"
#include "PipelineManager.h"

const float IPostEffect::clearColor_[4] = { 0.25f,0.5f,0.1f,0.0f };
const uint32_t IPostEffect::vertNum_ = 4;

IPostEffect::IPostEffect()
{
	//頂点バッファ生成
	CreateVertBuff();
	//ibView生成
	CreateibView();
	//テクスチャ生成
	CreateTexBuff();
	//SRV生成
	CreateSRV();
	//RTV作成
	CreateRTV();
	//深度バッファ生成
	CreateDepthBuff();
	//DSV作成
	CreateDSV();
}

void IPostEffect::PUpdate()
{
	TransferBuff();
}

void IPostEffect::Draw(std::string pipelineName)
{
	// パイプラインステートとルートシグネチャの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetPipelineObjects(pipelineName)->GetPipelineStateAlpha());

	RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetPipelineObjects(pipelineName)->GetRootSignature());

	// プリミティブ形状の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	//SRVヒープの設定コマンド
	std::vector<ID3D12DescriptorHeap*> heaps = { descHeapSRV_.Get() };
	RDirectX::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, heaps.data());
	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
	srvGpuHandle = descHeapSRV_.Get()->GetGPUDescriptorHandleForHeapStart();
	//SRVヒープの先頭にあるSRVをルートパラメータ0番に設定
	RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, srvGpuHandle);

	//定数バッファビュー(CBV)の設定コマンド
	//定数バッファをセットする
	SendToShader();
	//SetBuff(1, constBuff_.Get());
	// 頂点バッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//インデックスバッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView_);

	//描画コマンド
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void IPostEffect::PreDrawScene()
{
	ID3D12GraphicsCommandList& cmdList = *RDirectX::GetInstance()->GetCommandList();

	CD3DX12_RESOURCE_BARRIER bariier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	RDirectX::GetInstance()->GetCommandList()
		->ResourceBarrier(1,
			&bariier);

	//レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		descHeapRTV_->GetCPUDescriptorHandleForHeapStart();
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList.OMSetRenderTargets(1, &rtvH, false, &dsvH);
	//ビューポートの設定
	CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f,
		WinAPI::GetWindowSize().x, WinAPI::GetWindowSize().y);
	cmdList.RSSetViewports(1, &viewPort);
	//シザリング矩形の設定
	CD3DX12_RECT rect = CD3DX12_RECT(0, 0,
		(LONG)WinAPI::GetWindowSize().x, (LONG)WinAPI::GetWindowSize().y);
	cmdList.RSSetScissorRects(1, &rect);

	//全画面クリア
	cmdList.ClearRenderTargetView(rtvH, clearColor_, 0, nullptr);
	//深度バッファのクリア
	cmdList.ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void IPostEffect::PostDrawScene()
{
	//リソースバリアを変更（描画可能→シェーダーリソース）
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff_.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	RDirectX::GetInstance()->GetCommandList()->
		ResourceBarrier(1, &barrier);
}

void IPostEffect::CreateVertBuff()
{
	HRESULT result;

	//頂点バッファ生成
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resDesc =
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUV) * vertNum_);
	result = RDirectX::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));
	//頂点データ
	VertexPosUV vertices[vertNum_] = {
		{{-1.f,-1.f,0.0f},{0.f,1.f}},//左下
		{{-1.f,+1.f,0.0f},{0.f,0.f}},//左上
		{{+1.f,-1.f,0.0f},{1.f,1.f}},//右下
		{{+1.f,+1.f,0.0f},{1.f,0.f}},//右上
	};
	//頂点バッファへのデータ転送
	VertexPosUV* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff_->Unmap(0, nullptr);
	}

	//頂点バッファビューの設定
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(VertexPosUV) * 4;
	vbView_.StrideInBytes = sizeof(VertexPosUV);
}

void IPostEffect::CreateibView()
{
	HRESULT result;
	indices_.push_back(0);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(3);

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());

	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (uint32_t i = 0; i < indices_.size(); i++) {
		indexMap[i] = indices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	indexBuff_->Unmap(0, nullptr);
	//インデックスバッファビューの作成
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
}

void IPostEffect::CreateTexBuff()
{
	HRESULT result;

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		(UINT64)WinAPI::GetWindowSize().x,
		(UINT)WinAPI::GetWindowSize().y,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_HEAP_PROPERTIES prop =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0);
	CD3DX12_CLEAR_VALUE clear_Value =
		CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor_);
	//テクスチャバッファの生成
	result =
		RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&texResDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clear_Value,
			IID_PPV_ARGS(&texBuff_));
	assert(SUCCEEDED(result));

	//画素数
	const size_t pixelCount = (size_t)WinAPI::GetWindowSize().x * (size_t)WinAPI::GetWindowSize().y;
	//画像一行分のデータサイズ
	const auto rowPitch = sizeof(UINT) * WinAPI::GetWindowSize().x;
	//画像全体のデータサイズ
	const auto depthPitch = rowPitch * WinAPI::GetWindowSize().y;
	//画像イメージ
	std::vector<UINT> img;
	img.resize(pixelCount);
	for (size_t i = 0; i < pixelCount; i++)
	{
		img[i] = 0xff0000ff;
	}
	//テクスチャバッファにデータ転送
	result = texBuff_->WriteToSubresource(0, nullptr,
		img.data(), (UINT)rowPitch, (UINT)depthPitch);
	assert(SUCCEEDED(result));
}

void IPostEffect::CreateSRV()
{
	HRESULT result;
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV用デスクリプタヒープを生成
	result = RDirectX::GetInstance()->GetDevice()->
		CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV_));
	assert(SUCCEEDED(result));
	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	//デスクリプタヒープにSRV作成
	RDirectX::GetInstance()->GetDevice()->
		CreateShaderResourceView(texBuff_.Get(),
			&srvDesc,
			descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
		);
}

void IPostEffect::CreateRTV()
{
	HRESULT result;
	//RTV用デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV用デスクリプタヒープを生成
	result = RDirectX::GetInstance()->GetDevice()
		->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV_));
	assert(SUCCEEDED(result));
	//レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//デスクリプタヒープにRTV作成
	RDirectX::GetInstance()->GetDevice()
		->CreateRenderTargetView(texBuff_.Get(),
			&renderTargetViewDesc,
			descHeapRTV_->GetCPUDescriptorHandleForHeapStart()
		);
}

void IPostEffect::CreateDepthBuff()
{
	HRESULT result;
	CD3DX12_RESOURCE_DESC depthResDesc =
		//深度バッファリソース設定
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			(UINT64)WinAPI::GetWindowSize().x,
			(UINT)WinAPI::GetWindowSize().y,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);
	//深度バッファの生成
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	result = RDirectX::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&depthResDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&depthBuff_));
	assert(SUCCEEDED(result));
}

void IPostEffect::CreateDSV()
{
	HRESULT result;
	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを作成
	result = RDirectX::GetInstance()->GetDevice()->
		CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV_));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	RDirectX::GetInstance()->GetDevice()->
		CreateDepthStencilView(depthBuff_.Get(),
			&dsvDesc,
			descHeapDSV_->GetCPUDescriptorHandleForHeapStart());
}

void IPostEffect::SetBuff(uint32_t index, ID3D12Resource* constBuff) {
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(index, constBuff->GetGPUVirtualAddress());
}