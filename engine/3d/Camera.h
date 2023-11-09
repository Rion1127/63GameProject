#pragma once

#include <d3d12.h>

#include "DirectX.h"
#include "myMath.h"

/**
 * @file Camera.h
 * @brief カメラのトランスフォームを管理している
 */

// 定数バッファ用データ構造体
struct ConstVPBuff {
	Matrix4 view;       // ワールド → ビュー変換行列
	Matrix4 projection; // ビュー → プロジェクション変換行列
	Vector3 cameraPos;  // カメラ座標（ワールド座標）
};

enum class CameraMode {
	LookAT,	//座標と注視点座標
	LookTo	//座標とオイラー角
};

class Camera {
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Camera();
	//カメラ座標
	void SetEyePos(float x, float y, float z);
	void SetEyePos(const Vector3& pos);
	//ターゲットセット
	void SetTarget(float x, float y, float z);
	void SetTarget(const Vector3& pos);
	//アップベクトル
	void SetUpVec(float x, float y, float z);
	void SetUpVec(const Vector3& upVec);
	void MoveTo(const Vector3& goal, float speed);
	//座標と注視点座標で行列計算
	void UpdateLookAt();
	//座標とオイラー角で行列計算(こっちはデバッグカメラが対応していないため、バグる)
	void UpdateLookTo();

	void Update(CameraMode mode);

	Vector3 eye_;
	Vector3 target_;
	Vector3 up_;
	Vector3 rot_;

	Matrix4 matView_{};
	//透視投影行列の計算
	Matrix4 matProjection_{};
	//ビルボード行列
	Matrix4 matBillboard_;
	//Y軸周りのビルボード
	Matrix4 matBillboardY_;

	static Camera* scurrent_;
public:
	/// <summary>
	/// カメラシェイク
	/// </summary>
	/// <param name="time">揺れる時間</param>
	/// <param name="power">揺れる大きさ</param>
	void ShakeSet(uint32_t time, float power);
	void ShakeUpdate();
	void SetOriginalPos();
	bool GetIsShake() { return isShake_; }
public:
	Matrix4 GetMatView();
	Matrix4 GetMatProjection();

private:
	void UpdateMatProjection();
	// ビューポートのアスペクト比
	float aspectRatio_;

	//シェイクする前の場所
	Vector3 originalPos_;
	bool isShake_ = false;
	uint32_t maxShakeTime_;
	uint32_t shakeTime_ = 0;
	float power_;
};