#include <cassert>
#include "Util.h"
#include "Pipeline.h"

const std::string kBaseDirectory = "application/Resources/shader/";

D3D12_STATIC_SAMPLER_DESC SetSAMPLER_DESC()
{
	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//全てリニア保管
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダからのみ使用可能

	return samplerDesc;
}

void SetBlend(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc, uint32_t blend)
{

	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA全てのチャンネルを描画
	//共通設定（アルファ値）
	blenddesc.BlendEnable = true;					//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う
	//加算合成
	if (blend == BlendNum::ADD)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//ソースの値を100%使う
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//デストの値を100%使う
	}
	//減算合成
	else if (blend == BlendNum::SUB)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//デストからソースを減算
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//ソースの値を100%使う
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//デストの値を100%使う
	}
	//色反転
	else if (blend == BlendNum::NEGA)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
		blenddesc.DestBlend = D3D12_BLEND_ZERO;			//使わない
	}
	//半透明合成
	else if (blend == BlendNum::ALPHA)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//加算
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//ソースのアルファ値
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値
	}

	pipelineDesc.BlendState.RenderTarget[0] = blenddesc;
	pipelineDesc.BlendState.RenderTarget[1] = blenddesc;
}

void PipelineObject::Create(BlendNum blendNum, CULL_MODE cullmode,
	TOPOLOGY_TYPE topologytype, WRIGHT_MASK depthWriteMasc,
	TEXTURE_ADDRESS_MODE uvMode)
{
	HRESULT result;

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// シェーダーの設定
	if (vsBlob_ != nullptr)
	{
		pipelineDesc.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
		pipelineDesc.VS.BytecodeLength = vsBlob_->GetBufferSize();
	}
	if (psBlob_ != nullptr)
	{
		pipelineDesc.PS.pShaderBytecode = psBlob_->GetBufferPointer();
		pipelineDesc.PS.BytecodeLength = psBlob_->GetBufferSize();
	}
	if (gsBlob_ != nullptr)
	{
		pipelineDesc.GS.pShaderBytecode = gsBlob_->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = gsBlob_->GetBufferSize();
	}

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE(uvMode);					//横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE(uvMode);					//縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE(uvMode);					//奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//全てリニア保管
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダからのみ使用可能

	// ルートシグネチャ
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	// ルートシグネチャの設定
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams_.data();	//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = (UINT)rootParams_.size();			//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ルートシグネチャのシリアライズ
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob_);
	assert(SUCCEEDED(result));
	result = RDirectX::GetInstance()->GetDevice()->
		CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature_.Get();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE(cullmode); // カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	pipelineDesc.DepthStencilState.DepthEnable = true;	//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK(depthWriteMasc);//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout_.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout_.size();

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE(topologytype);
	// その他の設定
	pipelineDesc.NumRenderTargets = 2; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// ブレンドステート
	SetBlend(pipelineDesc, blendNum);
	// パイプランステートの生成
	if (blendNum == BlendNum::ADD)
	{
		result = RDirectX::GetInstance()->GetDevice()->
			CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineStateAdd_));
	}
	else if (blendNum == BlendNum::NEGA)
	{
		result = RDirectX::GetInstance()->GetDevice()->
			CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineStateNega_));
	}
	else if (blendNum == BlendNum::SUB)
	{
		result = RDirectX::GetInstance()->GetDevice()->
			CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineStateSub_));
	}
	else if (blendNum == BlendNum::ALPHA)
	{
		result = RDirectX::GetInstance()->GetDevice()->
			CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineStateAlpha_));
	}
	assert(SUCCEEDED(result));
}

void PipelineObject::Setshader(std::string fileName, ShaderType shadertype)
{
	std::wstring allFileName = ToWideString(kBaseDirectory + fileName);
	// 頂点シェーダの読み込みとコンパイル
	if (shadertype == ShaderType::VS)
	{
		ShaderCompileFromFile(allFileName.c_str(), "main", "vs_5_0", &vsBlob_, errorBlob_.Get());
	}
	else if (shadertype == ShaderType::PS)
	{
		ShaderCompileFromFile(allFileName.c_str(), "main", "ps_5_0", &psBlob_, errorBlob_.Get());
	}
	else if (shadertype == ShaderType::GS)
	{
		ShaderCompileFromFile(allFileName.c_str(), "main", "gs_5_0", &gsBlob_, errorBlob_.Get());
	}
}

void PipelineObject::AddrootParams(int32_t addNum)
{
	rootParams_.clear();
	int32_t size = 0;
	//inputLayout + 1の数分rootParams_を作る
	for (int32_t i = 0; i < addNum; i++)
	{
		D3D12_ROOT_PARAMETER rootParams{};
		//配列の最初にテクスチャ
		if (rootParams_.size() == 0)
		{
			//デスクリプタレンジの設定
			D3D12_DESCRIPTOR_RANGE* descriptorRange =
				new D3D12_DESCRIPTOR_RANGE{};
			std::vector<CD3DX12_DESCRIPTOR_RANGE> cDescRangeSRVs;

			//デスクリプタレンジの設定
			descriptorRange->NumDescriptors = 1;					//一度の描画に好かうテクスチャが1枚なので1
			descriptorRange->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			descriptorRange->BaseShaderRegister = 0;				//テクスチャレジスタ0番
			descriptorRange->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			rootParams.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
			rootParams.DescriptorTable.pDescriptorRanges = descriptorRange;//デスクリプタレンジ
			rootParams.DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
			rootParams.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
		}
		else
		{
			//定数バッファ
			rootParams.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
			rootParams.Descriptor.ShaderRegister = size;					//定数バッファ番号
			rootParams.Descriptor.RegisterSpace = 0;						//デフォルト値
			rootParams.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
			size++;
		}

		rootParams_.emplace_back(rootParams);
	}
}

void PipelineObject::AddrootParamsMultiTexture(int32_t addTexnum, int32_t addNum)
{
	rootParams_.clear();
	int32_t size = 0;

	
	for (int32_t i = 0; i < addTexnum; i++)
	{
		D3D12_ROOT_PARAMETER rootParams{};
		//デスクリプタレンジの設定
		CD3DX12_DESCRIPTOR_RANGE* descriptorRange =
			new CD3DX12_DESCRIPTOR_RANGE{};

		descriptorRange->Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i);

		rootParams.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
		rootParams.DescriptorTable.pDescriptorRanges = descriptorRange;//デスクリプタレンジ
		rootParams.DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
		rootParams.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

		rootParams_.emplace_back(rootParams);
	}

	for (int32_t i = 0; i < addNum; i++)
	{
		D3D12_ROOT_PARAMETER rootParams{};
		//定数バッファ
		rootParams.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
		rootParams.Descriptor.ShaderRegister = size;					//定数バッファ番号
		rootParams.Descriptor.RegisterSpace = 0;						//デフォルト値
		rootParams.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える
		size++;

		rootParams_.emplace_back(rootParams);

	}
}

void PipelineObject::AddInputLayout(const char* semanticName, DXGI_FORMAT format, uint32_t index)
{
	inputLayout_.push_back(
		{
		semanticName, index, format, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		});
}
