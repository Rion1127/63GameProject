#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
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

DirectXCommon* Sprite::directX_ = nullptr;
int Sprite::allNum = 0;

void Sprite::StaticIni()
{
	directX_ = DirectXCommon::GetInstance();
}

void Sprite::Ini(std::string guiname)
{
	spriteNum = allNum;
	Sprite::AddAllNum();




	std::string noneString = "";
	//���������Ă��Ȃ��ꍇ
	if (guiname == noneString) {

		std::ostringstream oss;

		oss << spriteNum;
		name = "Sprite" + oss.str();
		gui = name.c_str();
	}
	else {
		guiName_ = guiname;

		gui = guiName_.c_str();
	}

	HRESULT result;
#pragma region ���_�f�[�^
	//���_�f�[�^
	vertices.push_back({ {   -0.0f,100.0f,0.0f },{0.0f,1.0f} });//����
	vertices.push_back({ {   -0.0f, +0.0f,0.0f },{0.0f,0.0f} });//����
	vertices.push_back({ { +100.0f,100.0f,0.0f },{1.0f,1.0f} });//�E��
	vertices.push_back({ { +100.0f, +0.0f,0.0f },{1.0f,0.0f} });//�E��

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
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

	result = directX_->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);
#pragma endregion

#pragma region �C���f�b�N�X�f�[�^
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	result = directX_->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i]; // ���W���R�s�[
	}
	// �q���������
	indexBuff->Unmap(0, nullptr);
	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

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

	result = directX_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));
	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);
#pragma endregion

#pragma region �g�����X�t�H�[��
	{
		// �q�[�v�v���p�e�B
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// ���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

		// �萔�o�b�t�@�̐���
		result = directX_->GetDevice()->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(result));
	}

	// �萔�o�b�t�@�}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));
	//�P�ʍs�����
	constMapTransform->mat = XMMatrixIdentity();

	// �ˉe�s��v�Z
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)1280, (float)720, 0.0f, 0.0f, 1.0f);

	//�X�P�[��
	Scale_ = { 1.f,1.f };
	color_ = { 1,1,1,1 };
	anchorPoint_ = { 0.5f,0.5f };
#pragma endregion
}

void Sprite::SetPos(Vector2 pos)
{
	pos_ = pos;
}

void Sprite::SetRot(float rot)
{
	rot_ = rot;
}

void Sprite::SetScale(Vector2 scale)
{
	Scale_ = scale;
}

void Sprite::DrawImGui()
{
	ImGui::Begin(gui);
	/* �����ɒǉ�������GUI������ */

	//static int clicked = 0;
	if (ImGui::Button("isInvisible_"))clicked++;
	//�N���b�N���ꂽ�ꍇ�\�����Ȃ�
	if (clicked & 1)
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
	if (ImGui::CollapsingHeader("Posision"))
	{
		float x = pos_.x;
		float y = pos_.y;
		ImGui::SliderFloat("pos.x", &x, 0.0f, 2000.0f, "x = %.3f");
		ImGui::SliderFloat("pos.y", &y, 0.0f, 2000.0f, "y = %.3f");
		pos_.x = x;
		pos_.y = y;
	}
	// �X�P�[���ύX
	if (ImGui::CollapsingHeader("Scale"))
	{
		float scalex = Scale_.x;
		float scaley = Scale_.y;
		ImGui::SliderFloat("scale.x", &scalex, 0.0f, 1.0f, "x = %.3f");
		ImGui::SliderFloat("scale.y", &scaley, 0.0f, 1.0f, "y = %.3f");
		Scale_.x = scalex;
		Scale_.y = scaley;
	}
	//��]
	if (ImGui::CollapsingHeader("Rotation"))
	{
		float rot = rot_;
		ImGui::SliderFloat("Rot", &rot, 0.0f, ConvertAngleToRadian(360), "x = %.3f");
		rot_ = rot;
	}

	ImGui::End();
}

void Sprite::PreDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	directX_->GetCommandList()->SetPipelineState(
		PipelineManager::GetSpritePipeline(3)->gerPipelineState());

	directX_->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetSpritePipeline(3)->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	directX_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g
}

void Sprite::Draw(UINT descriptorSize)
{
	if (isImguiDisplay) {
		DrawImGui();
	}

	if (isInvisible_) {
		return;
	}
	HRESULT result = S_OK;
#pragma region �摜�̃T�C�Y���擾
	D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetResDesc(descriptorSize);

#pragma endregion
	float left = (0.0f - anchorPoint_.x) * resDesc.Width;
	float right = (1.0f - anchorPoint_.x) * resDesc.Width;
	float top = (0.0f - anchorPoint_.y) * resDesc.Height;
	float bottom = (1.0f - anchorPoint_.y) * resDesc.Height;
	//���E���]
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	//�㉺���]
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

#pragma region �摜�̒��_�f�[�^���X�V
	vertices.at(LB).pos = { left	, bottom	,0 };//����
	vertices.at(LT).pos = { left	, top		,0 };//����
	vertices.at(RB).pos = { right	, bottom	,0 };//�E��
	vertices.at(RT).pos = { right	, top		,0 };//�E��

	std::copy(std::begin(vertices), std::end(vertices), vertMap);

	// ���[���h�s��̍X�V
	matWorld_ = XMMatrixIdentity();
	matWorld_ *= XMMatrixScaling(Scale_.x, Scale_.y, 0);
	matWorld_ *= XMMatrixRotationZ(rot_);
	matWorld_ *= XMMatrixTranslation(pos_.x, pos_.y, 0.0f);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	constMapMaterial->color = color_;
	constMapTransform->mat = matWorld_ * matProjection; // �s��̍���

#pragma endregion

#pragma region �摜�͈͎w��
	//�؂���͈͂��ǂ����0�̏ꍇUV���W�͕ς��Ȃ�
	if (textureSize.x != 0 && textureSize.y != 0) {
		float tex_left = textureLeftTop_.x / resDesc.Width;
		float tex_right = (textureLeftTop_.x + textureSize.x) / resDesc.Width;
		float tex_top = textureLeftTop_.y / resDesc.Height;
		float tex_bottom = (textureLeftTop_.y + textureSize.y) / resDesc.Height;
		//���_��UV�ɔ��f����
		vertices.at(LB).uv = { tex_left	, tex_bottom };//����
		vertices.at(LT).uv = { tex_left	, tex_top };//����
		vertices.at(RB).uv = { tex_right, tex_bottom };//�E��
		vertices.at(RT).uv = { tex_right, tex_top };//�E��
	}
#pragma endregion

	TextureManager::GetInstance()->SetGraphicsDescriptorTable(descriptorSize);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directX_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	directX_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	directX_->GetCommandList()->IASetIndexBuffer(&ibView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directX_->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	//�`��R�}���h
	directX_->GetCommandList()->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Sprite::Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize)
{
	HRESULT result = S_OK;

#pragma region �摜�̒��_�f�[�^���X�V
	vertices.at(0).pos = { LdX,LdY,0 };//����
	vertices.at(1).pos = { LuX,LuY,0 };//����
	vertices.at(2).pos = { RdX,RdY,0 };//�E��
	vertices.at(3).pos = { RuX,RuY,0 };//�E��
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	std::copy(std::begin(vertices), std::end(vertices), vertMap);

	// ���[���h�s��̍X�V
	matWorld_ = XMMatrixIdentity();
	matWorld_ *= XMMatrixScaling(Scale_.x, Scale_.y, 0);
	matWorld_ *= XMMatrixRotationZ(rot_);
	matWorld_ *= XMMatrixTranslation(pos_.x, pos_.y, 0.0f);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	constMapMaterial->color = color_;
	constMapTransform->mat = matProjection; // �s��̍���

#pragma endregion
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(descriptorSize);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directX_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	directX_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	directX_->GetCommandList()->IASetIndexBuffer(&ibView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	directX_->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	//�`��R�}���h
	directX_->GetCommandList()->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Sprite::SetBlend(int blend)
{
	if (blend > 3) blend = 3;
	else if (blend < 0) blend = 0;
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	directX_->GetCommandList()->SetPipelineState(
		PipelineManager::GetSpritePipeline(blend)->gerPipelineState());

	directX_->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetSpritePipeline(blend)->GetRootSignature());
}
