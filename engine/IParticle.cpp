#include "IParticle.h"
#include "DirectX.h"
IParticle::IParticle()
{

}
IParticle::~IParticle() {
}

void IParticle::Init(int vertexCount)
{
	HRESULT result;

	vertices_.resize(vertexCount);
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(VertexParticle) * vertices_.size());

	// ���_�o�b�t�@�̐ݒ�
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
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

	// ���_�o�b�t�@�̐���
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexParticle* vertMap = nullptr;
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	for (uint32_t i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff_->Unmap(0, nullptr);
	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;				// ���_�o�b�t�@�̃T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);	// ���_1���̃f�[�^�T�C�Y
}

void IParticle::Update()
{
	//end_frame�𒴂�����폜
	DeleteUpdate();
	//�����X�V
	MoveUpdate();
	//�f�[�^�]��
	TransferBuff();
}

void IParticle::Draw()
{
	auto& cmdList = *RDirectX::GetInstance()->GetCommandList();

	// ���_�f�[�^�]��
	cmdList.IASetVertexBuffers(0, 1, &vbView_);

	// �萔�o�b�t�@�]��
	cmdList.SetGraphicsRootConstantBufferView(
		0, transform.constBuffTransform_->GetGPUVirtualAddress());

	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(texture.textureHandle);

	cmdList.DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}

void IParticle::TransferBuff()
{
	HRESULT result;
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexParticle* vertMap = nullptr;
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	for (size_t i = 0; i < particles.size(); i++)
	{
		vertMap->pos = particles[i].position;

		vertMap->scale = particles[i].scale;

		vertMap->rot = particles[i].rot;

		vertMap->ancorPoint = particles[i].ancorPoint_;

		vertMap->color = particles[i].color /*/ 255.0f*/;
		vertMap->color.a = particles[i].color.a / 255.0f;

		vertices_.at(i) = *vertMap;
	}
	// �q���������
	vertBuff_->Unmap(0, nullptr);

	transform.Update();

}

void IParticle::DeleteUpdate()
{
	for (size_t i = 0; i < particles.size(); i++)
	{
		if (particles[i].frame >= particles[i].end_frame)
		{
			particles.erase(particles.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}
}
