#include <cassert>
#include "Util.h"
#include "Pipeline.h"

const std::string kBaseDirectory = "application/Resources/shader/";

D3D12_STATIC_SAMPLER_DESC SetSAMPLER_DESC()
{
	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//�S�ă��j�A�ۊ�
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_����̂ݎg�p�\

	return samplerDesc;
}

void SetBlend(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc, uint32_t blend)
{

	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��
	//���ʐݒ�i�A���t�@�l�j
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��
	//���Z����
	if (blend == BlendNum::ADD)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̒l��100%�g��
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	//���Z����
	else if (blend == BlendNum::SUB)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//�f�X�g����\�[�X�����Z
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	//�F���]
	else if (blend == BlendNum::NEGA)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
		blenddesc.DestBlend = D3D12_BLEND_ZERO;			//�g��Ȃ�
	}
	//����������
	else if (blend == BlendNum::ALPHA)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//�\�[�X�̃A���t�@�l
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l
	}

	pipelineDesc.BlendState.RenderTarget[0] = blenddesc;
	pipelineDesc.BlendState.RenderTarget[1] = blenddesc;
}

void PipelineObject::Create(BlendNum blendNum, CULL_MODE cullmode,
	TOPOLOGY_TYPE topologytype, WRIGHT_MASK depthWriteMasc,
	TEXTURE_ADDRESS_MODE uvMode)
{
	HRESULT result;

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// �V�F�[�_�[�̐ݒ�
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

	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE(uvMode);					//���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE(uvMode);					//�c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE(uvMode);					//���s�J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//�S�ă��j�A�ۊ�
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_����̂ݎg�p�\

	// ���[�g�V�O�l�`��
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	// ���[�g�V�O�l�`���̐ݒ�
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams_.data();	//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = (UINT)rootParams_.size();			//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob_);
	assert(SUCCEEDED(result));
	result = RDirectX::GetInstance()->GetDevice()->
		CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature_.Get();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE(cullmode); // �J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	if (depthWriteMasc == WRIGHT_MASK::DEPTH_ENABLE_FALSE)
	{
		pipelineDesc.DepthStencilState.DepthEnable = false;
	}
	else
	{
		pipelineDesc.DepthStencilState.DepthEnable = true;	//�[�x�e�X�g���s��
		pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK(depthWriteMasc);//�������݋���
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//��������΍��i
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	}

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout_.data();
	pipelineDesc.InputLayout.NumElements = (UINT)inputLayout_.size();

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE(topologytype);
	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 2; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �u�����h�X�e�[�g
	SetBlend(pipelineDesc, blendNum);
	// �p�C�v�����X�e�[�g�̐���
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
	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
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
	//inputLayout + 1�̐���rootParams_�����
	for (int32_t i = 0; i < addNum; i++)
	{
		D3D12_ROOT_PARAMETER rootParams{};
		//�z��̍ŏ��Ƀe�N�X�`��
		if (rootParams_.size() == 0)
		{
			//�f�X�N���v�^�����W�̐ݒ�
			D3D12_DESCRIPTOR_RANGE* descriptorRange =
				new D3D12_DESCRIPTOR_RANGE{};
			std::vector<CD3DX12_DESCRIPTOR_RANGE> cDescRangeSRVs;

			//�f�X�N���v�^�����W�̐ݒ�
			descriptorRange->NumDescriptors = 1;					//��x�̕`��ɍD�����e�N�X�`����1���Ȃ̂�1
			descriptorRange->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			descriptorRange->BaseShaderRegister = 0;				//�e�N�X�`�����W�X�^0��
			descriptorRange->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			rootParams.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
			rootParams.DescriptorTable.pDescriptorRanges = descriptorRange;//�f�X�N���v�^�����W
			rootParams.DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
			rootParams.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
		}
		else
		{
			//�萔�o�b�t�@
			rootParams.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
			rootParams.Descriptor.ShaderRegister = size;					//�萔�o�b�t�@�ԍ�
			rootParams.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
			rootParams.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
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
		//�f�X�N���v�^�����W�̐ݒ�
		CD3DX12_DESCRIPTOR_RANGE* descriptorRange =
			new CD3DX12_DESCRIPTOR_RANGE{};

		descriptorRange->Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i);

		rootParams.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
		rootParams.DescriptorTable.pDescriptorRanges = descriptorRange;//�f�X�N���v�^�����W
		rootParams.DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
		rootParams.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����

		rootParams_.emplace_back(rootParams);
	}

	for (int32_t i = 0; i < addNum; i++)
	{
		D3D12_ROOT_PARAMETER rootParams{};
		//�萔�o�b�t�@
		rootParams.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
		rootParams.Descriptor.ShaderRegister = size;					//�萔�o�b�t�@�ԍ�
		rootParams.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
		rootParams.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
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
