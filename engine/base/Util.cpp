#include <wrl.h>
using namespace Microsoft::WRL;
#include <cassert>
#include <string>
#include "Util.h"
#include "DirectX.h"

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

wchar_t* ConvertStrToWchar(const std::string string)
{
	// SJIS �� wstring	std::string�^�̃T�C�Y���擾
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, (wchar_t*)NULL, 0);

	// �o�b�t�@�̎擾		HandleName�̃T�C�Y����wchara_t��p�ӂ���
	wchar_t* result = new wchar_t[iBufferSize];

	// SJIS �� wstring	std::string�^����wchara_t�^�ɕϊ�����
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, result, iBufferSize);
	return result;
}

using namespace std::chrono;

int GetNowCount(void) {
	return static_cast<int>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

float GetNowTime(int startCount)
{
	// �o�ߎ���
	float nowTime = 0.0f;

	// ���݂̎���
	int nowCount = GetNowCount();

	// �o�ߎ���
	return nowTime = (nowCount - startCount) / 1000.0f;
}

float Clamp(float value, float max, float min)
{
	// �l���ő�l�������Ă�����ő�l��Ԃ�
	if (value >= max) return max;

	// �l���ŏ��l��������Ă�����ŏ��l��Ԃ�
	if (value <= min) return min;

	// �ǂ���ɂ����Ă͂܂�Ȃ�������l�����̂܂ܕԂ�
	return value;
}

void MoveTo(Vector3 goal, float speed, WorldTransform& WT)
{
	Vector3 dir = goal - WT.position;
	float dirLength = dir.length2();
	if (dirLength < speed * speed)
	{
		WT.position = goal;
		return;
	}
	WT.position =
		WT.position + dir.SetLength(speed);
}