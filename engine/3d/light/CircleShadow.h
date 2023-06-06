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
		unsigned int active;
		float pad4;
	};

	void SetDir(const Vector3& dir) { this->dir = dir; }
	void SetCasterPos(const Vector3& casterPos) { this->casterPos = casterPos; }
	void SetDistanceCasterLight(const float& distance) { ditanveCasterLight = distance; }
	void SetAtten(const Vector3& atten) { this->atten = atten; }
	void SetFactorAngle(const Vector2& angle) {
		this->factorAngleCos.x = cosf(ConvertAngleToRadian(angle.x));
		this->factorAngleCos.y = cosf(ConvertAngleToRadian(angle.y));
	}
	void SetActive(bool active) {this->active = active;}

	Vector3 GetDir() { return dir; }
	float GetDistanceCasterLight() { return ditanveCasterLight; }
	Vector3 GetCasterPos() { return casterPos; }
	Vector3 GetAtten() { return atten; }
	Vector2 GetFactorAngleCos() { return factorAngleCos; }
	bool GetActive() { return active; }
	
private:
	//����
	Vector3 dir = { 1,0,0 };
	//�L���X�^�[�ƃ��C�g�̋���
	float ditanveCasterLight = 100.f;
	//�L���X�^�[���W�i���[���h���W�n�j
	Vector3 casterPos = { 0,0,0 };
	//���������W��
	Vector3 atten = { 0.5f,0.6f,0.0f };
	//�����p�x
	Vector2 factorAngleCos = { 0.2f,0.5f };
	//�L���t���O
	bool active = false;
};

