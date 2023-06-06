#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include "DirectX.h"
#include "myMath.h"

// 定数バッファ用データ構造体
struct ConstVPBuff {
	DirectX::XMMATRIX view;       // ワールド → ビュー変換行列
	DirectX::XMMATRIX projection; // ビュー → プロジェクション変換行列
	DirectX::XMFLOAT3 cameraPos;  // カメラ座標（ワールド座標）
};

class Camera {
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

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

	XMMATRIX GetMatView();
	XMMATRIX GetMatProjection();
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 up;

	XMMATRIX matView{};
	//透視投影行列の計算
	XMMATRIX matProjection{};

	//ビルボード行列
	XMMATRIX matBillboard;
	//Y軸周りのビルボード
	XMMATRIX matBillboardY;

	static Camera current;

	/// <summary>
	/// カメラシェイク
	/// </summary>
	/// <param name="time">揺れる時間</param>
	/// <param name="power">揺れる大きさ</param>
	void ShakeSet(int time, float power);
	void ShakeUpdate();
	void SetOriginalPos();
	bool GetIsShake() { return isShake; }
private:

	// ビューポートのアスペクト比
	const float aspectRatio = (float)16 / 9;

	DirectXCommon* directX_ = nullptr;


	//シェイクする前の場所
	XMFLOAT3 originalPos;
	bool isShake = false;
	int maxShakeTime;
	int shakeTime = 0;
	float power_;
};

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3 v1, const DirectX::XMFLOAT3 v2);
