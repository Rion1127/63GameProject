#pragma once
#include "LightGroup.h"
#include "mInput.h"
class LightManager
{
private:
	std::shared_ptr<LightGroup> lightGroup;
private:
	enum LIGHTTYPE {
		DIRECTION_,
		POINT_,
		SPOT_,
		NUMEND_
	};
	//���C�g�̃^�C�v
	uint32_t lightType_ = DIRECTION_;
private:
	float ambientColor0[3] = { 1,1,1 };

	//���������������l
	float lightDir0[3] = { 0,-1,0 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//�_����
	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };

	//�X�|�b�g���C�g
	float spotlightDir[3] = { 0,-1,0 };
	float spotlightPos[3] = { 0,5,0 };
	float spotLightColor[3] = { 1,0,1 };
	float spotLightAtten[3] = { 0.0f,0.0f, 0.0f };
	float spotLightFactorAngle[2] = { 20.f,30.f };

	//�ۉe
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f,0.5f };

	float fighterPos[3] = { 1,0.0f,0 };
private:
	void DirectionalLightUpdate();
	void PointLightUpdate();
	void SpotLightUpdate();
public:
	LightManager();
	void DebugUpdate();

	void Update();

	std::shared_ptr<LightGroup> GetLightGroup() { return lightGroup; }
};

