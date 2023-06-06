#include "LightGroup.h"
#include <cassert>

DirectXCommon* LightGroup::directX = nullptr;

void LightGroup::StaticInit()
{
	directX = DirectXCommon::GetInstance();
}

LightGroup* LightGroup::Create()
{
	//3Dオブジェクトのインスタンスを生成
	LightGroup* instance = new LightGroup();

	//初期化
	instance->Init();

	return instance;
}

void LightGroup::Init()
{
	//標準のライトの設定
	DefaultLightSetting();

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	HRESULT result;
	// 定数バッファの生成
	result = directX->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	// 定数バッファのマッピング
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {

		constMap->ambientColor = ambientColor;
		//平行根源
		for (int i = 0; i < DirLightNum; i++) {
			//ライトが有効なら設定を転送
			if (dirLights[i].IsAvtive()) {
				constMap->dirLights[i].active = true;
				constMap->dirLights[i].lightv = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights[i].GetLightColor();
			}
			//ライトが無効なら転送しない
			else {
				constMap->dirLights[i].active = false;
			}
		}
		//点光源
		for (int i = 0; i < PointLightNum; i++) {
			//ライトが有効なら設定を転送
			if (pointLights[i].GetActive()) {
				constMap->pointLights[i].active = true;
				constMap->pointLights[i].lightPos = pointLights[i].GetLightPos();
				constMap->pointLights[i].lightColor = pointLights[i].GetLightColor();
				constMap->pointLights[i].lightatten = pointLights[i].GetLightAtten();
			}
			//ライトが無効ならライト色を0に
			else {
				constMap->pointLights[i].active = false;
			}
		}
		//スポットライト
		for (int i = 0; i < SpotLightNum; i++) {
			//有効なら設定を転送
			if (spotLights[i].GetActive()) {
				constMap->spotLights[i].active = true;
				constMap->spotLights[i].lightv = -spotLights[i].GetLightDir().normalize();
				constMap->spotLights[i].lightPos = spotLights[i].GetLightPos();
				constMap->spotLights[i].lightColor = spotLights[i].GetLightColor();
				constMap->spotLights[i].lightatten = spotLights[i].GetLightAtten();
				constMap->spotLights[i].lightfactorranglecos =
					spotLights[i].GetLightFaactorAngleCos();
			}
			//ライトが無効ならライト色を０に
			else {
				constMap->spotLights[i].active = false;
			}
		}
		//丸影
		for (int i = 0; i < CircleShadowNum; i++) {
			//有効なら設定を転送
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
			//無効なら色を0に
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
#pragma region 平行光源
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
#pragma region 点光源
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
#pragma region スポットライト
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
#pragma region 丸影
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
	//値の更新があった時だけ定数バッファに転送する
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void LightGroup::Draw(UINT rootParameterIndex)
{
	//定数バッファビューをセット
	directX->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}
