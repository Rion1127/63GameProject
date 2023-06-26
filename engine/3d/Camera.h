#pragma once

#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include "DirectX.h"
#include "myMath.h"

// 定数バッファ用データ構造体
struct ConstVPBuff {
	Matrix4 view;       // ワールド → ビュー変換行列
	Matrix4 projection; // ビュー → プロジェクション変換行列
	Vector3 cameraPos;  // カメラ座標（ワールド座標）
};

class Camera {
private:
	// ビューポートのアスペクト比
	float aspectRatio_;

	//シェイクする前の場所
	Vector3 originalPos_;
	bool isShake_ = false;
	uint32_t maxShakeTime_;
	uint32_t shakeTime_ = 0;
	float power_;
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Vector3 eye_;
	Vector3 target_;
	Vector3 up_;

	Matrix4 matView_{};
	//透視投影行列の計算
	Matrix4 matProjection_{};

	//ビルボード行列
	Matrix4 matBillboard_;
	//Y軸周りのビルボード
	Matrix4 matBillboardY_;

	static Camera scurrent_;
public:
	Camera();
	//カメラ座標
	void SetEyePos(float x,float y,float z);
	void SetEyePos(Vector3 pos);
	//ターゲットセット
	void SetTarget(float x, float y, float z);
	void SetTarget(Vector3 pos);
	//アップベクトル
	void SetUpVec(float x, float y, float z);
	void SetUpVec(Vector3 upVec);
	void MoveTo(Vector3 goal, float speed);
	//初期化
	void Update();

	Matrix4 GetMatView();
	Matrix4 GetMatProjection();

	/// <summary>
	/// カメラシェイク
	/// </summary>
	/// <param name="time">揺れる時間</param>
	/// <param name="power">揺れる大きさ</param>
	void ShakeSet(uint32_t time, float power);
	void ShakeUpdate();
	void SetOriginalPos();
	bool GetIsShake() { return isShake_; }
};

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 v1, const DirectX::XMFLOAT3 v2);
