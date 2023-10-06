#pragma once
#include "Vector3.h"
#include <stdint.h>
class PointLight
{
public:

	struct ConstBufferData
	{
		Vector3 lightPos;
		float pad1;
		Vector3 lightColor;
		float pad2;
		Vector3 lightatten;
		uint32_t active;
	};
	//座標代入
	void SetLightPos(const Vector3& lightPos) { this->lightPos_ = lightPos; }
	//座標取得
	Vector3& GetLightPos() { return lightPos_; }
	//色代入
	void SetLightColor(const Vector3& lightColor) { this->lightColor_ = lightColor; }
	//色取得
	Vector3& GetLightColor() { return lightColor_; }
	//距離減衰代入
	void SetLightAtten(const Vector3& lightAtten) { this->lightAtten_ = lightAtten; }
	//距離減衰取得
	Vector3& GetLightAtten() { return lightAtten_; }
	//有効フラグ
	void SetActive(bool active) { this->active_ = active; }
	bool GetActive() { return active_; }
private:
	//ライト座標
	Vector3 lightPos_ = { 0,0,0 };
	//ライト色
	Vector3 lightColor_ = { 1,1,1 };
	//ライト距離減衰係数
	Vector3 lightAtten_ = { 1.0f,1.0f,1.0f };
	//有効フラグ
	bool active_ = false;
};

