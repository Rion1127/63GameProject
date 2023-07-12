#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Color.h"
#include "myMath.h"
#include <vector>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class IPostEffect
{
private:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12Resource> texBuff_;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff_;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;

	//���_�f�[�^
	ComPtr<ID3D12Resource> vertBuff_;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	//�C���f�b�N�X�f�[�^
	std::vector<uint16_t> indices_;
	// �C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView_{};
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		int32_t timer;
	};
protected:
	ComPtr<ID3D12Resource> constBuff_ = nullptr;
private:
	//��ʃN���A�J���[
	static const float clearColor_[4];
	static const uint32_t vertNum_;
	struct VertexPosUV {
		Vector3 pos;
		Vector2 uv;
	};
public:
	IPostEffect();
	virtual ~IPostEffect() {};
	//�X�V
	void PUpdate();

	void Draw(std::string pipelineName);
	//�V�[���O����
	void PreDrawScene();
	//�V�[���`��㏈��
	void PostDrawScene();
protected:
	//�p�����override����֐��͉��̓����
	//�V�F�[�_�[�֒l��n��
	virtual void TransferBuff() = 0;
	virtual void SendToShader() = 0;
	//�R�}���h���X�g��Buff�̃A�h���X��ς�
	void SetBuff(uint32_t index, ID3D12Resource* constBuff);
private:
	void CreateVertBuff();
	void CreateibView();
	void CreateTexBuff();
	void CreateSRV();
	void CreateRTV();
	void CreateDepthBuff();
	void CreateDSV();
};

