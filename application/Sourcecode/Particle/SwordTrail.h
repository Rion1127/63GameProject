#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include "Timer.h"
#include "WorldTransform.h"
#include "Texture.h"

class SwordTrail
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	struct SwordTrailVertex
	{
		Vector3 pos = { 0, 0, 0 };
		Vector2 uv = { 0, 0 };
	};
public:
	struct PosBuffer
	{
		Vector3 head; //剣の先端の位置
		Vector3 tail; //剣の末端の位置
	};
private:
	std::vector<PosBuffer> posArray_;
	std::vector<SwordTrailVertex> vertex_;
	uint32_t vertSize_;
	//現在フレームでの剣の位置
	PosBuffer tempPos;
	//転送するかどうか
	bool isTransfer_;
	//頂点を追加する間隔
	Timer timer_;
	WorldTransform transform_;

	Texture texture_;

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_;
private:
	//デバッグ
	bool isStop_;
public:
	SwordTrail(uint32_t vertSize);
	void Update();
	void SetPos(const Vector3& head, const Vector3& tail);

	void Draw();
private:
	//データ転送
	void TransferBuff();
};

