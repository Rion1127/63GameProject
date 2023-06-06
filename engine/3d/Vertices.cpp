#include <DirectXMath.h>
using namespace DirectX;

#include "DirectX.h"
#include "Vertices.h"

void Vertices::Ini(ID3D12Device* device)
{
	HRESULT result;
#pragma region ���_�f�[�^
	//�O
	//vertices.push_back({ {-5.0f,-5.0f, 0.0f },{}, { 0.0f,1.0f } });//����
	//vertices.push_back({ {-5.0f, 5.0f, 0.0f },{}, { 0.0f,0.0f } });//����
	//vertices.push_back({ { 5.0f,-5.0f, 0.0f },{}, { 1.0f,1.0f } });//�E��
	//vertices.push_back({ { 5.0f, 5.0f, 0.0f },{}, { 1.0f,0.0f } });//�E��
	vertices.push_back({ { 0.0f, 0.0f, 0.0f },{0,0,1}, { 0,1 } });//�E��
	//���
	//vertices.push_back({ {-5.0f,  5.0f, 5.0f},{}, { 0.0f,0.0f} });//����
	//vertices.push_back({ {-5.0f, -5.0f, 5.0f},{}, { 0.0f,1.0f} });//����
	//vertices.push_back({ { 5.0f,  5.0f, 5.0f},{}, { 1.0f,0.0f} });//�E��
	//vertices.push_back({ { 5.0f, -5.0f, 5.0f},{}, { 1.0f,1.0f} });//�E��
	////��
	//vertices.push_back({ {-5.0f,-5.0f,-5.0f} ,{},{0.0f,1.0f} });//����
	//vertices.push_back({ {-5.0f,-5.0f, 5.0f} ,{},{0.0f,0.0f} });//����
	//vertices.push_back({ {-5.0f, 5.0f,-5.0f} ,{},{1.0f,1.0f} });//�E��
	//vertices.push_back({ {-5.0f, 5.0f, 5.0f} ,{},{1.0f,0.0f} });//�E��
	////�E				 
	//vertices.push_back({ {5.0f,-5.0f, 5.0f} ,{},{0.0f,0.0f} });//����
	//vertices.push_back({ {5.0f,-5.0f,-5.0f} ,{},{0.0f,1.0f} });//����
	//vertices.push_back({ {5.0f, 5.0f, 5.0f} ,{},{1.0f,0.0f} });//�E��
	//vertices.push_back({ {5.0f, 5.0f,-5.0f} ,{},{1.0f,1.0f} });//�E��
	////��				 
	//vertices.push_back({ { 5.0f,-5.0f,-5.0f} ,{},{0.0f,1.0f} });//����
	//vertices.push_back({ { 5.0f,-5.0f, 5.0f} ,{},{0.0f,0.0f} });//����
	//vertices.push_back({ {-5.0f,-5.0f,-5.0f} ,{},{1.0f,1.0f} });//�E��
	//vertices.push_back({ {-5.0f,-5.0f, 5.0f} ,{},{1.0f,0.0f} });//�E��
	////��				 
	//vertices.push_back({ { 5.0f, 5.0f, 5.0f} ,{},{0.0f,0.0f} });//����
	//vertices.push_back({ { 5.0f, 5.0f,-5.0f} ,{},{0.0f,1.0f} });//����
	//vertices.push_back({ {-5.0f, 5.0f, 5.0f} ,{},{1.0f,0.0f} });//�E��
	//vertices.push_back({ {-5.0f, 5.0f,-5.0f} ,{},{1.0f,1.0f} });//�E��
#pragma endregion
#pragma region ���_�C���f�b�N�X
	//�O
	/*indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);*/
	//���
	//indices.push_back(4);
	//indices.push_back(5);
	//indices.push_back(6);
	//indices.push_back(6);
	//indices.push_back(5);
	//indices.push_back(7);
	////��
	//indices.push_back(8);
	//indices.push_back(9);
	//indices.push_back(10);
	//indices.push_back(10);
	//indices.push_back(9);
	//indices.push_back(11);
	////�E
	//indices.push_back(12);
	//indices.push_back(13);
	//indices.push_back(14);
	//indices.push_back(14);
	//indices.push_back(13);
	//indices.push_back(15);
	////��
	//indices.push_back(16);
	//indices.push_back(17);
	//indices.push_back(18);
	//indices.push_back(18);
	//indices.push_back(17);
	//indices.push_back(19);
	////��
	//indices.push_back(20);
	//indices.push_back(21);
	//indices.push_back(22);
	//indices.push_back(22);
	//indices.push_back(21);
	//indices.push_back(23);
#pragma endregion

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB;
	UINT sizeIB;
	sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

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
	for (int i = 0; i < indices.size() / 3; i++) {
		//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K���i������1�ɂ���j
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);

	}

	// ���_�o�b�t�@�̐���
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexPosNormalUv* vertMap = nullptr;
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

	if (indices.size() != 0) {
		//���\�[�X�ݒ�
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeIB; // �C���f�b�N�X��񂪓��镪�̃T�C�Y
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// ���_�o�b�t�@�̐���
		result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
			&heapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&resDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff));
		assert(SUCCEEDED(result));

		// �C���f�b�N�X�o�b�t�@���}�b�s���O
		uint16_t* indexMap = nullptr;
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		assert(SUCCEEDED(result));
		// �S�C���f�b�N�X�ɑ΂���
		for (int i = 0; i < indices.size(); i++) {
			indexMap[i] = indices[i]; // �C���f�b�N�X���R�s�[
		}
		// �}�b�s���O����
		indexBuff->Unmap(0, nullptr);

		ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R16_UINT;
		ibView.SizeInBytes = sizeIB;
	}
}

void Vertices::CreateBuffer()
{
	HRESULT result;

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());

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
	for (int i = 0; i < indices.size() / 3; i++) {
		//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K���i������1�ɂ���j
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);

	}

	// ���_�o�b�t�@�̐���
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexPosNormalUv* vertMap = nullptr;
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);
	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;				// ���_�o�b�t�@�̃T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);	// ���_1���̃f�[�^�T�C�Y

	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());
	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // �C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	assert(SUCCEEDED(result));

	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// �S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i]; // �C���f�b�N�X���R�s�[
	}
	// �}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

}

void Vertices::Map()
{
	HRESULT result;
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexPosNormalUv* vertMap = nullptr;
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);
}

void Vertices::Draw(uint32_t indexSize,
	WorldTransform* worldTransform,
	UINT descriptorSize)
{
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->
		IASetIndexBuffer(&ibView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, worldTransform->constBuffTransform->GetGPUVirtualAddress());
	//�`��R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indexSize, 1, 0, 0, 0);
}

void Vertices::Draw(WorldTransform* worldTransform, UINT descriptorSize)
{
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, worldTransform->constBuffTransform->GetGPUVirtualAddress());
	//�`��R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Vertices::DrawInstanced(WorldTransform* worldTransform, UINT descriptorSize)
{
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	//commandList->IASetIndexBuffer(&ibView);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, worldTransform->constBuffTransform->GetGPUVirtualAddress());
	//�`��R�}���h
	DirectXCommon::GetInstance()->GetCommandList()->
		DrawInstanced((UINT)vertices.size(), 1, 0, 0);
}


