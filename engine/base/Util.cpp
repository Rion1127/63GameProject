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

wchar_t* ConvertStrToWchar(const std::string string)
{
	// SJIS → wstring	std::string型のサイズを取得
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, (wchar_t*)NULL, 0);

	// バッファの取得		HandleNameのサイズ分のwchara_tを用意する
	wchar_t* result = new wchar_t[iBufferSize];

	// SJIS → wstring	std::string型からwchara_t型に変換する
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, result, iBufferSize);
	return result;
}

using namespace std::chrono;

int GetNowCount(void) {
	return static_cast<int>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

float GetNowTime(int startCount)
{
	// 経過時間
	float nowTime = 0.0f;

	// 現在の時間
	int nowCount = GetNowCount();

	// 経過時間
	return nowTime = (nowCount - startCount) / 1000.0f;
}

float Clamp(float value, float max, float min)
{
	// 値が最大値を上回っていたら最大値を返す
	if (value >= max) return max;

	// 値が最小値を下回っていたら最小値を返す
	if (value <= min) return min;

	// どちらにも当てはまらなかったら値をそのまま返す
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