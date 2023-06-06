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
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const int DirLightNum = 3;
	static const int PointLightNum = 3;
	static const int SpotLightNum = 3;
	static const int CircleShadowNum = 1;

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
	void SetDirLightActive(int index, bool active);
	/// <summary>
	/// ���s�����̃��C�g�������Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetDirLightDir(int index, const Vector3& lightdir);
	/// <summary>
	/// ���s�����̃J���[���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g�F</param>
	void SetDirLightColor(int index, const Vector3& lightColor);
#pragma endregion
#pragma region �_����
	/// <summary>
	/// �_�����̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="acrive">�L���t���O</param>
	void SetPointLightActive(int index, bool active);
	/// <summary>
	/// �_�����̍��W���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetPointLightPos(int index, const Vector3& lightPos);
	/// <summary>
	/// �_�����̃J���[���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g�F</param>
	void SetPointLightColor(int index, const Vector3& lightColor);
	/// <summary>
	/// �_�����̋����������Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g�F</param>
	void SetPointLightAtten(int index, const Vector3& lightAtten);
#pragma endregion
#pragma region �X�|�b�g���C�g
	void SetSpotLightActive(int index, bool active);
	void SetSpotLightDir(int index, const Vector3& lightdir);
	void SetSpotLightPos(int index, const Vector3& lightPos);
	void SetSpotLightColor(int index, const Vector3& lightColor);
	void SetSpotLightAtten(int index, const Vector3& lightAtten);
	void SetSpotLightFactorAngle(int index, const Vector2& lightFactorAngle);
#pragma endregion
#pragma region �ۉe
	void SetCircleShadowActive(int index, bool active);
	void SetCircleShadowCasterPos(int index, const Vector3& casterPos);
	void SetCircleShadowDir(int index, const Vector3& lightdir);
	void SetCircleShadowDistanceCasterLight(int index, float ditanceCasterLight);
	void SetCircleShadowAtten(int index, const Vector3& lightAtten);
	void SetCircleShadowFactorAngle(int index, const Vector2& lightFactorAngle);
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

private:
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		//�����̐F
		Vector3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		//�_�����p
		PointLight::ConstBufferData pointLights[PointLightNum];
		//�_�����p
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		//�ۉe
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};
	static DirectXCommon* directX;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�����̐F
	Vector3 ambientColor = { 1,1,1 };
	//���s�����̔z��
	DirectionalLight dirLights[DirLightNum];
	//�_�����̔z��
	PointLight pointLights[PointLightNum];
	//�X�|�b�g���C�g
	SpotLight spotLights[SpotLightNum];
	//�ۉe
	CircleShadow circleShadows[CircleShadowNum];

	//�_�[�e�B�t���O
	bool dirty = false;
};

