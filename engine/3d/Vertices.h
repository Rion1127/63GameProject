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
		XMFLOAT3 pos;		//xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		//uv���W
	};
	//�G�C���A�X�e���v���[�g
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
	
	// ���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	//�C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};

public:
	//���_�f�[�^
	std::vector<VertexPosNormalUv> vertices;
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	//���_�C���f�b�N�X
	std::vector<uint16_t> indices;
};

