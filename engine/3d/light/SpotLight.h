#pragma once
#include "vector3.h"
#include "Util.h"
#include "myMath.h"
class SpotLight
{
public:
	//定数バッファ用データ構造体
	struct ConstBufferData {
		Vector3 lightv;		//方向ベクトル
		float pad1;
		Vector3 lightPos;	//座標
		float pad2;
		Vector3 lightColor;	//色
		float pad3;
		Vector3 lightatten;	//距離減衰
		float pad4;
		Vector2 lightfactorranglecos;
		uint32_t active;
		float pad5;
	};
	void SetLightDir(const Vector3& lightdir) { this->lightdir_ = lightdir; }
	void SetLightPos(const Vector3& lightPos) { this->lightPos_ = lightPos; }
	void SetLightColor(const Vector3& lightcolor) { this->lightColor_ = lightcolor; }
	void SetLightAtten(const Vector3& lightAtten) { this->lightAtten_ = lightAtten; }
	void SetLightFactorAngle(const Vector2& lightFactorAngle) {
		this->lightFactorAngleCos_.x = cosf(Radian(lightFactorAngle.x));
		this->lightFactorAngleCos_.y = cosf(Radian(lightFactorAngle.y));
	}
	void SetActive(bool active) { this->active_ = active; }
	bool GetActive() { return active_; }

	Vector3 GetLightDir() { return lightdir_; }
	Vector3 GetLightPos() { return lightPos_; }
	Vector3 GetLightColor() { return lightColor_; }
	Vector3 GetLightAtten() { return lightAtten_; }
	Vector2 GetLightFaactorAngleCos() { return lightFactorAngleCos_; }
private:
	//ライト方向（単位ベクトル）
	Vector3 lightdir_ = { 1,0,0 };
	//ライト座標
	Vector3 lightPos_ = { 0,0,0 };
	//ライト色
	Vector3 lightColor_ = { 1,1,1 };
	//ライト距離減衰係数
	Vector3 lightAtten_ = { 1.0f,1.0f,1.0f };
	//ライト減衰角度（開始角度、終了角度）
	Vector2 lightFactorAngleCos_ = { 0.5f,0.2f };
	//有効フラグ
	bool active_ = false;
};

