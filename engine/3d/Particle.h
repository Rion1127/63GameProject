#pragma once
#include <wrl.h>
#include "Camera.h"

using namespace Microsoft::WRL;
class Particle
{
public:
	Particle();
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Particle* GetInstance();

	void Ini();

	static void PreDraw();

	/// <summary>
	/// ブレンド設定
	/// </summary>
	/// <param name="BLEND_ALPHA">アルファブレンド</param>
	/// <param name="BLEND_SUB">減算合成</param>
	/// <param name="BLEND_NEGA">色反転合成</param>
	/// <param name="BLEND_NORMAL">半透明合成</param>
	static void SetBlend(int blend);

	void Update(Camera VP);


	void ChangeColor(float x, float y, float z, float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(uint32_t descriptorSize);

private:
	static DirectXCommon* directX_;

	struct Vertex {
		Vector3 pos;
		float scale;
		DirectX::XMFLOAT4 color;
	};

	const int vertexCount = 1024;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	Vertex* vertMap_ = nullptr;

	struct ConstBufferData {
		DirectX::XMMATRIX mat;
		DirectX::XMMATRIX billBoardMat;
	};

	ComPtr<ID3D12Resource> constBuff = nullptr;
	ConstBufferData* constMatMap_ = nullptr;

	int activeCount = 2;
};

