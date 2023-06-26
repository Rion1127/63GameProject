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
	//���W���
	void SetLightPos(const Vector3& lightPos) { this->lightPos_ = lightPos; }
	//���W�擾
	Vector3& GetLightPos() { return lightPos_; }
	//�F���
	void SetLightColor(const Vector3& lightColor) { this->lightColor_ = lightColor; }
	//�F�擾
	Vector3& GetLightColor() { return lightColor_; }
	//�����������
	void SetLightAtten(const Vector3& lightAtten) { this->lightAtten_ = lightAtten; }
	//���������擾
	Vector3& GetLightAtten() { return lightAtten_; }
	//�L���t���O
	void SetActive(bool active) { this->active_ = active; }
	bool GetActive() { return active_; }
private:
	//���C�g���W
	Vector3 lightPos_ = { 0,0,0 };
	//���C�g�F
	Vector3 lightColor_ = { 1,1,1 };
	//���C�g���������W��
	Vector3 lightAtten_ = { 1.0f,1.0f,1.0f };
	//�L���t���O
	bool active_ = false;
};

