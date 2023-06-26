#pragma once
#include "vector3.h"
#include "Util.h"
#include "myMath.h"
class SpotLight
{
public:
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		Vector3 lightv;		//�����x�N�g��
		float pad1;
		Vector3 lightPos;	//���W
		float pad2;
		Vector3 lightColor;	//�F
		float pad3;
		Vector3 lightatten;	//��������
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
	//���C�g�����i�P�ʃx�N�g���j
	Vector3 lightdir_ = { 1,0,0 };
	//���C�g���W
	Vector3 lightPos_ = { 0,0,0 };
	//���C�g�F
	Vector3 lightColor_ = { 1,1,1 };
	//���C�g���������W��
	Vector3 lightAtten_ = { 1.0f,1.0f,1.0f };
	//���C�g�����p�x�i�J�n�p�x�A�I���p�x�j
	Vector2 lightFactorAngleCos_ = { 0.5f,0.2f };
	//�L���t���O
	bool active_ = false;
};

