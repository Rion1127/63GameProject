#pragma once
#include "DirectX.h"
#include "IPostEffect.h"
class Noise final:
	public IPostEffect {
private:
	struct ConstBuffTime {
		int32_t time = 0;
		float power;
		float width;
	};

	int32_t time_;
private:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//�萔�o�b�t�@�p�f�[�^�\����
	
	ConstBuffTime* constMap_;
protected:
	//�V�F�[�_�[�֒l��n��
	void TransferBuff() override;
	void SendToShader() override;
public:
	Noise();
	void PreDraw();
};