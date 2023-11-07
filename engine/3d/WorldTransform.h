#pragma once
#include "math.h"
#include <d3d12.h>
#include <wrl.h>

#include "DirectX.h"
#include "Quaternion.h"
#include "myMath.h"
#include "Camera.h"

/**
 * @file WorldTransform.h
 * @brief 座標・スケール・回転を管理しているクラス
 */

//定数バッファ用データ構造体（３D変換行列）
struct ConstBufferDataTransform {
	Matrix4 mat; //3D変換行列
	Matrix4 viewProj;	//ビューとプロジェクション合成行列
	Vector3 cameraPos;
};

enum class BillBoard {
	None,
	BillBoard,
	AxisYBillBoard
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

	void SetScale(const Vector3& scale) { scale_ = scale; }
	void SetRotation(const Vector3& rot) { rotation_ = rot; }
	void SetPosition(const Vector3& pos) { position_ = pos; }
	void SetQuaternion(const Quaternion& q) { quaternion_ = q; }
	void SetRotType(const RotType& type) { rotType = type; }

	void AddScale(float x, float y, float z);
	void AddRotation(float x, float y, float z);
	void AddPosition(float x, float y, float z);

	void AddScale(const Vector3& scale) { scale_ += scale; }
	void AddRotation(const Vector3& rot) { rotation_ += rot; }
	void AddPosition(const Vector3& pos) { position_ += pos; }
	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	/// <param name="viewProjection">カメラ</param>
	/// <param name="isBillboard">0 = 無し; 1 = ビルボードあり; 2 = Y軸ビルボード</param>
	void Update(BillBoard isBillboard = BillBoard::None, Camera* camera = Camera::scurrent_);

	Matrix4 GetMatWorld() { return matWorld_; }
	Vector3 GetWorldPos() { return Vector3(matWorld_.m[3][0], matWorld_.m[3][1], matWorld_.m[3][2]); }

	//定数バッファ（行列用）
	ComPtr<ID3D12Resource> constBuffTransform_;
	//定数バッファマップ（行列用）
	ConstBufferDataTransform* constMapTransform_;
	//親オブジェクトへのポインタ
	WorldTransform* parent_ = nullptr;
	Matrix4* parentRotMat_ = nullptr;
	Matrix4* parentPosMat_ = nullptr;

	//アフィン変換情報
	Vector3 scale_ = { 1,1,1 };
	Vector3 rotation_ = { 0,0,0 };
	Vector3 position_ = { 0,0,0 };
	//ワールド変換行列
	Matrix4 scaleMat_;
	Matrix4 rotMat_;
	Matrix4 posMat_;
	Matrix4 matWorld_;
	RotType rotType;
	//クォータニオン
	Quaternion quaternion_ = { 0,1,0,0 };
private:
};
