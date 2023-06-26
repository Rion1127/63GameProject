#pragma once
#include "Vector3.h"
#include "Util.h"
#include "myMath.h"
class CircleShadow
{
public:
	//�萔�o�b�t�@�p�f�[�^�\����
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
	void SetActive(bool active) {this->active_ = active;}

	Vector3 GetDir() { return dir_; }
	float GetDistanceCasterLight() { return ditanveCasterLight_; }
	Vector3 GetCasterPos() { return casterPos_; }
	Vector3 GetAtten() { return atten_; }
	Vector2 GetFactorAngleCos() { return factorAngleCos_; }
	bool GetActive() { return active_; }
	
private:
	//����
	Vector3 dir_ = { 1,0,0 };
	//�L���X�^�[�ƃ��C�g�̋���
	float ditanveCasterLight_ = 100.f;
	//�L���X�^�[���W�i���[���h���W�n�j
	Vector3 casterPos_ = { 0,0,0 };
	//���������W��
	Vector3 atten_ = { 0.5f,0.6f,0.0f };
	//�����p�x
	Vector2 factorAngleCos_ = { 0.2f,0.5f };
	//�L���t���O
	bool active_ = false;
};

