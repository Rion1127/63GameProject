#pragma once
#include "Vector3.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"
#include "DirectX.h"
#include <d3d12.h>
class LightGroup
{
private:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const uint32_t sDirLightNum = 3;
	static const uint32_t sPointLightNum = 3;
	static const uint32_t sSpotLightNum = 3;
	static const uint32_t sCircleShadowNum = 1;
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		//�����̐F
		Vector3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[sDirLightNum];
		//�_�����p
		PointLight::ConstBufferData pointLights_[sPointLightNum];
		//�_�����p
		SpotLight::ConstBufferData spotLights[sSpotLightNum];
		//�ۉe
		CircleShadow::ConstBufferData circleShadows[sCircleShadowNum];
	};

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;
	//�����̐F
	Vector3 ambientColor_ = { 1,1,1 };
	//���s�����̔z��
	DirectionalLight dirLights_[sDirLightNum];
	//�_�����̔z��
	PointLight pointLights_[sPointLightNum];
	//�X�|�b�g���C�g
	SpotLight spotLights_[sSpotLightNum];
	//�ۉe
	CircleShadow circleShadows_[sCircleShadowNum];

	//�_�[�e�B�t���O
	bool dirty_ = false;
public:
	static void StaticInit();
	//�C���X�^���X����
	static LightGroup* Create();

	//������
	void Init();
	//�萔�o�b�t�@�]��
	void TransferConstBuffer();

	void SetAmbientColor(const Vector3& color);
#pragma region ���s����
	/// <summary>
	/// ���s�����̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="acrive">�L���t���O</param>
	void SetDirLightActive(uint32_t index, bool active);
	/// <summary>
	/// ���s�����̃��C�g�������Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetDirLightDir(uint32_t index, const Vector3& lightdir);
	/// <summary>
	/// ���s�����̃J���[���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g�F</param>
	void SetDirLightColor(uint32_t index, const Vector3& lightColor);
#pragma endregion
#pragma region �_����
	/// <summary>
	/// �_�����̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="acrive">�L���t���O</param>
	void SetPointLightActive(uint32_t index, bool active);
	/// <summary>
	/// �_�����̍��W���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetPointLightPos(uint32_t index, const Vector3& lightPos);
	/// <summary>
	/// �_�����̃J���[���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g�F</param>
	void SetPointLightColor(uint32_t index, const Vector3& lightColor);
	/// <summary>
	/// �_�����̋����������Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g�F</param>
	void SetPointLightAtten(uint32_t index, const Vector3& lightAtten);
#pragma endregion
#pragma region �X�|�b�g���C�g
	void SetSpotLightActive(uint32_t index, bool active);
	void SetSpotLightDir(uint32_t index, const Vector3& lightdir);
	void SetSpotLightPos(uint32_t index, const Vector3& lightPos);
	void SetSpotLightColor(uint32_t index, const Vector3& lightColor);
	void SetSpotLightAtten(uint32_t index, const Vector3& lightAtten);
	void SetSpotLightFactorAngle(uint32_t index, const Vector2& lightFactorAngle);
#pragma endregion
#pragma region �ۉe
	void SetCircleShadowActive(uint32_t index, bool active);
	void SetCircleShadowCasterPos(uint32_t index, const Vector3& casterPos);
	void SetCircleShadowDir(uint32_t index, const Vector3& lightdir);
	void SetCircleShadowDistanceCasterLight(uint32_t index, float ditanceCasterLight);
	void SetCircleShadowAtten(uint32_t index, const Vector3& lightAtten);
	void SetCircleShadowFactorAngle(uint32_t index, const Vector2& lightFactorAngle);
#pragma endregion
	/// <summary>
	/// �W���̃��C�g�ݒ�
	/// </summary>
	void DefaultLightSetting();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	void Draw(UINT rootParameterIndex);
};

