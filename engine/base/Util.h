#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#include <string>
#include <chrono>
#include "Vector3.h"
#include "WorldTransform.h"

//WARNINGの詳細表示
void DisplayWarningInfo(ID3D12Device* device);

#pragma region シェーダー読み込み
void ShaderCompileFromFile(
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target,
	ID3DBlob** blob, ID3DBlob* errorBlob);

#pragma endregion

//std::string型からwchara_t型に変換する
wchar_t* ConvertStrToWchar(const std::string string);

// 現在の時刻をミリ秒単位で取得
int GetNowCount(void);

/// <summary>
/// 引数にいれた時間からの経過時間を返す
/// </summary>
/// <param name="startCount"> 開始時間 </param>
/// <returns></returns>
float GetNowTime(int startCount);

/// <summary>
	/// 入れた値が最大値を上回っていたら最大値を返し、最小値を下回っていたら最小値を返す。
	/// どちらにも当てはまらない場合は、値をそのまま返す
	/// </summary>
	/// <param name="value"> 値 </param>
	/// <param name="max"> 最大値 </param>
	/// <param name="min"> 最小値 </param>
	/// <returns></returns>
float Clamp(float value, float max, float min);

void MoveTo(Vector3 goal, float speed,WorldTransform& WT);



template <class MapClass>
inline Microsoft::WRL::ComPtr<ID3D12Resource> CreateBuff(MapClass& map) 
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Resource> buff;
	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};

	//定数バッファのヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(MapClass) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff)
	);
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = buff->Map(0, nullptr, (void**)&map);
	assert(SUCCEEDED(result));

	return buff;
}