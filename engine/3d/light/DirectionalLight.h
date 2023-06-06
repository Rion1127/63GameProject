#pragma once
#include "Vector3.h"
#include "DirectX.h"
#include <d3d12.h>
class DirectionalLight
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static DirectionalLight* Create();

	static void StaticInit();

	void Init();

	void Update();

	void Draw(UINT rootParameterIndex);

	void TransferConstBuffer();

	void SetLightDir(const Vector3& lightdir);

	void SetLightColor(const Vector3& color);
	//有効フラグ
	void SetActive(bool active) { this->active = active; }
	//有効フラグチェック
	bool IsAvtive() { return active; }

	Vector3 GetLightDir() { return lightdir; }

	Vector3 GetLightColor() { return lightcolor; }

	struct ConstBufferData {
		Vector3 lightv;		//ライトの方向を表すベクトル
		float pad1;
		Vector3 lightColor;	//ライトの色
		//float pad2;
		bool active;
	};
private:

	static DirectXCommon* directX;

	ComPtr<ID3D12Resource> constBuff;
	ConstBufferData* constMap;
	//ライト光線方向（単位ベクトル）
	Vector3 lightdir = { 1,0,0 };
	//ライトの色
	Vector3 lightcolor = { 1,1,1 };
	//ダーティフラグ
	bool dirty;

	//有効フラグ
	bool active = false;
};

