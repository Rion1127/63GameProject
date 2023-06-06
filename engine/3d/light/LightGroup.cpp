#include "LightGroup.h"
#include <cassert>

DirectXCommon* LightGroup::directX = nullptr;

void LightGroup::StaticInit()
{
	directX = DirectXCommon::GetInstance();
}

LightGroup* LightGroup::Create()
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	LightGroup* instance = new LightGroup();

	//������
	instance->Init();

	return instance;
}

void LightGroup::Init()
{
	//�W���̃��C�g�̐ݒ�
	DefaultLightSetting();

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	HRESULT result;
	// �萔�o�b�t�@�̐���
	result = directX->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�̃}�b�s���O
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {

		constMap->ambientColor = ambientColor;
		//���s����
		for (int i = 0; i < DirLightNum; i++) {
			//���C�g���L���Ȃ�ݒ��]��
			if (dirLights[i].IsAvtive()) {
				constMap->dirLights[i].active = true;
				constMap->dirLights[i].lightv = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights[i].GetLightColor();
			}
			//���C�g�������Ȃ�]�����Ȃ�
			else {
				constMap->dirLights[i].active = false;
			}
		}
		//�_����
		for (int i = 0; i < PointLightNum; i++) {
			//���C�g���L���Ȃ�ݒ��]��
			if (pointLights[i].GetActive()) {
				constMap->pointLights[i].active = true;
				constMap->pointLights[i].lightPos = pointLights[i].GetLightPos();
				constMap->pointLights[i].lightColor = pointLights[i].GetLightColor();
				constMap->pointLights[i].lightatten = pointLights[i].GetLightAtten();
			}
			//���C�g�������Ȃ烉�C�g�F��0��
			else {
				constMap->pointLights[i].active = false;
			}
		}
		//�X�|�b�g���C�g
		for (int i = 0; i < SpotLightNum; i++) {
			//�L���Ȃ�ݒ��]��
			if (spotLights[i].GetActive()) {
				constMap->spotLights[i].active = true;
				constMap->spotLights[i].lightv = -spotLights[i].GetLightDir().normalize();
				constMap->spotLights[i].lightPos = spotLights[i].GetLightPos();
				constMap->spotLights[i].lightColor = spotLights[i].GetLightColor();
				constMap->spotLights[i].lightatten = spotLights[i].GetLightAtten();
				constMap->spotLights[i].lightfactorranglecos =
					spotLights[i].GetLightFaactorAngleCos();
			}
			//���C�g�������Ȃ烉�C�g�F���O��
			else {
				constMap->spotLights[i].active = false;
			}
		}
		//�ۉe
		for (int i = 0; i < CircleShadowNum; i++) {
			//�L���Ȃ�ݒ��]��
			if (circleShadows[i].GetActive()) {
				constMap->circleShadows[i].active = true;
				constMap->circleShadows[i].dir = -circleShadows[i].GetDir().normalize();
				constMap->circleShadows[i].casterPos = circleShadows[i].GetCasterPos();
				constMap->circleShadows[i].distanceCasterLight = 
					circleShadows[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten = circleShadows[i].GetAtten();
				constMap->circleShadows[i].factorAngleCos =
					circleShadows[i].GetFactorAngleCos();
			}
			//�����Ȃ�F��0��
			else {
				constMap->circleShadows[i].active = false;
			}
		}

		constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::SetAmbientColor(const Vector3& color)
{
	ambientColor = color;
	dirty = true;
}
#pragma region ���s����
void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetActive(active);
}

void LightGroup::SetDirLightDir(int index, const Vector3& lightdir)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightDir(lightdir);
	dirty = true;
}

void LightGroup::SetDirLightColor(int index, const Vector3& lightColor)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightColor(lightColor);
	dirty = true;
}
#pragma endregion
#pragma region �_����
void LightGroup::SetPointLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);
	pointLights[index].SetActive(active);
}

void LightGroup::SetPointLightPos(int index, const Vector3& lightPos)
{
	assert(0 <= index && index < DirLightNum);
	pointLights[index].SetLightPos(lightPos);
	dirty = true;
}

void LightGroup::SetPointLightColor(int index, const Vector3& lightColor)
{
	assert(0 <= index && index < DirLightNum);
	pointLights[index].SetLightColor(lightColor);
	dirty = true;
}

void LightGroup::SetPointLightAtten(int index, const Vector3& lightAtten)
{
	assert(0 <= index && index < DirLightNum);
	pointLights[index].SetLightAtten(lightAtten);
	dirty = true;
}
#pragma endregion
#pragma region �X�|�b�g���C�g
void LightGroup::SetSpotLightActive(int index, bool active)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetActive(active);
}
void LightGroup::SetSpotLightDir(int index, const Vector3& lightdir)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightDir(lightdir);
	dirty = true;
}
void LightGroup::SetSpotLightPos(int index, const Vector3& lightPos)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightPos(lightPos);
	dirty = true;
}
void LightGroup::SetSpotLightColor(int index, const Vector3& lightColor)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightColor(lightColor);
	dirty = true;
}
void LightGroup::SetSpotLightAtten(int index, const Vector3& lightAtten)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightAtten(lightAtten);
	dirty = true;
}
void LightGroup::SetSpotLightFactorAngle(int index, const Vector2& lightFactorAngle)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightFactorAngle(lightFactorAngle);
	dirty = true;
}
#pragma endregion
#pragma region �ۉe
void LightGroup::SetCircleShadowActive(int index, bool active)
{
	assert(0 <= index && index < SpotLightNum);

	circleShadows[index].SetActive(active);
}
void LightGroup::SetCircleShadowCasterPos(int index, const Vector3& casterPos)
{
	assert(0 <= index && index < SpotLightNum);

	circleShadows[index].SetCasterPos(casterPos);
	dirty = true;
}
void LightGroup::SetCircleShadowDir(int index, const Vector3& lightdir)
{
	assert(0 <= index && index < SpotLightNum);

	circleShadows[index].SetDir(lightdir);
	dirty = true;
}
void LightGroup::SetCircleShadowDistanceCasterLight(int index, float ditanceCasterLight)
{
	assert(0 <= index && index < SpotLightNum);

	circleShadows[index].SetDistanceCasterLight(ditanceCasterLight);
	dirty = true;
}
void LightGroup::SetCircleShadowAtten(int index, const Vector3& lightAtten)
{
	assert(0 <= index && index < SpotLightNum);

	circleShadows[index].SetAtten(lightAtten);
	dirty = true;
}
void LightGroup::SetCircleShadowFactorAngle(int index, const Vector2& lightFactorAngle)
{
	assert(0 <= index && index < SpotLightNum);

	circleShadows[index].SetFactorAngle(lightFactorAngle);
	dirty = true;
}
#pragma endregion
void LightGroup::DefaultLightSetting()
{
	dirLights[0].SetActive(true);
	dirLights[0].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[0].SetLightDir({ 0.0f,-1.0f,0.0f });

	dirLights[1].SetActive(true);
	dirLights[1].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[1].SetLightDir({ 0.5f,0.1f,0.2f });

	dirLights[2].SetActive(true);
	dirLights[2].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[2].SetLightDir({ -0.5f,0.1f,-0.2f });
}

void LightGroup::Update()
{
	//�l�̍X�V���������������萔�o�b�t�@�ɓ]������
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void LightGroup::Draw(UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	directX->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}
