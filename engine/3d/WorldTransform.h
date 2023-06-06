#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include "math.h"
#include <d3d12.h>
#include <wrl.h>

#include "DirectX.h"
#include "Quaternion.h"

//定数バッファ用データ構造体（３D変換行列）
struct ConstBufferDataTransform {
	XMMATRIX mat; //3D変換行列
	XMMATRIX viewProj;	//ビューとプロジェクション合成行列
	Vector3 cameraPos;
};

class WorldTransform
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	WorldTransform();

	void SetScale(float x,float y,float z);
	void SetRotation(float x, float y, float z);
	void SetPosition(float x, float y, float z);

	void AddScale(float x, float y, float z);
	void AddRotation(float x, float y, float z);
	void AddPosition(float x, float y, float z);
	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	/// <param name="viewProjection">カメラ</param>
	/// <param name="isBillboard">0 = 無し; 1 = ビルボードあり; 2 = Y軸ビルボード</param>
	void Update(int isBillboard = 0);

	//定数バッファ（行列用）
	ComPtr<ID3D12Resource> constBuffTransform;
	//定数バッファマップ（行列用）
	ConstBufferDataTransform* constMapTransform;
	//親オブジェクトへのポインタ
	WorldTransform* parent = nullptr;
	
	//アフィン変換情報
	Vector3 scale = { 1,1,1 };
	Vector3 rotation = { 0,0,0 };
	Vector3 position = { 0,0,0 };
private:
	//ワールド変換行列
	XMMATRIX matWorld;

	//クォータニオン
	Quaternion q = { 0,1,0,0 };
};

