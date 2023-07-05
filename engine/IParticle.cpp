#include "IParticle.h"
#include "DirectX.h"

IParticle::~IParticle() {
}

void IParticle::Init(size_t vertexCount)
{
	HRESULT result;
	vertices_.resize(vertexCount);
	UINT sizeVB = static_cast<UINT>(sizeof(VertexParticle) * vertices_.size());

	////���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = RDirectX::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapprop,
			D3D12_HEAP_FLAG_NONE,
			&resdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff_)
		);
	assert(SUCCEEDED(result));

	vertBuff_->SetName(L"PARTICLE VERT BUFF");

	// ���_�o�b�t�@�r���[�̍쐬
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(vertices_[0]);
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
		1, transform_.constBuffTransform_->GetGPUVirtualAddress());

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
		vertMap->color.a = particles[i].color.a;

		vertices_.at(i) = *vertMap;
	}
	std::copy(vertices_.begin(), vertices_.end(), vertMap);
	
	// �q���������
	vertBuff_->Unmap(0, nullptr);

	transform_.Update();

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
