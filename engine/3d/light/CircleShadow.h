#pragma once
#include "Vector3.h"
#include "Util.h"
#include "myMath.h"
class CircleShadow
{
public:
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		Vector3 dir;
		float pad1;
		Vector3 casterPos;
		//float pad2;
		float distanceCasterLight;
		Vector3 atten;
		float pad3;
		Vector2 factorAngleCos;
		uint32_t active;
		float pad4;
	};

	void SetDir(const Vector3& dir) { this->dir_ = dir; }
	void SetCasterPos(const Vector3& casterPos) { this->casterPos_ = casterPos; }
	void SetDistanceCasterLight(const float& distance) { ditanveCasterLight_ = distance; }
	void SetAtten(const Vector3& atten) { this->atten_ = atten; }
	void SetFactorAngle(const Vector2& angle) {
		this->factorAngleCos_.x = cosf(Radian(angle.x));
		this->factorAngleCos_.y = cosf(Radian(angle.y));
	}
	void SetActive(bool active) { this->active_ = active; }

	Vector3 GetDir() { return dir_; }
	float GetDistanceCasterLight() { return ditanveCasterLight_; }
	Vector3 GetCasterPos() { return casterPos_; }
	Vector3 GetAtten() { return atten_; }
	Vector2 GetFactorAngleCos() { return factorAngleCos_; }
	bool GetActive() { return active_; }

private:
	//方向
	Vector3 dir_ = { 0,-1,0 };
	//キャスターとライトの距離
	float ditanveCasterLight_ = 100.f;
	//キャスター座標（ワールド座標系）
	Vector3 casterPos_ = { 0,0,0 };
	//距離減衰係数
	Vector3 atten_ = { 0.5f,0.6f,0.0f };
	//減衰角度
	Vector2 factorAngleCos_ = { 0.2f,0.5f };
	//有効フラグ
	bool active_ = false;
};

