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
	//���C�g�����i�P�ʃx�N�g���j
	Vector3 lightdir = { 1,0,0 };
	//���C�g���W
	Vector3 lightPos = { 0,0,0 };
	//���C�g�F
	Vector3 lightColor = { 1,1,1 };
	//���C�g���������W��
	Vector3 lightAtten = { 1.0f,1.0f,1.0f };
	//���C�g�����p�x�i�J�n�p�x�A�I���p�x�j
	Vector2 lightFactorAngleCos = { 0.5f,0.2f };
	//�L���t���O
	bool active = false;
};

