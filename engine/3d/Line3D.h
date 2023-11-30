#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "Color.h"
#include <wrl.h>
#include <vector>
class Line3D
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
protected:
	struct VertexPos
	{
		Vector3 pos = { 0, 0, 0 };
	};
	struct colorBuffer
	{
		Color color;
	};
private:
	std::vector<VertexPos> vertex_;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_;

	//色情報
	ComPtr<ID3D12Resource> constBuffColor_;
	//色情報
	colorBuffer* constMapColor_;

	uint32_t vertSize_;
	WorldTransform transform_;
	Color color_;
public:
	Line3D(uint32_t vertsize);

	void Update();
	void Draw();
public:
	uint32_t GetVertSize() { return vertSize_; }
	Color GetColor() { return color_; }
public:
	void SetVertPos(const std::vector<Vector3>& pos);
	void SetColor(Color col) { color_ = col; }
};

