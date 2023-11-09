#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include "Timer.h"
#include "WorldTransform.h"
#include "Texture.h"
#include "Color.h"

/**
 * @file ParticleTrail.h
 * @brief 軌道エフェクトを作るクラス
 */

class ParticleTrail
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
protected:
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
	struct colorBuffer
	{
		Color color;
	};
protected:
	std::vector<PosBuffer> posArray_;
	std::vector<SwordTrailVertex> vertex_;
	uint32_t vertSize_;
	//現在フレームでの剣の位置
	PosBuffer tempPos;

	WorldTransform transform_;
	Texture texture_;

	bool isVisible_;

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_;

	//色情報
	ComPtr<ID3D12Resource> constBuffColor_;
	//色情報
	colorBuffer* constMapColor_;

	Color color_;
protected:
	//デバッグ
	bool isStop_;
public:
	ParticleTrail(uint32_t vertSize);
	void Update();
	void SetPos(const Vector3& head, const Vector3& tail);
	void SetIsVisible(bool flag) { isVisible_ = flag; }
	void SetColor(Color color) { color_ = color; }

	void Draw();
	void DrawImgui();
private:
	//データ転送
	void TransferBuff();
	void CreateCurveVertex(std::vector<PosBuffer>& usedPosArray);
public:
	Color GetColor() { return color_; }
};

