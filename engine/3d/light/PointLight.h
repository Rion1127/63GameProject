#pragma once
#include "Vector3.h"
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
		unsigned int active;
	};
	//���W���
	void SetLightPos(const Vector3& lightPos) { this->lightPos = lightPos; }
	//���W�擾
	Vector3& GetLightPos() { return lightPos; }
	//�F���
	void SetLightColor(const Vector3& lightColor) { this->lightColor = lightColor; }
	//�F�擾
	Vector3& GetLightColor() { return lightColor; }
	//�����������
	void SetLightAtten(const Vector3& lightAtten) { this->lightAtten = lightAtten; }
	//���������擾
	Vector3& GetLightAtten() { return lightAtten; }
	//�L���t���O
	void SetActive(bool active) { this->active = active; }
	bool GetActive() { return active; }
private:
	//���C�g���W
	Vector3 lightPos = { 0,0,0 };
	//���C�g�F
	Vector3 lightColor = { 1,1,1 };
	//���C�g���������W��
	Vector3 lightAtten = { 1.0f,1.0f,1.0f };
	//�L���t���O
	bool active = false;
};

