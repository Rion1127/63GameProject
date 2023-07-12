#include <wrl.h>
using namespace Microsoft::WRL;
#include <cassert>
#include <string>
#include "Util.h"
#include "DirectX.h"
#include <filesystem>

void DisplayWarningInfo(ID3D12Device* device)
{
	assert(device);
	ComPtr<ID3D12DebugDevice> debugInterface;

	if (SUCCEEDED(device->QueryInterface(debugInterface.GetAddressOf())))
	{
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		debugInterface->Release();
	}
}

//�V�F�[�_�[�ǂݍ���
void ShaderCompileFromFile(
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target,
	ID3DBlob** blob, ID3DBlob* errorBlob)
{
	HRESULT result;

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		fileName, // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		entryPoint, target, // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		blob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

namespace fs = std::filesystem;
std::wstring GetDirectoryPath(const std::wstring& origin)
{
	std::wstring name = origin;

	fs::path p = origin.c_str();
	return p.remove_filename().c_str();
}
//�g���q�����ւ��� (const char* ext)�́u.�v���ʂ����g���q�����
std::wstring ReplaceExtension(const std::wstring& origin, const char* ext)
{
	fs::path p = origin.c_str();
	return p.replace_extension(ext).c_str();
}
//wstring��std::string(�}���`�o�C�g������)�ɕϊ�
std::string ToUTF8(const std::wstring& value)
{
	auto length = WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr, nullptr);
	std::vector<char> buffer;
	buffer.resize(length);
	WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, buffer.data(), length, nullptr, nullptr);

	std::string result(buffer.data());

	return result;
}
// std::string(�}���`�o�C�g������)����std::wstring(���C�h������)�𓾂�
std::wstring ToWideString(const std::string& str)
{
	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);
	return wstr;
}
// std::wstring(���C�h������)����std::string(�}���`�o�C�g������)�𓾂�
std::string WStringToString(std::wstring oWString)
{
	// wstring �� SJIS
	uint32_t iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char*)NULL, 0, NULL, NULL);

	// �o�b�t�@�̎擾
	std::vector<CHAR> cpMultiByte;
	cpMultiByte.resize(iBufferSize);
	// wstring �� SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte.data()
		, iBufferSize, NULL, NULL);

	// string�̐���
	std::string oRet(cpMultiByte.data(), cpMultiByte.data() + iBufferSize - 1);

	// �o�b�t�@�̔j��


	// �ϊ����ʂ�Ԃ�
	return(oRet);
}


void MoveTo(const Vector3& goal, float speed, WorldTransform& WT)
{
	Vector3 dir = goal - WT.position_;
	float dirLength = dir.length();
	if (dirLength < speed * speed)
	{
		WT.position_ = goal;
		return;
	}
	WT.position_ =
		WT.position_ + dir.SetLength(speed);
}

void MoveTo(const Vector3& goal, float speed, Vector3& value)
{
	Vector3 dir = goal - value;
	float dirLength = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
	if (dirLength < speed * speed)
	{
		value = goal;
		return;
	}
	value =
		value + dir.SetLength(speed);
}

Vector2 GetScreenPos(const WorldTransform& WT, const Camera& camera)
{
	Vector2 result;

	Vector4 pos(0,0,0,1);
	Vector2 winSize = WinAPI::GetInstance()->GetWindowSize();
	Matrix4 viewPort = {
		winSize.x / 2.f,	           0,0,0,
		              0,-winSize.y / 2.f,0,0,
		              0,               0,1,0,
		winSize.x / 2.f, winSize.y / 2.f,0,1,
	};
	//�R�������Q�����̕ϊ�
	pos = Vec4MulMat4(pos, WT.matWorld_);
	pos = Vec4MulMat4(pos, camera.matView_);
	pos = Vec4MulMat4(pos, camera.matProjection_);
	pos = Vec4MulMat4(pos, viewPort);

	result = { pos.x ,pos.y };

	return result;
}

