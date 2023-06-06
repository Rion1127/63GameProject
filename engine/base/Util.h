#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#include <string>
#include <chrono>
#include "Vector3.h"
#include "WorldTransform.h"

//WARNING�̏ڍו\��
void DisplayWarningInfo(ID3D12Device* device);

#pragma region �V�F�[�_�[�ǂݍ���
void ShaderCompileFromFile(
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target,
	ID3DBlob** blob, ID3DBlob* errorBlob);

#pragma endregion

//std::string�^����wchara_t�^�ɕϊ�����
wchar_t* ConvertStrToWchar(const std::string string);

// ���݂̎������~���b�P�ʂŎ擾
int GetNowCount(void);

/// <summary>
/// �����ɂ��ꂽ���Ԃ���̌o�ߎ��Ԃ�Ԃ�
/// </summary>
/// <param name="startCount"> �J�n���� </param>
/// <returns></returns>
float GetNowTime(int startCount);

/// <summary>
	/// ���ꂽ�l���ő�l�������Ă�����ő�l��Ԃ��A�ŏ��l��������Ă�����ŏ��l��Ԃ��B
	/// �ǂ���ɂ����Ă͂܂�Ȃ��ꍇ�́A�l�����̂܂ܕԂ�
	/// </summary>
	/// <param name="value"> �l </param>
	/// <param name="max"> �ő�l </param>
	/// <param name="min"> �ŏ��l </param>
	/// <returns></returns>
float Clamp(float value, float max, float min);

void MoveTo(Vector3 goal, float speed,WorldTransform& WT);



template <class MapClass>
inline Microsoft::WRL::ComPtr<ID3D12Resource> CreateBuff(MapClass& map) 
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
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
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