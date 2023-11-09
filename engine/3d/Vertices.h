#pragma once
#include <wrl.h>
#include <d3d12.h>

#include <cassert>
#include <vector>
#include "Material.h"
#include "WorldTransform.h"
#include <array>
#include "myMath.h"

/**
 * @file Vertices.h
 * @brief 頂点データを管理しているクラス
 */

class Vertices
{
public:
	const static int MAX_BONES_VERTICES = 4;
	struct VertexPosNormalUv {
		Vector3 pos;		//xyz座標
		Vector3 normal;	//法線ベクトル
		Vector2 uv;		//uv座標
		std::array<uint32_t, MAX_BONES_VERTICES> m_BoneIDs;
		std::array<float, MAX_BONES_VERTICES> m_Weights;
	};
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Ini();

	void CreateBuffer();

	void AddVertices(const VertexPosNormalUv& vertex) { vertices_.push_back(vertex); }
	void AddIndex(const uint16_t& vertex) { indices_.push_back(vertex); }

	size_t GetVertexCount() { return vertices_.size(); }

	inline const std::vector<uint16_t>& GetIndices() { return indices_; }

	void Map();

	void Draw(uint32_t indexSize,WorldTransform* worldTransform);

	void Draw(
		const WorldTransform& worldTransform);

	void DrawInstanced(WorldTransform* worldTransform);
private:

	// 頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	//インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView_{};

public:
	//頂点データ
	std::vector<VertexPosNormalUv> vertices_;
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	//頂点インデックス
	std::vector<uint16_t> indices_;

	std::string materialName_;
};

