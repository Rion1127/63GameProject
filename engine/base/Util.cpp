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

//シェーダー読み込み
void ShaderCompileFromFile(
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target,
	ID3DBlob** blob, ID3DBlob* errorBlob)
{
	HRESULT result;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		fileName, // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		entryPoint, target, // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		blob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
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
//拡張子を入れ替える (const char* ext)は「.」をぬいた拡張子を入力
std::wstring ReplaceExtension(const std::wstring& origin, const char* ext)
{
	fs::path p = origin.c_str();
	return p.replace_extension(ext).c_str();
}
//wstringをstd::string(マルチバイト文字列)に変換
std::string ToUTF8(const std::wstring& value)
{
	auto length = WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr, nullptr);
	std::vector<char> buffer;
	buffer.resize(length);
	WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, buffer.data(), length, nullptr, nullptr);

	std::string result(buffer.data());

	return result;
}
// std::string(マルチバイト文字列)からstd::wstring(ワイド文字列)を得る
std::wstring ToWideString(const std::string& str)
{
	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);
	return wstr;
}
// std::wstring(ワイド文字列)からstd::string(マルチバイト文字列)を得る
std::string WStringToString(std::wstring oWString)
{
	// wstring → SJIS
	uint32_t iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char*)NULL, 0, NULL, NULL);

	// バッファの取得
	std::vector<CHAR> cpMultiByte;
	cpMultiByte.resize(iBufferSize);
	// wstring → SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte.data()
		, iBufferSize, NULL, NULL);

	// stringの生成
	std::string oRet(cpMultiByte.data(), cpMultiByte.data() + iBufferSize - 1);

	// バッファの破棄


	// 変換結果を返す
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
	//３次元→２次元の変換
	pos = Vec4MulMat4(pos, WT.matWorld_);
	pos = Vec4MulMat4(pos, camera.matView_);
	pos = Vec4MulMat4(pos, camera.matProjection_);
	pos = Vec4MulMat4(pos, viewPort);

	result = { pos.x ,pos.y };

	return result;
}

