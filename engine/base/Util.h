#pragma once
#include <d3dcompiler.h>

#include <d3d12.h>
#include <string>
#include <chrono>
#include "Vector3.h"
#include "WorldTransform.h"
#include <cassert>
#include "Camera.h"

/**
 * @file Util.h
 * @brief 便利な関数をまとめている
 */

namespace fs = std::filesystem;
//シェーダー読み込み
void ShaderCompileFromFile(
	LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target,
	ID3DBlob** blob, ID3DBlob* errorBlob);
//goalまで値を変化させていく
void MoveTo(const Vector3& goal, float speed, Vector3& value);
void MoveTo(const Vector2& goal, float speed, Vector2& value);
//定数バッファ生成
template <class MapClass>
inline Microsoft::WRL::ComPtr<ID3D12Resource> CreateBuff(const MapClass& map)
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
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
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

std::wstring GetDirectoryPath(const std::wstring& origin);
//拡張子を入れ替える
std::wstring ReplaceExtension(const std::wstring& origin, const char* ext);
//wstringをstd::string(マルチバイト文字列)に変換
std::string ToUTF8(const std::wstring& value);
// std::string(マルチバイト文字列)からstd::wstring(ワイド文字列)を得る
std::wstring ToWideString(const std::string& str);
// std::wstring(ワイド文字列)からstd::string(マルチバイト文字列)を得る
std::string WStringToString(std::wstring oWString);
//3D座標から2D座標に変換する
Vector2 GetScreenPos(const WorldTransform& WT, const Camera& camera);

//int型数値を桁ごとに配列へ格納する
std::vector<int32_t> GetDigitNum(int32_t num, int32_t digitNum);
//digit桁目を取り出す
int32_t GetDigitNumber(int32_t number, int32_t digit);

template <class T>
inline std::unique_ptr<T> MakeUnique()
{
	return std::move(std::make_unique<T>());
}
/// <summary>
/// 同ディレクトリ内のファイル名を読み込む
/// </summary>
/// <param name="dir">ディレクトリ名指定</param>
/// <param name="extension">拡張子指定</param>
/// <returns></returns>
std::vector<std::string> FindFileNames(const std::string& dir, const std::string& extension,bool isExtension = true);
//数字を文字列に変換する
template <class Number>
std::string NumberToString(Number num) {
	std::ostringstream numstring;

	numstring << num;
	return numstring.str();
}
/// <summary>
/// ファイルから読み込んだ情報を格納する
/// </summary>
/// <typeparam name="T">読み込んだ文字列を格納する変数</typeparam>
/// <param name="line_stream">読み込んだファイルの一行</param>
/// <param name="key">項目の名前</param>
/// <param name="findName">項目の名前</param>
/// <param name="t">読み込んだ文字列を格納する変数</param>
/// <param name="isIgnore">除外する文字があるかどうか</param>
/// <param name="stringSize">除外する文字数</param>
/// <param name="ignoreChar">除外する文字</param>
template<class T>
void LoadFileString(std::stringstream& line_stream, const std::string& key, const std::string& findName, T& t, bool isIgnore = false, int32_t stringSize = 1, int ignoreChar = ' ') {

	if (key == findName)
	{
		if (isIgnore) line_stream.ignore(stringSize, ignoreChar);
		line_stream >> t;
	}
}
