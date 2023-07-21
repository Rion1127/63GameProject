#pragma once
#include <d3dcompiler.h>

#include <d3d12.h>
#include <string>
#include <chrono>
#include "Vector3.h"
#include "WorldTransform.h"
#include <cassert>
#include "Camera.h"

//WARNING�̏ڍו\��
void DisplayWarningInfo(ID3D12Device* device);

//�V�F�[�_�[�ǂݍ���
void ShaderCompileFromFile(
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target,
	ID3DBlob** blob, ID3DBlob* errorBlob);

void MoveTo(const Vector3& goal, float speed,WorldTransform& WT);
void MoveTo(const Vector3& goal, float speed, Vector3& value);

//�萔�o�b�t�@
template <class MapClass>
inline Microsoft::WRL::ComPtr<ID3D12Resource> CreateBuff(const MapClass& map) 
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Resource> buff;
	//�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	//�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};

	//�萔�o�b�t�@�̃q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//�萔�o�b�t�@�̃��\�[�X�ݒ�
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(MapClass) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff)
	);
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = buff->Map(0, nullptr, (void**)&map);
	assert(SUCCEEDED(result));

	return buff;
}

namespace fs = std::filesystem;

std::wstring GetDirectoryPath(const std::wstring& origin);
//�g���q�����ւ���
std::wstring ReplaceExtension(const std::wstring& origin, const char* ext);
//wstring��std::string(�}���`�o�C�g������)�ɕϊ�
std::string ToUTF8(const std::wstring& value);
// std::string(�}���`�o�C�g������)����std::wstring(���C�h������)�𓾂�
std::wstring ToWideString(const std::string& str);
// std::wstring(���C�h������)����std::string(�}���`�o�C�g������)�𓾂�
std::string WStringToString(std::wstring oWString);


Vector2 GetScreenPos(const WorldTransform& WT, const Camera& camera);

template <class T>
inline std::unique_ptr<T> MakeUnique()
{
	return std::move(std::make_unique<T>());
}