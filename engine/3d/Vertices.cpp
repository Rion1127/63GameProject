#include <DirectXMath.h>

#include "DirectX.h"
#include "Vertices.h"

void Vertices::Ini(ID3D12Device* device)
{
	HRESULT result;

	vertices_.push_back({ { 0.0f, 0.0f, 0.0f },{0,0,1}, { 0,1 } });//�E��

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB;
	UINT sizeIB;
	sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());

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

	//�@���̌v�Z
	for (uint32_t i = 0; i < indices_.size() / 3; i++) {
		//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices_[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 0];
		unsigned short index1 = indices_[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 1];
		unsigned short index2 = indices_[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		Vector3 p0 = vertices_[index0].pos;
		Vector3 p1 = vertices_[index1].pos;
		Vector3 p2 = vertices_[index2].pos;
		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		Vector3 v1 = p1-p0;
		Vector3 v2 = p2-p0;
		//�O�ς͗������琂���ȃx�N�g��
		Vector3 normal = v1.cross(v2);
		//���K���i������1�ɂ���j
		normal = normal.normalize();
		//���߂��@���𒸓_�f�[�^�ɑ��
		vertices_[index0].normal = normal;
		vertices_[index1].normal = normal;
		vertices_[index2].normal = normal;

	}

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
	VertexPosNormalUv* vertMap = nullptr;
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (uint32_t i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // ���W���R�s�[
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

	if (indices_.size() != 0) {
		//���\�[�X�ݒ�
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeIB; // �C���f�b�N�X��񂪓��镪�̃T�C�Y
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
			IID_PPV_ARGS(&indexBuff_));
		assert(SUCCEEDED(result));

		// �C���f�b�N�X�o�b�t�@���}�b�s���O
		uint16_t* indexMap = nullptr;
		result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
		assert(SUCCEEDED(result));
		// �S�C���f�b�N�X�ɑ΂���
		for (uint32_t i = 0; i < indices_.size(); i++) {
			indexMap[i] = indices_[i]; // �C���f�b�N�X���R�s�[
		}
		// �}�b�s���O����
		indexBuff_->Unmap(0, nullptr);

		ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
		ibView_.Format = DXGI_FORMAT_R16_UINT;
		ibView_.SizeInBytes = sizeIB;
	}
}

void Vertices::CreateBuffer()
{
	HRESULT result;

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices_.size());

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

	//�@���̌v�Z
	for (uint32_t i = 0; i < indices_.size() / 3; i++) {
		//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices_[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 0];
		unsigned short index1 = indices_[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 1];
		unsigned short index2 = indices_[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		Vector3 p0 = vertices_[index0].pos;
		Vector3 p1 = vertices_[index1].pos;
		Vector3 p2 = vertices_[index2].pos;
		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		Vector3 v1 = p1 - p0;
		Vector3 v2 = p2 - p0;
		//�O�ς͗������琂���ȃx�N�g��
		Vector3 normal = v1.cross(v2);
		//���K���i������1�ɂ���j
		normal = normal.normalize();
		//���߂��@���𒸓_�f�[�^�ɑ��
		vertices_[index0].normal = normal;
		vertices_[index1].normal = normal;
		vertices_[index2].normal = normal;
	}

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
	VertexPosNormalUv* vertMap = nullptr;
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	for (uint32_t i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff_->Unmap(0, nullptr);
	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;				// ���_�o�b�t�@�̃T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);	// ���_1���̃f�[�^�T�C�Y

	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());
	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // �C���f�b�N�X��񂪓��镪�̃T�C�Y
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
		IID_PPV_ARGS(&indexBuff_));
	assert(SUCCEEDED(result));

	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// �S�C���f�b�N�X�ɑ΂���
	for (uint32_t i = 0; i < indices_.size(); i++) {
		indexMap[i] = indices_[i]; // �C���f�b�N�X���R�s�[
	}
	// �}�b�s���O����
	indexBuff_->Unmap(0, nullptr);

	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;

}

void Vertices::Map()
{
	HRESULT result;
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexPosNormalUv* vertMap = nullptr;
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (uint32_t i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff_->Unmap(0, nullptr);
}

void Vertices::Draw(uint32_t indexSize,
	WorldTransform* worldTransform,
	UINT descriptorSize)
{
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		IASetIndexBuffer(&ibView_);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, worldTransform->constBuffTransform_->GetGPUVirtualAddress());
	//�`��R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indexSize, 1, 0, 0, 0);
}

void Vertices::Draw(const WorldTransform& worldTransform)
{
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView_);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(1, worldTransform.constBuffTransform_->GetGPUVirtualAddress());
	//�`��R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void Vertices::DrawInstanced(WorldTransform* worldTransform, UINT descriptorSize)
{
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	//commandList->IASetIndexBuffer(&ibView_);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, worldTransform->constBuffTransform_->GetGPUVirtualAddress());
	//�`��R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		DrawInstanced((UINT)vertices_.size(), 1, 0, 0);
}


