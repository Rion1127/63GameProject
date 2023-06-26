#pragma once
#include <wrl.h>
#include "Camera.h"
#include "myMath.h"

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

	void Update(Camera VP);

	void Draw(uint32_t descriptorSize);

private:

	struct Vertex {
		Vector3 pos;
		float scale;
		DirectX::XMFLOAT4 color;
	};

	const uint32_t CVERTEX_COUNT = 1024;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Vertex* vertMap_ = nullptr;

	struct ConstBufferData {
		Matrix4 mat;
		Matrix4 billBoardMat;
	};

	ComPtr<ID3D12Resource> constBuff_ = nullptr;
	ConstBufferData* constMatMap_ = nullptr;

	uint32_t activeCount_ = 2;
};

