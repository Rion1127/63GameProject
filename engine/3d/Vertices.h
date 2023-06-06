#pragma once
#include <wrl.h>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <cassert>
#include <vector>
#include "Material.h"
#include "WorldTransform.h"

class Vertices
{
public:
	struct VertexPosNormalUv {
		XMFLOAT3 pos;		//xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;		//uv座標
	};
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Ini(ID3D12Device* device);

	void CreateBuffer();

	void AddVertices(const VertexPosNormalUv& vertex) { vertices.push_back(vertex); }
	void AddIndex(const uint16_t& vertex) { indices.push_back(vertex); }

	size_t GetVertexCount() { return vertices.size(); }

	//void ChangeColor(float x, float y, float z, float w);
	//void ChangeColor(XMFLOAT4 color_);

	inline const std::vector<uint16_t>& GetIndices() { return indices; }

	void Map();

	void Draw(uint32_t indexSize,
		WorldTransform* worldTransform,
		UINT descriptorSize);

	void Draw(
		WorldTransform* worldTransform,
		UINT descriptorSize);

	void DrawInstanced(
		WorldTransform* worldTransform,
		UINT descriptorSize);
private:
	
	// 頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	//インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};

public:
	//頂点データ
	std::vector<VertexPosNormalUv> vertices;
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	//頂点インデックス
	std::vector<uint16_t> indices;
};

