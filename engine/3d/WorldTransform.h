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

enum class RotType {
	Euler,
	Quaternion
};

class WorldTransform
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	WorldTransform();

	void SetScale(Vector3 scale) { scale_ = scale; }
	void SetRotation(Vector3 rot) { rotation_ = rot; }
	void SetPosition(Vector3 pos) { position_ = pos; }
	void SetQuaternion(Quaternion q) { quaternion_ = q; }
	void SetRotType(RotType type) { rotType = type; }

	void AddScale(float x, float y, float z);
	void AddRotation(float x, float y, float z);
	void AddPosition(float x, float y, float z);

	void AddScale(Vector3 scale) { scale_ += scale; }
	void AddRotation(Vector3 rot) { rotation_ += rot; }
	void AddPosition(Vector3 pos) { position_ += pos; }
	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	/// <param name="viewProjection">カメラ</param>
	/// <param name="isBillboard">0 = 無し; 1 = ビルボードあり; 2 = Y軸ビルボード</param>
	void Update(uint32_t isBillboard = 0);

	Matrix4 GetMatWorld() { return matWorld_; }

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
	//ワールド変換行列
	Matrix4 matWorld_;
	RotType rotType;
	//クォータニオン
	Quaternion quaternion_ = { 0,1,0,0 };
private:
};
