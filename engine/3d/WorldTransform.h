#pragma once
#include "math.h"
#include <d3d12.h>
#include <wrl.h>

#include "DirectX.h"
#include "Quaternion.h"
#include "myMath.h"

//定数バッファ用データ構造体（３D変換行列）
struct ConstBufferDataTransform {
	Matrix4 mat; //3D変換行列
	Matrix4 viewProj;	//ビューとプロジェクション合成行列
	Vector3 cameraPos;
};

class WorldTransform
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	WorldTransform();

	void SetScale(Vector3 scale);
	void SetRotation(Vector3 rot);
	void SetPosition(Vector3 pos);

	void AddScale(Vector3 scale);
	void AddRotation(Vector3 rot);
	void AddPosition(Vector3 pos);
	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	/// <param name="viewProjection">カメラ</param>
	/// <param name="isBillboard">0 = 無し; 1 = ビルボードあり; 2 = Y軸ビルボード</param>
	void Update(uint32_t isBillboard = 0);

	//定数バッファ（行列用）
	ComPtr<ID3D12Resource> constBuffTransform_;
	//定数バッファマップ（行列用）
	ConstBufferDataTransform* constMapTransform_;
	//親オブジェクトへのポインタ
	WorldTransform* parent_ = nullptr;
	
	//アフィン変換情報
	Vector3 scale_ = { 1,1,1 };
	Vector3 rotation_ = { 0,0,0 };
	Vector3 position_ = { 0,0,0 };
private:
	//ワールド変換行列
	Matrix4 matWorld_;

	//クォータニオン
	Quaternion quaternion_ = { 0,1,0,0 };
};

