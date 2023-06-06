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
		unsigned int active;
		float pad5;
	};
	void SetLightDir(const Vector3& lightdir) { this->lightdir = lightdir; }
	void SetLightPos(const Vector3& lightPos) { this->lightPos = lightPos; }
	void SetLightColor(const Vector3& lightcolor) { this->lightColor = lightcolor; }
	void SetLightAtten(const Vector3& lightAtten) { this->lightAtten = lightAtten; }
	void SetLightFactorAngle(const Vector2& lightFactorAngle) {
		this->lightFactorAngleCos.x = cosf(ConvertAngleToRadian(lightFactorAngle.x));
		this->lightFactorAngleCos.y = cosf(ConvertAngleToRadian(lightFactorAngle.y));
	}
	void SetActive(bool active) { this->active = active; }
	bool GetActive() { return active; }

	Vector3 GetLightDir() { return lightdir; }
	Vector3 GetLightPos() { return lightPos; }
	Vector3 GetLightColor() { return lightColor; }
	Vector3 GetLightAtten() { return lightAtten; }
	Vector2 GetLightFaactorAngleCos() { return lightFactorAngleCos; }
private:
	//ライト方向（単位ベクトル）
	Vector3 lightdir = { 1,0,0 };
	//ライト座標
	Vector3 lightPos = { 0,0,0 };
	//ライト色
	Vector3 lightColor = { 1,1,1 };
	//ライト距離減衰係数
	Vector3 lightAtten = { 1.0f,1.0f,1.0f };
	//ライト減衰角度（開始角度、終了角度）
	Vector2 lightFactorAngleCos = { 0.5f,0.2f };
	//有効フラグ
	bool active = false;
};

