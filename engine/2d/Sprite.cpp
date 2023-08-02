#include <d3dcompiler.h>

#include <d3d12.h>
#include <d3dx12.h>
#include <cassert>
#include <string>
#include "Texture.h"
#include <DirectXTex.h>
#include "Util.h"
#include "Sprite.h"
#include <sstream>
#include "myMath.h"

uint32_t Sprite::SAllNum = 0;

void Sprite::Ini(const std::string& guiname)
{
	spriteNum_ = SAllNum;
	Sprite::AddAllNum();

	std::string noneString = "";
	//���������Ă��Ȃ��ꍇ
	if (guiname == noneString) {

		std::ostringstream oss;

		oss << spriteNum_;
		name_ = "Sprite" + oss.str();
		gui_ = name_.c_str();
	}
	else {
		guiName_ = guiname;

		gui_ = guiName_.c_str();
	}

	HRESULT result;
#pragma region ���_�f�[�^
	//���_�f�[�^
	vertices_.push_back({ {   -0.0f,100.0f,0.0f },{0.0f,1.0f} });//����
	vertices_.push_back({ {   -0.0f, +0.0f,0.0f },{0.0f,0.0f} });//����
	vertices_.push_back({ { +100.0f,100.0f,0.0f },{1.0f,1.0f} });//�E��
	vertices_.push_back({ { +100.0f, +0.0f,0.0f },{1.0f,0.0f} });//�E��

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());
	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (uint32_t i = 0; i < vertices_.size(); i++) {
		vertMap_[i] = vertices_[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff_->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView_.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);
#pragma endregion

#pragma region �C���f�b�N�X�f�[�^
	indices_.push_back(0);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(3);

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (uint32_t i = 0; i < indices_.size(); i++) {
		indexMap[i] = indices_[i]; // ���W���R�s�[
	}
	// �q���������
	indexBuff_->Unmap(0, nullptr);
	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;

#pragma endregion

#pragma region �V�F�[�_�[�ɐF��n��

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	//���\�[�X�ݒ�
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_);
	assert(SUCCEEDED(result));
	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial_->color = Color(1, 1, 1, 1.0f);
#pragma endregion

#pragma region �g�����X�t�H�[��
	{
		// �q�[�v�v���p�e�B
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// ���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

		// �萔�o�b�t�@�̐���
		result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&constBuffTransform_));
		assert(SUCCEEDED(result));
	}

	// �萔�o�b�t�@�}�b�s���O
	result = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result));
	//�P�ʍs�����
	constMapTransform_->mat.UnitMatrix();

	// �ˉe�s��v�Z
	matProjection_ = {
		2 / WinAPI::GetWindowSize().x,0,0,0,
		0,-2 / WinAPI::GetWindowSize().y,0,0,
		0,0,1,0,
		-1,1,0,1
	};
	//�X�P�[��
	Scale_ = { 1.f,1.f };
	anchorPoint_ = { 0.5f,0.5f };
#pragma endregion
}

void Sprite::DrawImGui()
{
	ImGui::Begin(gui_);
	/* �����ɒǉ�������GUI������ */

	//static int clicked_ = 0;
	if (ImGui::Button("isInvisible_"))clicked_++;
	//�N���b�N���ꂽ�ꍇ�\�����Ȃ�
	if (clicked_ & 1)
	{
		ImGui::SameLine();
		ImGui::Text("TRUE");
		isInvisible_ = true;
	}
	else {
		ImGui::SameLine();
		ImGui::Text("FALSE");
		isInvisible_ = false;
	}

	// Menu Bar
	float pos[2] = { pos_.x,pos_.y };
	ImGui::DragFloat2("pos", pos, 1.0f, 0.f,WinAPI::GetWindowSize().x);

	pos_.x = pos[0];
	pos_.y = pos[1];
	// �X�P�[���ύX
	float scale[2] = { Scale_.x,Scale_.y };
	ImGui::DragFloat2("scale", scale, 0.1f, 0.0f, 10.0f);

	Scale_.x = scale[0];
	Scale_.y = scale[1];

	//��]
	float rot = rot_;
	ImGui::SliderFloat("Rot", &rot, 0.0f, Radian(360), "x = %.3f");
	rot_ = rot;

	// textureLeftTop�ύX
	float textureLeftTop[2] = { textureLeftTop_.x,textureLeftTop_.y };
	ImGui::DragFloat2("textureLeftTop", textureLeftTop, 1.0f, 0.0f, 1280.0f);

	textureLeftTop_.x = textureLeftTop[0];
	textureLeftTop_.y = textureLeftTop[1];

	// textureSize�ύX
	float textureSize[2] = { textureSize_.x,textureSize_.y };
	ImGui::DragFloat2("textureSize", textureSize, 1.0f, 0.0f, 1280.0f);

	textureSize_.x = textureSize[0];
	textureSize_.y = textureSize[1];

	// color�ύX
	float color[4] = { color_.r,color_.g,color_.b,color_.a };
	ImGui::DragFloat4("color", color, 1.0f, 0.0f, 255.0f);

	color_.r = color[0];
	color_.g = color[1];
	color_.b = color[2];
	color_.a = color[3];

	ImGui::End();
}

void Sprite::Update()
{
	if (isInvisible_)
	{
		return;
	}

	// ���[���h�s��̍X�V
	matWorld_.UnitMatrix();
	matWorld_ *= ConvertScalingMat({ Scale_.x, Scale_.y, 0 });
	matWorld_ *= ConvertRotationZAxisMat(rot_);
	matWorld_ *= ConvertTranslationMat({ pos_.x, pos_.y, 0.0f });

	// �萔�o�b�t�@�Ƀf�[�^�]��
	constMapMaterial_->color = color_ / 255.f;
	constMapTransform_->mat = matWorld_ * matProjection_; // �s��̍���

#pragma endregion

	TransferVertex();
}

void Sprite::Draw()
{
	if (isImguiDisplay_) {
		DrawImGui();
	}

	if (isInvisible_) {
		return;
	}

	TextureManager::GetInstance()->SetGraphicsDescriptorTable(texture_.textureHandle);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(1, constBuffMaterial_->GetGPUVirtualAddress());
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView_);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	//�`��R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void Sprite::Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize)
{
	HRESULT result = S_OK;

#pragma region �摜�̒��_�f�[�^���X�V
	vertices_.at(0).pos = { LdX,LdY,0 };//����
	vertices_.at(1).pos = { LuX,LuY,0 };//����
	vertices_.at(2).pos = { RdX,RdY,0 };//�E��
	vertices_.at(3).pos = { RuX,RuY,0 };//�E��
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	std::copy(std::begin(vertices_), std::end(vertices_), vertMap_);

	// ���[���h�s��̍X�V
	matWorld_.UnitMatrix();
	matWorld_ *= ConvertScalingMat({ Scale_.x, Scale_.y, 0 });
	matWorld_ *= ConvertRotationZAxisMat(rot_);
	matWorld_ *= ConvertTranslationMat({ pos_.x, pos_.y, 0.0f });

	// �萔�o�b�t�@�Ƀf�[�^�]��
	constMapMaterial_->color = color_;
	constMapTransform_->mat = matProjection_; // �s��̍���

#pragma endregion
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(descriptorSize);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView_);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	//�`��R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void Sprite::SetBlend(uint32_t blend)
{
	//if (blend > 3) blend = 3;
	//else if (blend < 0) blend = 0;
	//// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	//RDirectX::GetInstance()->GetCommandList()->SetPipelineState(
	//	PipelineManager::GetSpritePipeline(blend)->gerPipelineState());

	//RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
	//	PipelineManager::GetSpritePipeline(blend)->GetRootSignature());
}

void Sprite::TransferVertex()
{
	float left = (0.0f - anchorPoint_.x) * texture_.size_.x;
	float right = (1.0f - anchorPoint_.x) * texture_.size_.x;
	float top = (0.0f - anchorPoint_.y) * texture_.size_.y;
	float bottom = (1.0f - anchorPoint_.y) * texture_.size_.y;
	//���E���]
	if (isFlipX_)
	{
		left = -left;
		right = -right;
	}
	//�㉺���]
	if (isFlipY_)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices_.at(LB).pos = { left	, bottom	,0 };//����
	vertices_.at(LT).pos = { left	, top		,0 };//����
	vertices_.at(RB).pos = { right	, bottom	,0 };//�E��
	vertices_.at(RT).pos = { right	, top		,0 };//�E��

	std::copy(std::begin(vertices_), std::end(vertices_), vertMap_);

	//�؂���͈͂��ǂ����0�̏ꍇUV���W�͕ς��Ȃ�
	if (textureSize_.x != 0 && textureSize_.y != 0)
	{
		float tex_left = textureLeftTop_.x / texture_.size_.x;
		float tex_right = (textureLeftTop_.x + textureSize_.x) / texture_.size_.x;
		float tex_top = textureLeftTop_.y / texture_.size_.y;
		float tex_bottom = (textureLeftTop_.y + textureSize_.y) / texture_.size_.y;
		//���_��UV�ɔ��f����
		vertices_.at(LB).uv = { tex_left	, tex_bottom };//����
		vertices_.at(LT).uv = { tex_left	, tex_top };//����
		vertices_.at(RB).uv = { tex_right, tex_bottom };//�E��
		vertices_.at(RT).uv = { tex_right, tex_top };//�E��
	}
}
