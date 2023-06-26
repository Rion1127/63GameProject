#include "LightGroup.h"
#include <cassert>


void LightGroup::StaticInit()
{
	
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
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�̃}�b�s���O
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {

		constMap->ambientColor = ambientColor_;
		//���s����
		for (uint32_t i = 0; i < sDirLightNum; i++) {
			//���C�g���L���Ȃ�ݒ��]��
			if (dirLights_[i].IsAvtive()) {
				constMap->dirLights[i].active = true;
				constMap->dirLights[i].lightv = -dirLights_[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights_[i].GetLightColor();
			}
			//���C�g�������Ȃ�]�����Ȃ�
			else {
				constMap->dirLights[i].active = false;
			}
		}
		//�_����
		for (uint32_t i = 0; i < sPointLightNum; i++) {
			//���C�g���L���Ȃ�ݒ��]��
			if (pointLights_[i].GetActive()) {
				constMap->pointLights_[i].active = true;
				constMap->pointLights_[i].lightPos = pointLights_[i].GetLightPos();
				constMap->pointLights_[i].lightColor = pointLights_[i].GetLightColor();
				constMap->pointLights_[i].lightatten = pointLights_[i].GetLightAtten();
			}
			//���C�g�������Ȃ烉�C�g�F��0��
			else {
				constMap->pointLights_[i].active = false;
			}
		}
		//�X�|�b�g���C�g
		for (uint32_t i = 0; i < sSpotLightNum; i++) {
			//�L���Ȃ�ݒ��]��
			if (spotLights_[i].GetActive()) {
				constMap->spotLights[i].active = true;
				constMap->spotLights[i].lightv = -spotLights_[i].GetLightDir().normalize();
				constMap->spotLights[i].lightPos = spotLights_[i].GetLightPos();
				constMap->spotLights[i].lightColor = spotLights_[i].GetLightColor();
				constMap->spotLights[i].lightatten = spotLights_[i].GetLightAtten();
				constMap->spotLights[i].lightfactorranglecos =
					spotLights_[i].GetLightFaactorAngleCos();
			}
			//���C�g�������Ȃ烉�C�g�F���O��
			else {
				constMap->spotLights[i].active = false;
			}
		}
		//�ۉe
		for (uint32_t i = 0; i < sCircleShadowNum; i++) {
			//�L���Ȃ�ݒ��]��
			if (circleShadows_[i].GetActive()) {
				constMap->circleShadows[i].active = true;
				constMap->circleShadows[i].dir = -circleShadows_[i].GetDir().normalize();
				constMap->circleShadows[i].casterPos = circleShadows_[i].GetCasterPos();
				constMap->circleShadows[i].distanceCasterLight = 
					circleShadows_[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten = circleShadows_[i].GetAtten();
				constMap->circleShadows[i].factorAngleCos =
					circleShadows_[i].GetFactorAngleCos();
			}
			//�����Ȃ�F��0��
			else {
				constMap->circleShadows[i].active = false;
			}
		}

		constBuff_->Unmap(0, nullptr);
	}
}

void LightGroup::SetAmbientColor(const Vector3& color)
{
	ambientColor_ = color;
	dirty_ = true;
}
#pragma region ���s����
void LightGroup::SetDirLightActive(uint32_t index, bool active)
{
	assert(0 <= index && index < sDirLightNum);
	dirLights_[index].SetActive(active);
}

void LightGroup::SetDirLightDir(uint32_t index, const Vector3& lightdir)
{
	assert(0 <= index && index < sDirLightNum);
	dirLights_[index].SetLightDir(lightdir);
	dirty_ = true;
}

void LightGroup::SetDirLightColor(uint32_t index, const Vector3& lightColor)
{
	assert(0 <= index && index < sDirLightNum);
	dirLights_[index].SetLightColor(lightColor);
	dirty_ = true;
}
#pragma endregion
#pragma region �_����
void LightGroup::SetPointLightActive(uint32_t index, bool active)
{
	assert(0 <= index && index < sDirLightNum);
	pointLights_[index].SetActive(active);
}

void LightGroup::SetPointLightPos(uint32_t index, const Vector3& lightPos)
{
	assert(0 <= index && index < sDirLightNum);
	pointLights_[index].SetLightPos(lightPos);
	dirty_ = true;
}

void LightGroup::SetPointLightColor(uint32_t index, const Vector3& lightColor)
{
	assert(0 <= index && index < sDirLightNum);
	pointLights_[index].SetLightColor(lightColor);
	dirty_ = true;
}

void LightGroup::SetPointLightAtten(uint32_t index, const Vector3& lightAtten)
{
	assert(0 <= index && index < sDirLightNum);
	pointLights_[index].SetLightAtten(lightAtten);
	dirty_ = true;
}
#pragma endregion
#pragma region �X�|�b�g���C�g
void LightGroup::SetSpotLightActive(uint32_t index, bool active)
{
	assert(0 <= index && index < sSpotLightNum);

	spotLights_[index].SetActive(active);
}
void LightGroup::SetSpotLightDir(uint32_t index, const Vector3& lightdir)
{
	assert(0 <= index && index < sSpotLightNum);

	spotLights_[index].SetLightDir(lightdir);
	dirty_ = true;
}
void LightGroup::SetSpotLightPos(uint32_t index, const Vector3& lightPos)
{
	assert(0 <= index && index < sSpotLightNum);

	spotLights_[index].SetLightPos(lightPos);
	dirty_ = true;
}
void LightGroup::SetSpotLightColor(uint32_t index, const Vector3& lightColor)
{
	assert(0 <= index && index < sSpotLightNum);

	spotLights_[index].SetLightColor(lightColor);
	dirty_ = true;
}
void LightGroup::SetSpotLightAtten(uint32_t index, const Vector3& lightAtten)
{
	assert(0 <= index && index < sSpotLightNum);

	spotLights_[index].SetLightAtten(lightAtten);
	dirty_ = true;
}
void LightGroup::SetSpotLightFactorAngle(uint32_t index, const Vector2& lightFactorAngle)
{
	assert(0 <= index && index < sSpotLightNum);

	spotLights_[index].SetLightFactorAngle(lightFactorAngle);
	dirty_ = true;
}
#pragma endregion
#pragma region �ۉe
void LightGroup::SetCircleShadowActive(uint32_t index, bool active)
{
	assert(0 <= index && index < sSpotLightNum);

	circleShadows_[index].SetActive(active);
}
void LightGroup::SetCircleShadowCasterPos(uint32_t index, const Vector3& casterPos)
{
	assert(0 <= index && index < sSpotLightNum);

	circleShadows_[index].SetCasterPos(casterPos);
	dirty_ = true;
}
void LightGroup::SetCircleShadowDir(uint32_t index, const Vector3& lightdir)
{
	assert(0 <= index && index < sSpotLightNum);

	circleShadows_[index].SetDir(lightdir);
	dirty_ = true;
}
void LightGroup::SetCircleShadowDistanceCasterLight(uint32_t index, float ditanceCasterLight)
{
	assert(0 <= index && index < sSpotLightNum);

	circleShadows_[index].SetDistanceCasterLight(ditanceCasterLight);
	dirty_ = true;
}
void LightGroup::SetCircleShadowAtten(uint32_t index, const Vector3& lightAtten)
{
	assert(0 <= index && index < sSpotLightNum);

	circleShadows_[index].SetAtten(lightAtten);
	dirty_ = true;
}
void LightGroup::SetCircleShadowFactorAngle(uint32_t index, const Vector2& lightFactorAngle)
{
	assert(0 <= index && index < sSpotLightNum);

	circleShadows_[index].SetFactorAngle(lightFactorAngle);
	dirty_ = true;
}
#pragma endregion
void LightGroup::DefaultLightSetting()
{
	dirLights_[0].SetActive(true);
	dirLights_[0].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[0].SetLightDir({ 0.0f,-1.0f,0.0f });

	dirLights_[1].SetActive(true);
	dirLights_[1].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[1].SetLightDir({ 0.5f,0.1f,0.2f });

	dirLights_[2].SetActive(true);
	dirLights_[2].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[2].SetLightDir({ -0.5f,0.1f,-0.2f });
}

void LightGroup::Update()
{
	//�l�̍X�V���������������萔�o�b�t�@�ɓ]������
	if (dirty_) {
		TransferConstBuffer();
		dirty_ = false;
	}
}

void LightGroup::Draw(UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff_->GetGPUVirtualAddress());
}
