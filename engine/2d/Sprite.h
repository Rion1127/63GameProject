#pragma once
#include <wrl.h>
#include <vector>
#include "math.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include <imgui.h>
#include <DirectXMath.h>
class Sprite
{
public:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static void StaticIni();
	void Ini(std::string guiname = "");
	//���W��������
	void SetPos(float x, float y) {
		pos_.x = x;
		pos_.y = y;
	};
	void SetPos(Vector2 pos);
	//��]������
	void SetRot(float rot);
	//�X�P�[���ݒ�
	void SetScale(float x, float y) {
		Scale_.x = x;
		Scale_.y = y;
	};
	void SetScale(Vector2 scale);
	//�摜�̒��S�����߂�
	void SetAnchor(float x, float y) {
		anchorPoint_.x = x;
		anchorPoint_.y = y;
	};
	void SetAnchor(Vector2 pos) {
		anchorPoint_ = pos;
	};
	//�F�ύX
	void ChangeColor(float x, float y, float z, float w) {
		color_ = {x, y, z, w};
	};
	void ChangeColor(DirectX::XMFLOAT4 color) {
		color_ = color;
	};
	//���E���]
	void SetFlipX(bool flip) { isFlipX_ = flip; }
	//�㉺���]
	void SetFlipY(bool flip) { isFlipY_ = flip; }
	//�\���t���O
	void SetInvisivle(bool invisivle) { isInvisible_ = invisivle; }
	//�摜����ύX
	void SetTex_LeftTop(Vector2 pos) { textureLeftTop_ = pos; }
	//�摜UV���W�ύX
	void SetTex_Size(Vector2 pos) { textureSize = pos; }

	Vector2 GetScale() { return Scale_; }
	Vector2 GetPos() { return pos_; }

	void SetImGui(bool flag) { isImguiDisplay = flag; }
	void DrawImGui();

	static void PreDraw();
	//�摜�T�C�Y�����擾(�`����W�͒��S)
	void Draw(UINT descriptorSize);
	//�摜�̒��_�f�[�^�������Ŏw��
	void Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize);
	/// <summary>
	/// �u�����h�ݒ�
	/// </summary>
	/// <param name="BLEND_ALPHA">�A���t�@�u�����h</param>
	/// <param name="BLEND_SUB">���Z����</param>
	/// <param name="BLEND_NEGA">�F���]����</param>
	/// <param name="BLEND_NORMAL">����������</param>
	static void SetBlend(int blend);

	static void AddAllNum() { allNum++; }
private:
	static int allNum;
	int spriteNum = 0;

	static DirectXCommon* directX_;
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	enum VertNum {
		LB,	//����
		LT,	//����
		RB,	//�E��
		RT	//�E��
	};

#pragma region ���_�f�[�^
	//���_�f�[�^
	std::vector<Vertex> vertices;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// ���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff = nullptr;

	Vertex* vertMap = nullptr;
#pragma endregion
#pragma region �C���f�b�N�X�f�[�^
	//�C���f�b�N�X�f�[�^
	std::vector<uint16_t> indices;
	// �C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
#pragma endregion

	ComPtr<ID3D12GraphicsCommandList> commandList_;
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;
	};
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	struct ConstBufferDataTransform {
		XMMATRIX mat;
	};
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
	XMMATRIX matProjection;

	// ���[���h�s��
	DirectX::XMMATRIX matWorld_{};
	//�F
	DirectX::XMFLOAT4 color_;
	//��]
	float rot_;
	//���W
	Vector2 pos_;
	//�X�P�[��
	Vector2 Scale_;
	//�A���J�[�|�C���g
	Vector2 anchorPoint_;
	//���E�t���b�v
	bool isFlipX_ = false;
	//�㉺�t���b�v
	bool isFlipY_ = false;
	//��\���t���O
	bool isInvisible_ = false;
	//�e�N�X�`��������W
	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	//�e�N�X�`���؂�o���T�C�Y
	Vector2 textureSize = { 0.f,0.f };
	//imgui�\��
	bool isImguiDisplay = false;

#pragma region
	std::string guiName_;
	int clicked = 0;
	const char* gui;

	std::string name;
#pragma endregion
};

