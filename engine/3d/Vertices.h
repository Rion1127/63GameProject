#pragma once
#include <wrl.h>
#include <d3d12.h>

#include <cassert>
#include <vector>
#include "Material.h"
#include "WorldTransform.h"
#include <array>
#include "myMath.h"

class Vertices
{
public:
	const static int MAX_BONES_VERTICES = 4;
	struct VertexPosNormalUv {
		Vector3 pos;		//xyz���W
		Vector3 normal;	//�@���x�N�g��
		Vector2 uv;		//uv���W
		std::array<uint32_t, MAX_BONES_VERTICES> m_BoneIDs;
		std::array<float, MAX_BONES_VERTICES> m_Weights;
	};
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Ini(ID3D12Device* device);

	void CreateBuffer();

	void AddVertices(const VertexPosNormalUv& vertex) { vertices_.push_back(vertex); }
	void AddIndex(const uint16_t& vertex) { indices_.push_back(vertex); }

	size_t GetVertexCount() { return vertices_.size(); }

	//void ChangeColor(float x, float y, float z, float w);
	//void ChangeColor(XMFLOAT4 color_);

	inline const std::vector<uint16_t>& GetIndices() { return indices_; }

	void Map();

	void Draw(uint32_t indexSize,
		WorldTransform* worldTransform,
		UINT descriptorSize);

	void Draw(
		const WorldTransform& worldTransform);

	void DrawInstanced(
		WorldTransform* worldTransform,
		UINT descriptorSize);
private:
	
	// ���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	//�C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView_{};

public:
	//���_�f�[�^
	std::vector<VertexPosNormalUv> vertices_;
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	//���_�C���f�b�N�X
	std::vector<uint16_t> indices_;

	std::string materialName_;
};

